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
	// �ؽ�ó�� ���̴� ���Ͽ� ����
	HRESULT SetUp_OnShader(class CShader* pShader, const char* pValueName, _uint iTextureIndex = 0);

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pTexturePath, _uint iNumTextures);
	virtual HRESULT NativeConstruct(void* pArg);

private:
	/*
	shader-resource-view �������̽���
	������ �߿� ���̴��� �׼����� �� �ִ� ���� ���ҽ��� �����մϴ�.
	���̴� ���ҽ��� ���δ� ��� ����, �ؽ�ó ���� �� �ؽ�ó�� �ֽ��ϴ�.
	*/
	vector<ID3D11ShaderResourceView*>				m_Textures;
	typedef vector<ID3D11ShaderResourceView*>		TEXTURES;

public:
	static CTexture* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar* pTexturePath, _uint iNumTextures);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END