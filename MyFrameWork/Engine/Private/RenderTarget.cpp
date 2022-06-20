#include "RenderTarget.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: mDevice(pDevice)
	, mDeviceContext(pDeviceContext)

{

}

HRESULT CRenderTarget::NativeConstruct(_uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	if (nullptr == mDevice)
		return E_FAIL;

	// �ʱ�ȭ ��
	mClearColor = vClearColor;

	// ���� Ÿ��� �ؽ�ó ����
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

	texDesc.Width = iSizeX;
	texDesc.Height = iSizeY;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = eFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT; // ���̴��� ���� ����
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // ���뵵
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// �ؽ�ó / ���ҽ�Ÿ�� / ���̴����ҽ��� ����
	FAILED_CHECK(mDevice->CreateTexture2D(&texDesc, nullptr, &mTexture2D));
	FAILED_CHECK(mDevice->CreateRenderTargetView(mTexture2D, nullptr, &mRTV));
	FAILED_CHECK(mDevice->CreateShaderResourceView(mTexture2D, nullptr, &mSRV));

	return S_OK;
}

HRESULT CRenderTarget::Clear()
{
	// ���� Ÿ�� �ʱ�ȭ
	if (nullptr == mDeviceContext)
		return E_FAIL;

	mDeviceContext->ClearRenderTargetView(mRTV, (_float*)&mClearColor);
	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderTarget::Ready_DebugDesc(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	// ���̴��� �ѱ� ��Ʈ���� ����
	_uint		iNumViewports = 1;

	D3D11_VIEWPORT		Viewport;

	mDeviceContext->RSGetViewports(&iNumViewports, &Viewport);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	// ���� ����
	m_WorldMatrix._11 = fSizeX;
	m_WorldMatrix._22 = fSizeY;
	m_WorldMatrix._33 = 1.f;
	memcpy(&m_WorldMatrix.m[3][0], &_float4(fX - (Viewport.Width * 0.5f), -fY + (Viewport.Height * 0.5f), 0.f, 1.f), sizeof(_float4));

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));
	return S_OK;
}

HRESULT CRenderTarget::Render_DebugBuffer(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	// ���õ� ������ ���̴��� �Ѱܼ� �׷���
	pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	// ���̴� Ÿ�� ������ �ѱ�
	// ������ �ؽ�ó ���� Deferred ���� ����Ѵ�.
	pShader->Set_Texture("g_TargetTexture", mSRV );

	pVIBuffer->Render(pShader, 0);
	return S_OK;
}
#endif _DEBUG

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
