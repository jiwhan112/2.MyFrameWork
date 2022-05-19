#include "RenderTarget.h"

CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: mDevice(pDevice)
	, mDeviceContext(pDeviceContext)

{

}

HRESULT CRenderTarget::NativeConstruct(_uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	if (nullptr == mDevice)
		return E_FAIL;

	// 랜더 타깃용 텍스처 선언
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

	texDesc.Width = iSizeX;
	texDesc.Height = iSizeY;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = eFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT; // 셰이더에 의한 접근
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // 사용용도
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// 텍스처 / 리소스타겟 / 셰이더리소스뷰 생성
	FAILED_CHECK(mDevice->CreateTexture2D(&texDesc, nullptr, &mTexture2D));
	FAILED_CHECK(mDevice->CreateRenderTargetView(mTexture2D, nullptr, &mRTV));
	FAILED_CHECK(mDevice->CreateShaderResourceView(mTexture2D, nullptr, &mSRV));

	return S_OK;
}

CRenderTarget * CRenderTarget::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	CRenderTarget*	pInstance = NEW CRenderTarget(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(iSizeX,iSizeY,eFormat,vClearColor)))
	{
		MSGBOX("Failed to Created CRenderTarget");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRenderTarget::Free()
{
	Safe_Release(mTexture2D);
	Safe_Release(mRTV);
	Safe_Release(mSRV);

}
