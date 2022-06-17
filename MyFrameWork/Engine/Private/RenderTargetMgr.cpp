#include "RenderTargetMgr.h"
#include "RenderTarget.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

IMPLEMENT_SINGLETON(CRenderTargetMgr)


CRenderTargetMgr::CRenderTargetMgr()
{
}

HRESULT CRenderTargetMgr::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	// �ؽ�ó�� ���̴� ���
	mComVIbuffer_Rect = CVIBuffer_Rect::Create(pDevice, pDeviceContext);
	if (nullptr == mComVIbuffer_Rect)
		return E_FAIL;

	mComShader = CShader::Create(pDevice, pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == mComShader)
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderTargetMgr::Add_RenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pRenderTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	if (nullptr != Find_RenderTarget(pRenderTargetTag))
		return E_FAIL;

	CRenderTarget*		pRenderTarget = CRenderTarget::Create(pDevice, pDeviceContext, iSizeX, iSizeY, eFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	mMapRenderTarget.emplace(pRenderTargetTag, pRenderTarget);

	return S_OK;
}


HRESULT CRenderTargetMgr::Add_MRT(const _tchar * pMRTTag, const _tchar * pRenderTargetTag)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;


	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);

		mMapMRT.emplace(pMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);	

	return S_OK;
}

HRESULT CRenderTargetMgr::Begin(ID3D11DeviceContext * pDeviceContext, const _tchar * pMRTTag)
{
	// �ش� ��Ƽ ���� Ÿ������ ����

	list<CRenderTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	// 1. ���� ����� ����.
	pDeviceContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOriginalDSV);

	// 2. �ش� ���� Ÿ���� ����.
	_uint iNumViews = 0;

	ID3D11RenderTargetView*		pRenderTargets[8] = { nullptr };

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		pRenderTargets[iNumViews++] = pRenderTarget->Get_RTV();
	}
	pDeviceContext->OMSetRenderTargets(iNumViews, pRenderTargets, m_pOriginalDSV);

	// ���� ���� ����
	// ����Ÿ���� �ؽ�ó�� �׸� �� ī�޶� ȭ�� ������ ��ŭ �׷����� ������ ���� ���۸� ���� �׷����Ѵ�.

	return S_OK;
}

HRESULT CRenderTargetMgr::End(ID3D11DeviceContext * pDeviceContext, const _tchar * pMRTTag)
{
	_uint		iNumViews = 1;

	// ���� �� ���� �ٽ� ����
	pDeviceContext->OMSetRenderTargets(iNumViews, &m_pOldRTV, m_pOriginalDSV);

	Safe_Release(m_pOldRTV);
	Safe_Release(m_pOriginalDSV);

	return S_OK;
}

#if _DEBUG // _DEBUG
HRESULT CRenderTargetMgr::Ready_DebugDesc(const _tchar * pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_DebugDesc(fX, fY, fSizeX, fSizeY);
}

HRESULT CRenderTargetMgr::Render_DebugBuffer(const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render_DebugBuffer(mComShader, mComVIbuffer_Rect);
	}

	return S_OK;
}

#endif // _DEBUG
ID3D11ShaderResourceView * CRenderTargetMgr::Get_SRV(const _tchar * pTargetTag) const
{
	auto	iter = find_if(mMapRenderTarget.begin(), mMapRenderTarget.end(), CTagFinder(pTargetTag));

	if (iter == mMapRenderTarget.end())
		return nullptr;

	return iter->second->Get_SRV();
}

CRenderTarget * CRenderTargetMgr::Find_RenderTarget(const _tchar * pRenderTargetTag)
{
	auto	iter = find_if(mMapRenderTarget.begin(), mMapRenderTarget.end(), CTagFinder(pRenderTargetTag));
	if (iter == mMapRenderTarget.end())
		return nullptr;

	return iter->second;
}

list<CRenderTarget*>* CRenderTargetMgr::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(mMapMRT.begin(), mMapMRT.end(), CTagFinder(pMRTTag));

	if (iter == mMapMRT.end())
		return nullptr;

	return &iter->second;
}

void CRenderTargetMgr::Free()
{
	Safe_Release(mComVIbuffer_Rect);
	Safe_Release(mComShader);

	// ��Ƽ ����Ÿ���� ���� ����
	for (auto& Pair : mMapMRT)
	{
		for (auto& pRenderTarget : Pair.second)
		{
			Safe_Release(pRenderTarget);
		}
		Pair.second.clear();
	}
	mMapMRT.clear();

	// �� ���� ���� Ÿ�� ����
	for (auto& Pair : mMapRenderTarget)
		Safe_Release(Pair.second);
	mMapRenderTarget.clear();

}
