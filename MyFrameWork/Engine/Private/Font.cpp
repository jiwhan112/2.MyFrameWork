#include "Font.h"

CFont::CFont(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:mDevice(pDevice), mDeviceContext(pDeviceContext)
{
	Safe_AddRef(mDevice);
	Safe_AddRef(mDeviceContext);
}

HRESULT CFont::NativeConstruct(const _tchar * pFontFilePath)
{
	// 폰트 파일 생성
	mBatch = new SpriteBatch(mDeviceContext);
	if (nullptr == mBatch)
		return E_FAIL;

	mFont = new SpriteFont(mDevice, pFontFilePath);
	if (nullptr == mFont)
		return E_FAIL;

	return S_OK;
}

HRESULT CFont::Render(const _tchar * pText, _float2 vPosition, _fvector vColor)
{
	// 랜더링시 위치와 색 정보를 넣는다.
	mBatch->Begin();

	mFont->DrawString(mBatch, pText, vPosition, vColor);

	mBatch->End();
	
	return S_OK;
}

CFont * CFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontFilePath)
{
	CFont*	pInstance = NEW CFont(pDevice,pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pFontFilePath)))
	{
		MSGBOX("Failed to Created CFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFont::Free()
{
	Safe_Release(mDevice);
	Safe_Release(mDeviceContext);
}