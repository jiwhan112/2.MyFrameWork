#include "stdafx.h"
#include "GameObject/GameObject_Goblin.h"
#include "AI/AI_Action.h"

CGameObject_Goblin::CGameObject_Goblin(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_GOBLIN;
}

CGameObject_Goblin::CGameObject_Goblin(const CGameObject_Goblin& rhs)
	: CGameObject_3D_Dynamic(rhs)
{

}

HRESULT CGameObject_Goblin::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_Goblin::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	string str("crea_Goblin.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);
	return S_OK;
}


HRESULT CGameObject_Goblin::Init_Unit()
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

}

HRESULT CGameObject_Goblin::Init_AI()
{
	

	return S_OK;
}


CGameObject_Goblin * CGameObject_Goblin::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Goblin*	pInstance = NEW CGameObject_Goblin(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Goblin");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Goblin* CGameObject_Goblin::Clone(void* pArg)
{
	CGameObject_Goblin*	pInstance = NEW CGameObject_Goblin(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Goblin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Goblin::Free()
{
	__super::Free();

}
