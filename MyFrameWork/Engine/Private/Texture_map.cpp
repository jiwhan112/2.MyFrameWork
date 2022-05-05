#include "..\Public\Texture_map.h"
#include "Shader.h"

CTexture_map::CTexture_map(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CTexture_map::CTexture_map(const CTexture_map & rhs)
	: CComponent(rhs)
	, mMapTextures(rhs.mMapTextures)
	, mCurrentKey(rhs.mCurrentKey)
	, mListKey(rhs.mListKey)
{
	for (auto& maptex : mMapTextures)
	{
		Safe_AddRef(maptex.second);
	}

	mCurrentKey = "";
	mCurrentTexture = nullptr;
}

HRESULT CTexture_map::SetUp_OnShader(CShader * pShader, const char * pValueName)
{
	if (mCurrentTexture == nullptr)
		return E_FAIL;

	return pShader->Set_Texture(pValueName, mCurrentTexture);
}

HRESULT CTexture_map::Set_TextureMap(string texturename)
{
	mCurrentKey = texturename;
	mCurrentTexture = Find_MapTexture(mCurrentKey);

	if (mCurrentTexture == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CTexture_map::NativeConstruct_Prototype(list<MYFILEPATH*> listpath)
{
	// 스레드가 여러개 실행될 떄 잘못 초기화될 경우를 방지.
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	mListKey = NEW list<string>;

	if (listpath.empty())
		return E_FAIL;

	// 텍스처 로드
	// 1. 텍스처는 확장자에 따라 다르게 로드한다.

	for (auto& filepath : listpath)
	{
		// 파일이름을 맵의 키로 저장

		_tchar		szTextureFilePath[MAX_PATH] = TEXT("");
		_tchar		szExt[MAX_PATH] = TEXT("");

		ID3D11ShaderResourceView*		pSRV = nullptr;
		lstrcpy(szTextureFilePath, filepath->FullPath);
		lstrcpy(szExt, filepath->Extension);

		string		keyname;
		wstring w = filepath->FileName;
		keyname.assign(w.begin(), w.end());
		if (Find_MapTexture(keyname) != nullptr)
			continue;

		if (!lstrcmp(szExt, TEXT("dds")))
		{
			// DTK 지원함수
			if (FAILED(CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV)))
				return E_FAIL;
		}
		// tga 지원 X
		else if (!lstrcmp(szExt, TEXT("tga")))
			return E_FAIL;

		else
		{
			// DTK 지원함수
			if (FAILED(CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV)))
				return E_FAIL;
		}
		mMapTextures.emplace(keyname, pSRV);

		mListKey->push_front(keyname);
	}

	return S_OK;
}

HRESULT CTexture_map::NativeConstruct(void * pArg)
{
	return S_OK;
}

ID3D11ShaderResourceView * CTexture_map::Find_MapTexture(string key)
{
	if (mMapTextures.empty())
		return nullptr;

	auto iter = mMapTextures.find(key.c_str());
	if (iter == mMapTextures.end())
		return nullptr;
	return iter->second;
}

CTexture_map * CTexture_map::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, list<MYFILEPATH*> listpath)
{
	CTexture_map*	pInstance = NEW CTexture_map(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(listpath)))
	{
		MSGBOX("Failed to Created CTexture_map");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture_map::Clone(void * pArg)
{
	CTexture_map*	pInstance = NEW CTexture_map(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CTexture_map");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture_map::Free()
{
	__super::Free();

	for (auto& maptex : mMapTextures)
	{
		Safe_Release(maptex.second);
	}
	mMapTextures.clear();

	if (m_isCloned == false)
		Safe_Delete(mListKey);
}