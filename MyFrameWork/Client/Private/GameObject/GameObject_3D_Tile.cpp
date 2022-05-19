#include "stdafx.h"
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
}

HRESULT CGameObject_3D_Tile::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	// 이름 정의
	mTileNames[TILETYPE_TOP] = "tileset_GreyRock_TOP.fbx";
	mTileNames[TILETYPE_WALL] = "tileset_GreyRock_1U_00.fbx";
	mTileNames[TILETYPE_CONER] = "tileset_GreyRock_CO_00.fbx";
	mTileNames[TILETYPE_FLOOR] = "tileset_GreyRock_FLOOR.fbx";
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

	if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON)& DIS_Down)
	{
		// 상하좌우에 연결된 타일이 있다면 자신의 인덱스를 지운다.
		// 타일 리스트 가져오기

		for (int i =0;i< NEIGHBOR_TILE_END;++i)
		{
			if(mNeighborIndex[i] == -1)
				continue;


		}

		
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

HRESULT CGameObject_3D_Tile::Update_NeighborTile()
{
	// 타일이 삭제됐다고 가정하고 -1인 지점에 따라 타일이 변경된다.
	
	// 타일이 하나가 없다면 위아래를 비교하고 설정한다.

	// L
	if (mNeighborIndex[NEIGHBOR_TILE_LEFT] == -1)
	{
		if (mNeighborIndex[NEIGHBOR_TILE_TOP] == -1)
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
			mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(90));
		}
		else if (mNeighborIndex[NEIGHBOR_TILE_BOTTOM] == -1)
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
		}
		else
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_WALL);

		}

		return S_OK;
	}
	// R
	if (mNeighborIndex[NEIGHBOR_TILE_RIGHT] == -1)
	{
		if (mNeighborIndex[NEIGHBOR_TILE_TOP] == -1)
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
			mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(180));
		}
		else if (mNeighborIndex[NEIGHBOR_TILE_BOTTOM] == -1)
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
			mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(270));
		}
		else
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_WALL);
			mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(180));
		}

		return S_OK;
	}

	// T
	if (mNeighborIndex[NEIGHBOR_TILE_TOP] == -1)
	{
		if (mNeighborIndex[NEIGHBOR_TILE_LEFT] == -1)
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
			mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(90));

		}
		else if (mNeighborIndex[NEIGHBOR_TILE_RIGHT] == -1)
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
			mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(90));

		}
		else
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_WALL);
			mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(90));

		}

		return S_OK;
	}

	// B
	if (mNeighborIndex[NEIGHBOR_TILE_BOTTOM] == -1)
	{
		if (mNeighborIndex[NEIGHBOR_TILE_LEFT] == -1)
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
			mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(270));


		}
		else if (mNeighborIndex[NEIGHBOR_TILE_RIGHT] == -1)
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_CONER);
			mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(270));

		}
		else
		{
			Set_LoadNewFBX(CGameObject_3D_Tile::E_TILETYPE::TILETYPE_WALL);
			mComTransform->Rotation(_float3(0, 1, 0), XMConvertToRadians(270));

		}

		return S_OK;
	}


	// 두개 없을 떄 

	return S_OK;
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
}