#include "FontMgr.h"
#include "Font.h"

IMPLEMENT_SINGLETON(CFontMgr)

HRESULT CFontMgr::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	CFont* newFont = Find_Font(pFontTag);
	if (newFont != nullptr)
		return E_FAIL;

	newFont = CFont::Create(pDevice, pDeviceContext, pFontFilePath);
	mMapFonts.emplace(pFontTag,newFont);
	return S_OK;
}

HRESULT CFontMgr::Render_Font(const _tchar * pFontTag, const _tchar * pText, _float2 vPosition, _fvector vColor)
{
	CFont*		pFont = Find_Font(pFontTag);
	if (nullptr == pFont)
		return E_FAIL;

	return pFont->Render(pText, vPosition, vColor);
	return S_OK;
}

CFont * CFontMgr::Find_Font(const _tchar * pTag)
{

	auto iter = find_if(mMapFonts.begin(), mMapFonts.end(), CTagFinder(pTag));
	if (iter != mMapFonts.end())
		return iter->second;
	return nullptr;
}

void CFontMgr::Free()
{
	for (auto pair : mMapFonts)
	{
		Safe_Release(pair.second);
	}
}
