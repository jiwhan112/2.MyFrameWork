#pragma once

#include "Base.h"



BEGIN(Engine)

// 폰트 매니저
class CFontMgr final : public CBase
{
	DECLARE_SINGLETON(CFontMgr)

private:
	CFontMgr() {}
	virtual ~CFontMgr() = default;

public:
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pText, _float2 vPosition, _fvector vColor);

private:
	map<const _tchar*, class CFont*>			mMapFonts;

private:
	class CFont* Find_Font(const _tchar* pTag);

public:
	virtual void Free() override;
};

END