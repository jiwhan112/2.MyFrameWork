#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
public:
	// 셰이더의 Pass + 레이아웃객체를 생성시에 DESC 형태로 받는다.
	typedef struct tagPassDesc
	{
		ID3DX11EffectPass*			pPass;
		//메모리에 배치된 꼭짓점 데이터를 그래픽 파이프라인의 입력 어셈블러 단계로 전달.
		ID3D11InputLayout*			pInputLayout;
	} PASSDESC;

public:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT NativeConstruct(void* pArg);
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);

	HRESULT Apply(_uint iPassIndex);
	HRESULT Set_InputLayout(_uint iPassIndex);
	HRESULT Set_RawValue(const char* pValueName, const void* pData, _uint iLength);
	HRESULT Set_Texture(const char* pValueName, ID3D11ShaderResourceView* pShaderResourceView);


private:
	ID3DX11Effect*				m_pEffect = nullptr;
	vector<PASSDESC*>			m_Passes;

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,
		const _tchar* pShaderFilePath, 
		const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END