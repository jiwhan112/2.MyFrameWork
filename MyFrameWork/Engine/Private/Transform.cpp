#include "..\Public\Transform.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
{
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if (pArg != nullptr)
		memcpy( &mDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

HRESULT CTransform::GO_Straight(_double deltatime)
{
	return S_OK;
}

HRESULT CTransform::GO_Left(_double deltatime)
{
	return S_OK;
}

HRESULT CTransform::GO_Right(_double deltatime)
{
	return S_OK;
}

HRESULT CTransform::GO_Backward(_double deltatime)
{
	return S_OK;
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTransform*	pInstance = new CTransform(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
