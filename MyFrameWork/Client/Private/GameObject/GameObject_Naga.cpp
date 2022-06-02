#include "stdafx.h"
#include "GameObject/GameObject_Naga.h"

CGameObject_Naga::CGameObject_Naga(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_ORC;
}

CGameObject_Naga::CGameObject_Naga(const CGameObject_Naga& rhs)
	: CGameObject_3D_Dynamic(rhs)
{

}

HRESULT CGameObject_Naga::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_Naga::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	mComModel->SetUp_AnimIndex(0);


	return S_OK;
}


HRESULT CGameObject_Naga::Init_Unit()
{
	__super::Init_Unit();

	return S_OK;
}

HRESULT CGameObject_Naga::Init_AI()
{
	return E_NOTIMPL;
}


CGameObject_Naga * CGameObject_Naga::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Naga*	pInstance = NEW CGameObject_Naga(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Naga");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Naga* CGameObject_Naga::Clone(void* pArg)
{
	CGameObject_Naga*	pInstance = NEW CGameObject_Naga(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Naga");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Naga::Free()
{
	__super::Free();


	//	Safe_Release(mComNaviMesh);
}
