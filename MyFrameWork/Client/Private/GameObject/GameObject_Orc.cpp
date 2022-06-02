#include "stdafx.h"
#include "GameObject/GameObject_Orc.h"

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
	mComModel->SetUp_AnimIndex(0);


	return S_OK;
}


HRESULT CGameObject_Orc::Init_Unit()
{
	__super::Init_Unit();

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

	Safe_Release(mComModel);

	for (auto& obj : *mComListCollider)
	{
		Safe_Release(obj);
	}
	mComListCollider->clear();
	Safe_Delete(mComListCollider);


	//	Safe_Release(mComNaviMesh);
}
