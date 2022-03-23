#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
public:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, void* pDeclaration);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT Render();

private:
	ID3DX11Effect*				m_pEffect = nullptr;
	vector<ID3DX11EffectPass*>	m_Passes;

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath, void* pDeclaration);

	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END