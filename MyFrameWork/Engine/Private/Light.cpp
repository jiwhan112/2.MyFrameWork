#include "..\Public\Light.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

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

// 넘겨주는 것만 함
HRESULT CLight::Render(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	if (nullptr == pShader)
		return E_FAIL;

	_uint		iPassIndex = 0;

	if (LIGHTDESC::TYPE_DIRECTIONAL == mLightDesc.eLightType)
	{
		pShader->Set_RawValue("g_vLightDir", &mLightDesc.vDirection, sizeof(_float4));
		
		// 임의의 위치
		mLightDesc.vPosition = -_float4(mLightDesc.vDirection) * 500;
		mLightDesc.vPosition.w = 1.0f;
		pShader->Set_RawValue("g_vLightPos", &mLightDesc.vPosition, sizeof(_float4));

		iPassIndex = 1;
	}

	else
	{
		pShader->Set_RawValue("g_vLightPos", &mLightDesc.vPosition, sizeof(_float4));
		pShader->Set_RawValue("g_fLightRange", &mLightDesc.fRange, sizeof(_float));
		iPassIndex = 2;
	}

	pShader->Set_RawValue("g_vLightDiffuse", &mLightDesc.vDiffuse, sizeof(_float4));
	pShader->Set_RawValue("g_vLightAmbient", &mLightDesc.vAmbient, sizeof(_float4));
	pShader->Set_RawValue("g_vLightSpecular", &mLightDesc.vSpecular, sizeof(_float4));

	return pVIBuffer->Render(pShader, iPassIndex);

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