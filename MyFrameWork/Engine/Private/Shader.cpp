#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_Passes(rhs.m_Passes)
{
	Safe_AddRef(m_pEffect);
}

HRESULT CShader::NativeConstruct(void * pArg)
{

	return S_OK;
}

HRESULT CShader::NativeConstruct_Prototype(const _tchar * pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	// 셰이더 디버깅 옵션
	iFlag = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#else
	// 셰이더 최적화 옵션
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	// 버퍼 생성
	// ID3DBlob은 버퍼와 동일하다. 할당될 빈공간을 의미
	ID3DBlob*			pShaderBinaryCodes = nullptr;
	ID3DBlob*			pErrorMessage = nullptr;

	// 1. 셰이더파일이 들어오면 먼저 컴파일을 수행한다.
	// pShaderBinaryCodes : 셰이더파일정보 pErrorMessage: 에러문자정보
	if (FAILED(D3DCompileFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, nullptr, "fx_5_0", iFlag, 0, &pShaderBinaryCodes, &pErrorMessage)))
		return E_FAIL;

	// 2. 컴파일된 메모리로 셰이더 m_pEffect에 생성.
	if (FAILED(D3DX11CreateEffectFromMemory(pShaderBinaryCodes->GetBufferPointer(), pShaderBinaryCodes->GetBufferSize(), 0, m_pDevice, &m_pEffect)))
		return E_FAIL;

	// 3. 테크 포인터에서 pass 정보를 가져온다.
	ID3DX11EffectTechnique*			pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	// 테크 포인터에 접근해서 DESC정보를 가져와 조작한다.
	pTechnique->GetDesc(&TechniqueDesc);

	// 4. pass의 개수에따라 셰이더 pass Vecotr에 넣어준다.
	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		PASSDESC*		pPassDesc = DBG_NEW PASSDESC;
		ZeroMemory(pPassDesc, sizeof(PASSDESC));

		pPassDesc->pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == pPassDesc->pPass)
			return E_FAIL;

		D3DX11_PASS_DESC		PassDesc;
		ZeroMemory(&PassDesc, sizeof(D3DX11_PASS_DESC));

		pPassDesc->pPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pPassDesc->pInputLayout)))
			return E_FAIL;

		m_Passes.push_back(pPassDesc);
	}
	// 텍스처 세팅하는 것과 비슷
	return S_OK;
}



HRESULT CShader::Apply(_uint iPassIndex)
{
	if (iPassIndex >= m_Passes.size())
		return E_FAIL;
	if (m_Passes.empty())
		return E_FAIL;

	return m_Passes[iPassIndex]->pPass->Apply(0, m_pDeviceContext);

}

HRESULT CShader::Set_InputLayout(_uint iPassIndex)
{
	if (iPassIndex >= m_Passes.size())
		return E_FAIL;
	if (m_Passes.empty())
		return E_FAIL;

	// layout 바인딩은 context에서 수행한다.
	m_pDeviceContext->IASetInputLayout(m_Passes[iPassIndex]->pInputLayout);
	return S_OK;
}

HRESULT CShader::Set_RawValue(const char * pValueName, void * pData, _uint iLength)
{
	// 셰이더의 변수 이름을 ID3DX11EffectVariable* 형으로 받는다. 
	ID3DX11EffectVariable*		pValue = m_pEffect->GetVariableByName(pValueName);
	if (nullptr == pValue)
		return E_FAIL;

	// 유효한 값이면 세팅
	return pValue->SetRawValue(pData, 0, iLength);
}

HRESULT CShader::Set_Texture(const char * pValueName, ID3D11ShaderResourceView * pShaderResourceView)
{
	// 해당 값이 유효한지 확인
	ID3DX11EffectVariable*		pValue = m_pEffect->GetVariableByName(pValueName);
	if (nullptr == pValue)
		return E_FAIL;

	// 해당 리소스가 텍스처를 받는 리소스가 맞는지 확인
	ID3DX11EffectShaderResourceVariable* pValueTex = pValue->AsShaderResource();
	if (nullptr == pValueTex)
		return E_FAIL;

	// 텍스처 ID3D11ShaderResourceView 타입으로 세팅
	return pValueTex->SetResource(pShaderResourceView);
}

CShader * CShader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext,
	const _tchar * pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC * pElements, _uint iNumElements)
{
	CShader*	pInstance = DBG_NEW CShader(pDevice,pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pShaderFilePath,pElements,iNumElements)))
	{
		MSGBOX("Failed to Creating CShader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CShader::Clone(void * pArg)
{
	CShader*	pInstance = DBG_NEW CShader(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	// pInputLayout 객체가 버그로 AddRef가 안된다. 따라서 클론일때는 삭제하지 않는다.
	if (false == m_isCloned)
	{
		for (auto& pPassDesc : m_Passes)
		{
			Safe_Release(pPassDesc->pPass);
			Safe_Release(pPassDesc->pInputLayout);

			Safe_Delete(pPassDesc);
		}

		m_Passes.clear();
	}

	Safe_Release(m_pEffect);
}
