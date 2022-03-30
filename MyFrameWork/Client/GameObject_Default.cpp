#include "stdafx.h"
#include "GameObject_Default.h"

CGameObject_Default::CGameObject_Default(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CGameObject_Default::CGameObject_Default(const CGameObject_Default& rhs)
	: CGameObject(rhs)
{

}

HRESULT CGameObject_Default::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_Default::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	return S_OK;
}

_int CGameObject_Default::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));
	return _int();
}

_int CGameObject_Default::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
}

HRESULT CGameObject_Default::Render()
{
	FAILED_CHECK(__super::Render());
	return S_OK;
}


CGameObject_Default * CGameObject_Default::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Default*	pInstance = new CGameObject_Default(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Default");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Default* CGameObject_Default::Clone(void* pArg)
{
	CGameObject_Default*	pInstance = new CGameObject_Default(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Default");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Default::Free()
{
	__super::Free();

}
