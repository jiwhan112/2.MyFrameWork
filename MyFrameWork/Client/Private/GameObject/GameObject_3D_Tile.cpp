#include "stdafx.h"
#include "GameObject/Daungon_Manager.h"
#include "GameObject/GameObject_3D_Tile.h"
#include "GameObject/GameObject_3D_Static2.h"

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
	mCollider_Desc.mOffset = _float3(0, 1.3f, 0);
	mCollider_Desc.mSize = _float3(size, size, size);
	Set_LoadColliderDESC(mCollider_Desc);
	Set_Component();
	
	return S_OK;
}

_int CGameObject_3D_Tile::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	// #TODO: 충돌 매니저 만들기
	// 중복 충돌 막기
	// 마우스 충돌은 한 프레임에 한번만 하게 수정
	GetSingle(CGameManager)->Add_ColliderObject(CColliderManager::E_COLLIDEROBJ_TYPE::COLLIDEROBJ_STATIC, this);

	return UPDATENONE;
}

HRESULT CGameObject_3D_Tile::Render()
{
	FAILED_CHECK(__super::Render());
	
	//if (mComModel_deco != nullptr)
	//{
	//	_uint iNumMaterials = mComModel_deco->Get_NumMaterials();

	//	// 재질 개수만큼 루프
	//	for (int i = 0; i < iNumMaterials; ++i)
	//	{
	//		// 1. Diffuse 텍스처 설정
	//		mComModel_deco->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);

	//		// 2. 랜더링
	//		// 여기서 뼈를 넘긴다.
	//		FAILED_CHECK(mComModel_deco->Render(mComShader, mCurrentShaderPass, 0));
	//	}

	//}
	return S_OK;
}

HRESULT CGameObject_3D_Tile::CollisionFunc(_float3 PickPosition, _float dist)
{
	FAILED_CHECK(__super::CollisionFunc(PickPosition,dist));
	   
	if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON)& DIS_Down)
	{
		if (mCurrentShaderPass == 0)
			mCurrentShaderPass = 1;
		else
			mCurrentShaderPass = 0;
	}

	if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_WHEEL)& DIS_Down)
	{
		Update_Debug_TILESTATE();
	}

	if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON)& DIS_Down)
	{
		// 상하좌우에 연결된 타일이 있다면 자신의 인덱스를 지운다.
		// 타일 리스트 가져오기

		for (int i =0;i< NEIGHBOR_TILE_END;++i)
		{
			if (mNeighborIndex[i] != -1)
			{
				CGameObject_3D_Tile* findtile = GetSingle(CGameManager)->Get_DaungonManager()->FInd_TIleForIndex(mNeighborIndex[i]);
				if(findtile == nullptr)
					continue;

				findtile->Set_EmptySearchNeighbor(mIndex);
				findtile->Update_NeighborTile();
				
			}
		}	
		GetSingle(CGameManager)->Get_DaungonManager()->RemoveTile(this);
		Set_Dead();
	}
	return S_OK;
}



HRESULT CGameObject_3D_Tile::Set_LoadNewFBX(E_TILETYPE type)
{
	MODEL_STATIC_DESC desc;
	strcpy_s(desc.mModelName, mTileNames[type].c_str());
	Set_LoadModelDESC(desc);
	return S_OK;
}

//HRESULT CGameObject_3D_Tile::Set_LoadModel_Deco_DESC(E_TILETYPE tileIndex)
//{
//	if (tileIndex > TILETYPE_END)
//		return -1;
//
//	// 해당 모델 컴포넌트로 변경
//	if (mComModel_deco != nullptr)
//	{
//		Safe_Release(mComModel_deco);
//		mComModel_deco = nullptr;
//	}
//	FAILED_CHECK(__super::Release_Component(TEXT("Com_Model_Deco")));
//
//	if (tileIndex == TILETYPE_END)
//		return S_OK;
//
//	string strModel = mTileNames[tileIndex];
//	wstring ModelName = CHelperClass::Convert_str2wstr(strModel);
//
//	
//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, ModelName.c_str(), TEXT("Com_Model_Deco"), (CComponent**)&mComModel_deco));
//	return S_OK;
//}

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

HRESULT CGameObject_3D_Tile::Update_NeighborTile()
{
	// 타일이 삭제됐다고 가정하고 -1인 지점에 따라 타일이 변경된다.	

	// 타일 상태 결정
	meTileState = CGameObject_3D_Tile::TILESTATE_TOP;


	// L
	if (mNeighborIndex[NEIGHBOR_TILE_LEFT] == -1)
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


	Update_TILESTATE();


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