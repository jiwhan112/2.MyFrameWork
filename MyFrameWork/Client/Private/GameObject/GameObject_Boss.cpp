#include "stdafx.h"
#include "GameObject/GameObject_BOSS.h"

CGameObject_BOSS::CGameObject_BOSS(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_ORC;
}

CGameObject_BOSS::CGameObject_BOSS(const CGameObject_BOSS& rhs)
	: CGameObject_3D_Dynamic(rhs)
{

}

HRESULT CGameObject_BOSS::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_BOSS::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	mComModel->SetUp_AnimIndex(0);


	return S_OK;
}


HRESULT CGameObject_BOSS::Init_Unit()
{
	__super::Init_Unit();

	return S_OK;
}

HRESULT CGameObject_BOSS::Init_AI()
{
	return E_NOTIMPL;
}


CGameObject_BOSS * CGameObject_BOSS::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_BOSS*	pInstance = NEW CGameObject_BOSS(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_BOSS");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_BOSS* CGameObject_BOSS::Clone(void* pArg)
{
	CGameObject_BOSS*	pInstance = NEW CGameObject_BOSS(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_BOSS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_BOSS::Free()
{
	__super::Free();
}
