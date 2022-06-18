#pragma once

#include "Base.h"

BEGIN(Engine)

// 랜더 타깃 매니저
class CRenderTargetMgr final : public CBase
{
	DECLARE_SINGLETON(CRenderTargetMgr)

private:
	CRenderTargetMgr();
	virtual ~CRenderTargetMgr() = default;

public:
//	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pRenderTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pRenderTargetTag);

	HRESULT Begin(ID3D11DeviceContext* pDeviceContext, const _tchar* pMRTTag);
	HRESULT End(ID3D11DeviceContext* pDeviceContext, const _tchar* pMRTTag);

#ifdef _DEBUG
public:
	// 텍스처를 회면에 그려준다. / 화면 좌표와 사이즈
	HRESULT Ready_DebugDesc(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_DebugBuffer(const _tchar* pMRTTag,class CVIBuffer_Rect* vibuffer, class CShader* shader);
#endif // _DEBUG

public: 
	ID3D11ShaderResourceView* Get_SRV(const _tchar* pTargetTag) const;

private: // 랜더 타겟 저장
	map<const _tchar*, class CRenderTarget*>			mMapRenderTarget;
	typedef map<const _tchar*, class CRenderTarget*>	RENDERTARGETS;

private: // 한번에 여러개의 랜더타깃 세팅을 위함 // 맵에 랜더타깃 리스트를 저장
	map<const _tchar*, list<class CRenderTarget*>>		mMapMRT;
	typedef map<const _tchar*, class CRenderTarget*>	MRTs;

private:
	ID3D11RenderTargetView*			m_pOldRTV = nullptr;
	ID3D11DepthStencilView*			m_pOriginalDSV = nullptr;

#ifdef _DEBUG
private:
	// 텍스처 랜더용 사각형 버퍼
//	class CVIBuffer_Rect*					mComVIbuffer_Rect = nullptr;
//	class CShader*							mComShader= nullptr;
#endif // _DEBUG

public:
	// 랜더 타깃
	class CRenderTarget*		Find_RenderTarget(const _tchar* pRenderTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};

END