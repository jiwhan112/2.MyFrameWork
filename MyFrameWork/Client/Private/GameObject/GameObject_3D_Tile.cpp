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
	float size = 0.5f;
	mCollider_Desc.mSize = _float3(size, 0.5f, size);
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
		/// 타일 변경

		// 타일 돌리기
		mComTransform->Rotation_Add(_float3(0, 1, 0), XMConvertToRadians(90));
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

//HRESULT CGameObject_3D_Tile::Create_Model()
//{
//	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
//
//	_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
//
//	// 참조로 넣어야한다.
//	for (int i =0;i< TILETYPE_END; i++)
//	{
//		CGameObject_3D_Static2* tileobj = (CGameObject_3D_Static2*)Create_Manager->
//			Create_ObjectClone_Prefab(idx, mTileNames[i].c_str(), TAGLAY(LAY_CUBETILE));
//
//		mTileObject[i] = tileobj;
//		Safe_AddRef(mTileObject[i]);
//	}	
//
//	return S_OK;
//}


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