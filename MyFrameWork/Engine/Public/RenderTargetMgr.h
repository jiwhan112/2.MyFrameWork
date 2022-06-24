#pragma once

#include "Base.h"

BEGIN(Engine)

// ���� Ÿ�� �Ŵ���
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
	// �ؽ�ó�� ȸ�鿡 �׷��ش�. / ȭ�� ��ǥ�� ������
	HRESULT Ready_DebugDesc(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_DebugBuffer(const _tchar* pMRTTag,class CVIBuffer_Rect* vibuffer, class CShader* shader);
#endif // _DEBUG

public: 
	ID3D11ShaderResourceView* Get_SRV(const _tchar* pTargetTag) const;

private: // ���� Ÿ�� ����
	map<const _tchar*, class CRenderTarget*>			mMapRenderTarget;
	typedef map<const _tchar*, class CRenderTarget*>	RENDERTARGETS;

private: // �ѹ��� �������� ����Ÿ�� ������ ���� // �ʿ� ����Ÿ�� ����Ʈ�� ����
	map<const _tchar*, list<class CRenderTarget*>>		mMapMRT;
	typedef map<const _tchar*, class CRenderTarget*>	MRTs;

private:
	ID3D11RenderTargetView*			m_pOldRTV = nullptr;
	ID3D11DepthStencilView*			m_pOriginalDSV = nullptr;

#ifdef _DEBUG
private:
	// �ؽ�ó ������ �簢�� ����
//	class CVIBuffer_Rect*					mComVIbuffer_Rect = nullptr;
//	class CShader*							mComShader= nullptr;
#endif // _DEBUG

public:
	// ���� Ÿ��
	class CRenderTarget*		Find_RenderTarget(const _tchar* pRenderTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};

END