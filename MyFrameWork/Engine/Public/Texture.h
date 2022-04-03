#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	// 텍스처를 셰이더 파일에 전달
	HRESULT SetUp_OnShader(class CShader* pShader, const char* pValueName, _uint iTextureIndex = 0);

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pTexturePath, _uint iNumTextures);
	virtual HRESULT NativeConstruct(void* pArg);

private:
	/*
	shader-resource-view 인터페이스는
	렌더링 중에 셰이더가 액세스할 수 있는 하위 리소스를 지정합니다.
	셰이더 리소스의 예로는 상수 버퍼, 텍스처 버퍼 및 텍스처가 있습니다.
	*/
	vector<ID3D11ShaderResourceView*>				m_Textures;
	typedef vector<ID3D11ShaderResourceView*>		TEXTURES;

public:
	static CTexture* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar* pTexturePath, _uint iNumTextures);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END