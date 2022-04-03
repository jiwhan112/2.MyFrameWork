#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_Textures(rhs.m_Textures)
{
	for (auto& pTex : m_Textures)
	{
		Safe_AddRef(pTex);
	}
}

HRESULT CTexture::SetUp_OnShader(CShader * pShader, const char * pValueName, _uint iTextureIndex)
{
	if (nullptr == pShader ||
		iTextureIndex >= m_Textures.size())
		return E_FAIL;

	return pShader->Set_Texture(pValueName, m_Textures[iTextureIndex]);
}

HRESULT CTexture::NativeConstruct_Prototype(const _tchar* pTexturePath, _uint iNumTextures)
{
	// 스레드가 여러개 실행될 떄 잘못 초기화될 경우를 방지.
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// 텍스처 로드
	// 1. 텍스처는 확장자에 따라 다르게 로드한다.
	_tchar		szTextureFilePath[MAX_PATH] = TEXT("");
	_tchar		szExt[MAX_PATH] = TEXT("");

	m_Textures.reserve(iNumTextures);

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		wsprintf(szTextureFilePath, pTexturePath, i);

		ID3D11ShaderResourceView*		pSRV = nullptr;

		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		if (!lstrcmp(szExt, TEXT(".dds")))
		{
			// DTK 지원함수
			if (FAILED(CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV)))
				return E_FAIL;
		}
		// tga 지원 X
		else if (!lstrcmp(szExt, TEXT(".tga")))
			return E_FAIL;

		else
		{
			// DTK 지원함수
			if (FAILED(CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV)))
				return E_FAIL;
		}
		// 셰이더는 ShaderResourceView 형태로 데이터를 받는다.
		m_Textures.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::NativeConstruct(void * pArg)
{
	return S_OK;
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar* pTexturePath, _uint iNumTextures)
{
	CTexture*	pInstance = DBG_NEW CTexture(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pTexturePath, iNumTextures)))
	{
		MSGBOX("Failed to Created CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture*	pInstance = DBG_NEW CTexture(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture::Free()
{
	__super::Free();
	for (auto& pTex : m_Textures)
		Safe_Release(pTex);

	m_Textures.clear();
}