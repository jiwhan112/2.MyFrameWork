#include "stdafx.h"
#include "GameObject/GameObject_3D_Floor.h"

CGameObject_3D_Floor::CGameObject_3D_Floor(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject_3D_Floor(pDevice,pDeviceContext)
{

}

CGameObject_3D_Floor::CGameObject_3D_Floor(const CGameObject_3D_Floor & rhs)
	:CGameObject_3D_Static(rhs)
{
}

HRESULT CGameObject_3D_Floor::Set_Component()
{
	return S_OK;
}

HRESULT CGameObject_3D_Floor::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject_3D_Floor::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CGameObject_3D_Floor::Tick(_double TimeDelta)
{
	return UPDATENONE;
}

HRESULT CGameObject_3D_Floor::Render()
{
	return S_OK;
}

CGameObject_3D_Floor * CGameObject_3D_Floor::Create(ID3D11Device * d, ID3D11DeviceContext * c)
{
	CGameObject_3D_Floor*	pInstance = NEW CGameObject_3D_Floor(d, c);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Floor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject_3D_Floor * CGameObject_3D_Floor::Clone(void * pArg)
{
	CGameObject_3D_Floor*	pInstance = NEW CGameObject_3D_Floor(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Floor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D_Floor::Free()
{
	__super::Free();

}
