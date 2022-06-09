#include "stdafx.h"
#include "GameObject/Client_Object.h"

CGameObject_3D_Tile::CGameObject_3D_Tile(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Static(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_STATIC_TILES;
}

CGameObject_3D_Tile::CGameObject_3D_Tile(const CGameObject_3D_Tile& rhs)
	: CGameObject_3D_Static(rhs)
{
	for (int i=0; i<TILETYPE_END;++i)
	{
		mTileNames[i] = rhs.mTileNames[i];
	}
	//mComModel_deco = nullptr;
}

HRESULT CGameObject_3D_Tile::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	// 이름 정의
	mTileNames[TILETYPE_TOP] = "tileset_GreyRock_TOP.fbx";
	mTileNames[TILETYPE_WALL] = "tileset_GreyRock_1U_00.fbx";
	mTileNames[TILETYPE_CONER] = "tileset_GreyRock_CO_00.fbx";
	mTileNames[TILETYPE_FLOOR] = "tileset_GreyRock_FLOOR.fbx";
	mTileNames[TILETYPE_BEHIND] = "tileset_GreyRock_CC_Cap.fbx";
	mTileNames[TILETYPE_DIAG] = "tileset_GreyRock_DIAG_00.fbx";

	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_3D_Tile::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	mCollider_Desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_AABB;
	float size = 0.4f;
	float sizet = 0.6f;
	mCollider_Desc.mOffset = _float3(0, 0.8f, 0);
	mCollider_Desc.mSize = _float3(size, size*2.0f, size);
	Set_LoadColliderDESC(mCollider_Desc);
	Set_Component();	
	return S_OK;
}

_int CGameObject_3D_Tile::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	switch (meTileTickState)
	{
	case Client::CGameObject_3D_Tile::E_TICKE_NONE:
		mCurrentShaderPass = 0;
		break;
	case Client::CGameObject_3D_Tile::E_TICKE_PICK:
		mCurrentShaderPass = 1;
		meTileTickState = CGameObject_3D_Tile::E_TICKE_NONE;
		break;
	case Client::CGameObject_3D_Tile::E_TICKE_TASK:
		mCurrentShaderPass = 3;
		break;
	case Client::CGameObject_3D_Tile::E_TICKE_END:
		break;
	default:
		break;
	}



	GetSingle(CGameManager)->Add_ColliderObject(CColliderManager::E_COLLIDEROBJ_TYPE::COLLIDEROBJ_STATIC, this);

	return UPDATENONE;
}

HRESULT CGameObject_3D_Tile::Render()
{
	FAILED_CHECK(__super::Render());
	return S_OK;
}

HRESULT CGameObject_3D_Tile::CollisionFunc(_float3 PickPosition, _float dist)
{
	FAILED_CHECK(__super::CollisionFunc(PickPosition,dist));
	   
	if (meTileTickState == CGameObject_3D_Tile::E_TICKE_TASK)
		return S_OK;

	//if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON)& DIS_Down)
	//{
	//	meTileTickState = CGameObject_3D_Tile::E_TICKE_TASK;
	//	RemoveThisTile();
	//	return S_OK;
	//}

	//if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_WHEEL)& DIS_Down)
	//{
	//	Update_Debug_TILESTATE();
	//	return S_OK;
	//}


	if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON)& DIS_Down)
	{
		Add_TileTask_this();
		return S_OK;
	}

	meTileTickState = CGameObject_3D_Tile::E_TICKE_PICK;
	return S_OK;

	


	return S_OK;
}

void CGameObject_3D_Tile::Add_TileTask_this()
{
	// 타일 채굴에 넣기
	if (meTileDigType == CGameObject_3D_Tile::TILE_DIGTYPE_WALL)
	{
		meTileTickState = CGameObject_3D_Tile::E_TICKE_TASK;
		GetSingle(CGameManager)->Get_DaungonManager()->Add_Task_Tile(mIndex);
	}
	else if (meTileDigType == CGameObject_3D_Tile::TILE_DIGTYPE_GOLD)
	{
		meTileTickState = CGameObject_3D_Tile::E_TICKE_TASK;
		GetSingle(CGameManager)->Get_DaungonManager()->Add_Task_Gold(mIndex);
	}
}

HRESULT CGameObject_3D_Tile::Set_LoadNewFBX(E_TILETYPE type)
{
	MODEL_STATIC_DESC desc;
	strcpy_s(desc.mModelName, mTileNames[type].c_str());
	Set_LoadModelDESC(desc);
	return S_OK;
}

bool CGameObject_3D_Tile::Set_EmptySearchNeighbor(int searchIndex)
{
	for (int i = 0; i < NEIGHBOR_TILE_END; ++i)
	{
		if (searchIndex == mNeighborIndex[i])
		{
			mNeighborIndex[i] = -1;
			return false;
		}
	}
	return false;
}

void CGameObject_3D_Tile::Set_ColliderPosition()
{
	mComCollider->Update_Transform(mComTransform->GetWorldFloat4x4());
}

_bool CGameObject_3D_Tile::Get_IsBlocked() const
{
	if (
		(mNeighborIndex[E_NEIGHBOR_TILE::NEIGHBOR_TILE_LEFT] == -1	)||
		(mNeighborIndex[E_NEIGHBOR_TILE::NEIGHBOR_TILE_TOP] == -1	)||
		(mNeighborIndex[E_NEIGHBOR_TILE::NEIGHBOR_TILE_RIGHT] == -1 )||
		(mNeighborIndex[E_NEIGHBOR_TILE::NEIGHBOR_TILE_BOTTOM] == -1)
		)
		return false;

	return true;
}

_float3 CGameObject_3D_Tile::Get_AbleTilePos(_float offset)
{
	// 상하좌우를 검사해서 갈 뚫려있는 위치 반환

	_float offsetPos = offset;

	_float3 worldPos = Get_WorldPostition();
	_float3 goalPos = worldPos;

	if (mNeighborIndex[E_NEIGHBOR_TILE::NEIGHBOR_TILE_LEFT] == -1)
	{ 
		goalPos = _float3(worldPos.x - offsetPos,worldPos.y,worldPos.z);

	}

	else if (mNeighborIndex[E_NEIGHBOR_TILE::NEIGHBOR_TILE_RIGHT] == -1)
	{
		goalPos = _float3(worldPos.x + offsetPos, worldPos.y, worldPos.z);

	}

	else if (mNeighborIndex[E_NEIGHBOR_TILE::NEIGHBOR_TILE_TOP] == -1)
	{
		goalPos = _float3(worldPos.x , worldPos.y, worldPos.z + offsetPos);

	}

	else if (mNeighborIndex[E_NEIGHBOR_TILE::NEIGHBOR_TILE_BOTTOM] == -1)
	{
		goalPos = _float3(worldPos.x , worldPos.y, worldPos.z - offsetPos);

	}

	return goalPos;
}

HRESULT CGameObject_3D_Tile::RemoveThisTile()
{
	// 상하좌우에 연결된 타일이 있다면 자신의 인덱스를 지운다.
	// 타일 리스트 가져오기

	for (int i = 0; i < NEIGHBOR_TILE_END; ++i)
	{
		if (mNeighborIndex[i] != -1)
		{
			CGameObject_3D_Tile* findtile = GetSingle(CGameManager)->Get_DaungonManager()->FInd_TIleForIndex(mNeighborIndex[i]);
			if (findtile == nullptr)
				continue;

			findtile->Set_EmptySearchNeighbor(mIndex);
			findtile->Update_NeighborTile();

		}
	}
	FAILED_CHECK(GetSingle(CGameManager)->Get_DaungonManager()->Setup_TileState(mIndex));
	FAILED_CHECK(GetSingle(CGameManager)->Get_DaungonManager()->RemoveTile(this));
	Set_Dead();
	return S_OK;

}

HRESULT CGameObject_3D_Tile::Update_NeighborTile()
{
	// 타일이 삭제됐다고 가정하고 -1인 지점에 따라 타일이 변경된다.	

	// 타일 상태 결정
	meTileState = CGameObject_3D_Tile::TILESTATE_TOP;

	// 4
	if (
		mNeighborIndex[NEIGHBOR_TILE_LEFT] == -1 &&
		mNeighborIndex[NEIGHBOR_TILE_TOP] == -1 &&
		mNeighborIndex[NEIGHBOR_TILE_RIGHT] == -1 &&
		mNeighborIndex[NEIGHBOR_TILE_BOTTOM] == -1
		)
	{
		meTileState = CGameObject_3D_Tile::TILESTATE_DIAGTEST_BOTTOM;
	}

	// 3
	else if (
		mNeighborIndex[NEIGHBOR_TILE_LEFT] == -1 &&
		mNeighborIndex[NEIGHBOR_TILE_RIGHT] == -1 &&
		mNeighborIndex[NEIGHBOR_TILE_BOTTOM] == -1
		)
	{
		meTileState = CGameObject_3D_Tile::TILESTATE_CONER_RB;
	}
	else if (
		mNeighborIndex[NEIGHBOR_TILE_LEFT] == -1 &&
		mNeighborIndex[NEIGHBOR_TILE_RIGHT] == -1 &&
		mNeighborIndex[NEIGHBOR_TILE_TOP] == -1
		)
	{
		meTileState = CGameObject_3D_Tile::TILESTATE_CONER_RT;
	}



	
	else if (
		mNeighborIndex[NEIGHBOR_TILE_LEFT] == -1 &&
		mNeighborIndex[NEIGHBOR_TILE_RIGHT] == -1 
		)
	{
		meTileState = CGameObject_3D_Tile::TILESTATE_WALL_RIGHT;
	}

	// L
	else if (mNeighborIndex[NEIGHBOR_TILE_LEFT] == -1)
	{
		if (mNeighborIndex[NEIGHBOR_TILE_TOP] == -1)
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_CONER_LT;
		}
		else if (mNeighborIndex[NEIGHBOR_TILE_BOTTOM] == -1)
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_CONER_LB;
		}
		else
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_WALL_LEFT;
		}

	}
	// R
	else if (mNeighborIndex[NEIGHBOR_TILE_RIGHT] == -1)
	{
		if (mNeighborIndex[NEIGHBOR_TILE_TOP] == -1)
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_CONER_RT;

		}
		else if (mNeighborIndex[NEIGHBOR_TILE_BOTTOM] == -1)
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_CONER_RB;

		}
		else
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_WALL_RIGHT;

		}

	}

	// T
	else if (mNeighborIndex[NEIGHBOR_TILE_TOP] == -1)
	{
		if (mNeighborIndex[NEIGHBOR_TILE_LEFT] == -1)
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_CONER_LT;


		}
		else if (mNeighborIndex[NEIGHBOR_TILE_RIGHT] == -1)
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_CONER_RT;

		}
		else
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_WALL_TOP;


		}

	}

	// B
	else if (mNeighborIndex[NEIGHBOR_TILE_BOTTOM] == -1)
	{
		if (mNeighborIndex[NEIGHBOR_TILE_LEFT] == -1)
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_CONER_LB;

		}
		else if (mNeighborIndex[NEIGHBOR_TILE_RIGHT] == -1)
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_CONER_RB;

		}
		else
		{
			meTileState = CGameObject_3D_Tile::TILESTATE_WALL_BOTTOM;
		}

	}
	// 대각선
	else if (mNeighborIndex[NEIGHBOR_TILE_LT] == -1)
	{
		meTileState = CGameObject_3D_Tile::TILESTATE_BEHIND_TOP;

	}

	else if (mNeighborIndex[NEIGHBOR_TILE_RT] == -1)
	{
		meTileState = CGameObject_3D_Tile::TILESTATE_BEHIND_RIGHT;

	}
	else if (mNeighborIndex[NEIGHBOR_TILE_RB] == -1)
	{ 
		meTileState = CGameObject_3D_Tile::TILESTATE_BEHIND_BOTTOM;

	}
	else if (mNeighborIndex[NEIGHBOR_TILE_LB] == -1)
	{
		meTileState = CGameObject_3D_Tile::TILESTATE_BEHIND_LEFT;

	}


	
	// 실제 모델 변경
	Update_TILESTATE();
	return S_OK;
}

HRESULT CGameObject_3D_Tile::Update_Tile_CrossCheck()
{
	// 대각선 검사
	// 왼쪽 / 위가 유효하면 왼쪽 위가 있음
	if (mNeighborIndex[NEIGHBOR_TILE_TOP] != -1)
	{
		if (mNeighborIndex[NEIGHBOR_TILE_LEFT] != -1)
			mNeighborIndex[NEIGHBOR_TILE_LT] = mNeighborIndex[NEIGHBOR_TILE_TOP] - 1;
		if (mNeighborIndex[NEIGHBOR_TILE_RIGHT] != -1)
			mNeighborIndex[NEIGHBOR_TILE_RT] = mNeighborIndex[NEIGHBOR_TILE_TOP] + 1;
	}

	if (mNeighborIndex[NEIGHBOR_TILE_BOTTOM] != -1)
	{
		if (mNeighborIndex[NEIGHBOR_TILE_LEFT] != -1)
			mNeighborIndex[NEIGHBOR_TILE_LB] = mNeighborIndex[NEIGHBOR_TILE_BOTTOM] - 1;
		if (mNeighborIndex[NEIGHBOR_TILE_RIGHT] != -1)
			mNeighborIndex[NEIGHBOR_TILE_RB] = mNeighborIndex[NEIGHBOR_TILE_BOTTOM] + 1;
	}
	

	return S_OK;
}

void CGameObject_3D_Tile::Update_TILESTATE()
{
	switch (meTileState)
	{
	case Client::CGameObject_3D_Tile::TILESTATE_TOP:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_TOP);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(0));

		break;
	case Client::CGameObject_3D_Tile::TILESTATE_WALL_LEFT:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_WALL);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(0));

		break;
	case Client::CGameObject_3D_Tile::TILESTATE_WALL_TOP:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_WALL);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(90));


		break;
	case Client::CGameObject_3D_Tile::TILESTATE_WALL_RIGHT:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_WALL);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(180));


		break;
	case Client::CGameObject_3D_Tile::TILESTATE_WALL_BOTTOM:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_WALL);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(270));


		break;
	case Client::CGameObject_3D_Tile::TILESTATE_CONER_LB:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(0));

		break;
	case Client::CGameObject_3D_Tile::TILESTATE_CONER_LT:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(90));

		break;
	case Client::CGameObject_3D_Tile::TILESTATE_CONER_RT:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(180));

		break;
	case Client::CGameObject_3D_Tile::TILESTATE_CONER_RB:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(270));
		break;
	case CGameObject_3D_Tile::TILESTATE_FLOOR:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_FLOOR);
		break;

	case Client::CGameObject_3D_Tile::TILESTATE_BEHIND_LEFT:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_BEHIND);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(0));
		break;
	case Client::CGameObject_3D_Tile::TILESTATE_BEHIND_TOP:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_BEHIND);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(90));
		break;
	case Client::CGameObject_3D_Tile::TILESTATE_BEHIND_RIGHT:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_BEHIND);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(180));
		break;
	case Client::CGameObject_3D_Tile::TILESTATE_BEHIND_BOTTOM:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_BEHIND);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(270));
		break;

	case Client::CGameObject_3D_Tile::TILESTATE_DIAGTEST_LEFT:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_DIAG);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(0));

		break;
	case Client::CGameObject_3D_Tile::TILESTATE_DIAGTEST_TOP:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_DIAG);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(90));

		break;
	case Client::CGameObject_3D_Tile::TILESTATE_DIAGTEST_RIGHT:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_DIAG);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(180));

		break;
	case Client::CGameObject_3D_Tile::TILESTATE_DIAGTEST_BOTTOM:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_DIAG);
		mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(270));

		break;

	default:
		Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_TOP);
		break;
	}
}

void CGameObject_3D_Tile::Update_Debug_TILESTATE()
{
	int enumTIle = meTileState;
	enumTIle++;
	enumTIle %= (E_TILESTATE)TILESTATE_END;
	meTileState = (E_TILESTATE)enumTIle;
	Update_TILESTATE();
}

CGameObject_3D_Tile * CGameObject_3D_Tile::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_3D_Tile*	pInstance = NEW CGameObject_3D_Tile(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Tile");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_3D_Tile* CGameObject_3D_Tile::Clone(void* pArg)
{
	CGameObject_3D_Tile*	pInstance = NEW CGameObject_3D_Tile(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Tile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D_Tile::Free()
{
	__super::Free();

//	Safe_Release(mComModel_deco);

}