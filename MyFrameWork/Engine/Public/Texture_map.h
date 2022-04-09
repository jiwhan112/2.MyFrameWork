#pragma once

#include "Component.h"

BEGIN(Engine)

// ������ �ؽ�ó ���� ������Ʈ
class ENGINE_DLL CTexture_map final : public CComponent
{
public:
	CTexture_map(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTexture_map(const CTexture_map& rhs);
	virtual ~CTexture_map() = default;

public:
	// �ؽ�ó�� ���̴� ���Ͽ� ����
	HRESULT SetUp_OnShader(class CShader * pShader, const char * pValueName);
	HRESULT Set_TextureMap(string texturename);

public:
	virtual HRESULT NativeConstruct_Prototype(list<MYFILEPATH*> listpath);	
	virtual HRESULT NativeConstruct(void* pArg);

public:
	

private:
	ID3D11ShaderResourceView* Find_MapTexture(string key);

private:
	// ���� ���ε��� �ؽ�ó
	string	mCurrentKey = "";
	ID3D11ShaderResourceView* mCurrentTexture = nullptr;


private:
	/*
	shader-resource-view �������̽���
	������ �߿� ���̴��� �׼����� �� �ִ� ���� ���ҽ��� �����մϴ�.
	���̴� ���ҽ��� ���δ� ��� ����, �ؽ�ó ���� �� �ؽ�ó�� �ֽ��ϴ�.
	*/

	// �ؽ�ó ��
	map<string,ID3D11ShaderResourceView*>				mMapTextures;
	typedef map<string, ID3D11ShaderResourceView*>		TEXTURES_MAP;

public:
	static CTexture_map* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, list<MYFILEPATH*> listpath);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END