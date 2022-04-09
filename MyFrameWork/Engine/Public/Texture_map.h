#pragma once

#include "Component.h"

BEGIN(Engine)

// 맵으로 텍스처 관리 컴포넌트
class ENGINE_DLL CTexture_map final : public CComponent
{
public:
	CTexture_map(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTexture_map(const CTexture_map& rhs);
	virtual ~CTexture_map() = default;

public:
	// 텍스처를 셰이더 파일에 전달
	HRESULT SetUp_OnShader(class CShader * pShader, const char * pValueName);
	HRESULT Set_TextureMap(string texturename);

public:
	virtual HRESULT NativeConstruct_Prototype(list<MYFILEPATH*> listpath);	
	virtual HRESULT NativeConstruct(void* pArg);

public:
	

private:
	ID3D11ShaderResourceView* Find_MapTexture(string key);

private:
	// 현재 바인딩된 텍스처
	string	mCurrentKey = "";
	ID3D11ShaderResourceView* mCurrentTexture = nullptr;


private:
	/*
	shader-resource-view 인터페이스는
	렌더링 중에 셰이더가 액세스할 수 있는 하위 리소스를 지정합니다.
	셰이더 리소스의 예로는 상수 버퍼, 텍스처 버퍼 및 텍스처가 있습니다.
	*/

	// 텍스처 맵
	map<string,ID3D11ShaderResourceView*>				mMapTextures;
	typedef map<string, ID3D11ShaderResourceView*>		TEXTURES_MAP;

public:
	static CTexture_map* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, list<MYFILEPATH*> listpath);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END