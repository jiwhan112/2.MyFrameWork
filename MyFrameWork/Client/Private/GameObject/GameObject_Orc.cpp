#include "stdafx.h"
#include "GameObject/GameObject_Orc.h"
#include "AI/AI_Action.h"


CGameObject_Orc::CGameObject_Orc(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_ORC;
}

CGameObject_Orc::CGameObject_Orc(const CGameObject_Orc& rhs)
	: CGameObject_3D_Dynamic(rhs)
{

}

HRESULT CGameObject_Orc::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());
	mCurrentShaderPass = 0;
	return S_OK;
}

HRESULT CGameObject_Orc::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_WORLD);

	string str("crea_Orc.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);

	return S_OK;
}


HRESULT CGameObject_Orc::Init_Unit()
{
	_float size = 0.8f;

	mComTransform->Scaled(_float3(size, size, size));

	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc.mSize = _float3(size, size, size);
	Add_ColliderDesc(&desc, 1);
	Update_Collider();

	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE));
	return S_OK;
	return S_OK;
}

HRESULT CGameObject_Orc::Init_AI()
{
	

	return S_OK;
}


CGameObject_Orc * CGameObject_Orc::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Orc*	pInstance = NEW CGameObject_Orc(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Orc");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Orc* CGameObject_Orc::Clone(void* pArg)
{
	CGameObject_Orc*	pInstance = NEW CGameObject_Orc(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Orc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Orc::Free()
{
	__super::Free();


	//	Safe_Release(mComNaviMesh);
}
