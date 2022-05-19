#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderTarget final : public CBase
{
private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CRenderTarget() = default;

public:
	HRESULT NativeConstruct(_uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor);


private:
	ID3D11Device*			mDevice = nullptr;
	ID3D11DeviceContext*	mDeviceContext = nullptr;
	

	// 2D ���� Ÿ��
	ID3D11Texture2D*				mTexture2D = nullptr;

	// ����Ÿ�� ó�� Ŭ����
	ID3D11RenderTargetView*			mRTV = nullptr;
	// ���ҽ��� �ѱ� �� �ִ� Ŭ����
	ID3D11ShaderResourceView*		mSRV = nullptr;

private:
	_float4					m_vClearColor;

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor);
	virtual void Free() override;
};

END