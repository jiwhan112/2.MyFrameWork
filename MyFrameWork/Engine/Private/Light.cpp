#include "..\Public\Light.h"

CLight::CLight(ID3D11Device * device, ID3D11DeviceContext * devicecontext)
{
	mpDevice = device;
	mpDeviceContext = devicecontext;

	Safe_AddRef(mpDevice);
	Safe_AddRef(mpDeviceContext);
}

HRESULT CLight::NativeConstruct(const LIGHTDESC & LightDesc)
{
	memcpy(&mLightDesc, &LightDesc, sizeof(LIGHTDESC));
	return S_OK;
}

CLight * CLight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & LightDesc)
{
	CLight*	pInstance = NEW CLight(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(LightDesc)))
	{
		MSGBOX("Failed to Created CLight");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLight::Free()
{
	Safe_Release(mpDevice);
	Safe_Release(mpDeviceContext);
}