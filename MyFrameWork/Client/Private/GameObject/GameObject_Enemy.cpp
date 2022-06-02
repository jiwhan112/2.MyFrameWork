#include "stdafx.h"
#include "GameObject/GameObject_Enemy.h"

CGameObject_Enemy::CGameObject_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_ORC;
}

CGameObject_Enemy::CGameObject_Enemy(const CGameObject_Enemy& rhs)
	: CGameObject_3D_Dynamic(rhs)
{

}

HRESULT CGameObject_Enemy::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_Enemy::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	mComModel->SetUp_AnimIndex(0);


	return S_OK;
}


HRESULT CGameObject_Enemy::Init_Unit()
{
	__super::Init_Unit();

	return S_OK;
}

HRESULT CGameObject_Enemy::Init_AI()
{
	return E_NOTIMPL;
}


CGameObject_Enemy * CGameObject_Enemy::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Enemy*	pInstance = NEW CGameObject_Enemy(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Enemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Enemy* CGameObject_Enemy::Clone(void* pArg)
{
	CGameObject_Enemy*	pInstance = NEW CGameObject_Enemy(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Enemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Enemy::Free()
{
	__super::Free();
}
