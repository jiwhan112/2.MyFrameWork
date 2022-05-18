#pragma once

#include "Base.h"

BEGIN(Engine)

// ���� Ÿ�� �Ŵ���
class CRenderTargetMgr final : public CBase
{
private:
	CRenderTargetMgr();
	virtual ~CRenderTargetMgr() = default;

public:
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pRenderTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pRenderTargetTag);

private: // ���� Ÿ�� ��
	map<const _tchar*, class CRenderTarget*>			mMapRenderTarget;
	typedef map<const _tchar*, class CRenderTarget*>	RENDERTARGETS;

private: // �������� ����Ÿ�� ������ ���� // �ʿ� ����Ÿ�� ����Ʈ�� ����
	map<const _tchar*, list<class CRenderTarget*>>		mMapMRT;
	typedef map<const _tchar*, class CRenderTarget*>	MRTs;

public:
	// ���� Ÿ��
	class CRenderTarget*		Find_RenderTarget(const _tchar* pRenderTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};

END