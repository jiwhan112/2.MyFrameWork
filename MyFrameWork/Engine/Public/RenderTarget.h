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
	

	// 2D 랜더 타깃
	ID3D11Texture2D*				mTexture2D = nullptr;

	// 랜더타깃 처리 클래스
	ID3D11RenderTargetView*			mRTV = nullptr;
	// 리소스로 넘길 수 있는 클래스
	ID3D11ShaderResourceView*		mSRV = nullptr;

private:
	_float4					m_vClearColor;

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor);
	virtual void Free() override;
};

END