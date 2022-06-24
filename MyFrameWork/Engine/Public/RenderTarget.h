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
	HRESULT Clear();

	ID3D11RenderTargetView*			Get_RTV() { return mRTV; };
	ID3D11ShaderResourceView*		Get_SRV() { return mSRV; };
	
#ifdef _DEBUG
public: // ������
	HRESULT Ready_DebugDesc(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_DebugBuffer(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG


private:
	ID3D11Device*			mDevice = nullptr;
	ID3D11DeviceContext*	mDeviceContext = nullptr;

	// 2D ���� Ÿ��
	ID3D11Texture2D*				mTexture2D = nullptr;
	// ����Ÿ�� ó�� Ŭ����
	ID3D11RenderTargetView*			mRTV = nullptr;
	// ���ҽ��� �ѱ� �� �ִ� Ŭ����
	ID3D11ShaderResourceView*		mSRV = nullptr;
	// �ʱ�ȭ ��
	_float4							mClearColor = _float4();

#ifdef _DEBUG
private:
	// �ؽ�ó �����
	_float4x4				m_WorldMatrix;
	_float4x4				m_ViewMatrix;
	_float4x4				m_ProjMatrix;
#endif // _DEBUG


public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor);
	virtual void Free() override;
};

END