#include "RenderTargetMgr.h"
#include "RenderTarget.h"

CRenderTargetMgr::CRenderTargetMgr()
{
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
	// 멀티 랜더타깃을 먼저 지움
	for (auto& Pair : mMapMRT)
	{
		for (auto& pRenderTarget : Pair.second)
		{
			Safe_Release(pRenderTarget);
		}
		Pair.second.clear();
	}
	mMapMRT.clear();

	// 그 다음 랜더 타깃 삭제
	for (auto& Pair : mMapRenderTarget)
		Safe_Release(Pair.second);
	mMapRenderTarget.clear();

}
