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
	// ���̴� ����� �ɼ�
	iFlag = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#else
	// ���̴� ����ȭ �ɼ�
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	// ���� ����
	// ID3DBlob�� ���ۿ� �����ϴ�. �Ҵ�� ������� �ǹ�
	ID3DBlob*			pShaderBinaryCodes = nullptr;
	ID3DBlob*			pErrorMessage = nullptr;

	// 1. ���̴������� ������ ���� �������� �����Ѵ�.
	// pShaderBinaryCodes : ���̴��������� pErrorMessage: ������������
	if (FAILED(D3DCompileFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, nullptr, "fx_5_0", iFlag, 0, &pShaderBinaryCodes, &pErrorMessage)))
		return E_FAIL;

	// 2. �����ϵ� �޸𸮷� ���̴� m_pEffect�� ����.
	if (FAILED(D3DX11CreateEffectFromMemory(pShaderBinaryCodes->GetBufferPointer(), pShaderBinaryCodes->GetBufferSize(), 0, m_pDevice, &m_pEffect)))
		return E_FAIL;

	// 3. ��ũ �����Ϳ��� pass ������ �����´�.
	ID3DX11EffectTechnique*			pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	// ��ũ �����Ϳ� �����ؼ� DESC������ ������ �����Ѵ�.
	pTechnique->GetDesc(&TechniqueDesc);

	// 4. pass�� ���������� ���̴� pass Vecotr�� �־��ش�.
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
	// �ؽ�ó �����ϴ� �Ͱ� ���
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

	// layout ���ε��� context���� �����Ѵ�.
	m_pDeviceContext->IASetInputLayout(m_Passes[iPassIndex]->pInputLayout);
	return S_OK;
}

HRESULT CShader::Set_RawValue(const char * pValueName, void * pData, _uint iLength)
{
	// ���̴��� ���� �̸��� ID3DX11EffectVariable* ������ �޴´�. 
	ID3DX11EffectVariable*		pValue = m_pEffect->GetVariableByName(pValueName);
	if (nullptr == pValue)
		return E_FAIL;

	// ��ȿ�� ���̸� ����
	return pValue->SetRawValue(pData, 0, iLength);
}

HRESULT CShader::Set_Texture(const char * pValueName, ID3D11ShaderResourceView * pShaderResourceView)
{
	// �ش� ���� ��ȿ���� Ȯ��
	ID3DX11EffectVariable*		pValue = m_pEffect->GetVariableByName(pValueName);
	if (nullptr == pValue)
		return E_FAIL;

	// �ش� ���ҽ��� �ؽ�ó�� �޴� ���ҽ��� �´��� Ȯ��
	ID3DX11EffectShaderResourceVariable* pValueTex = pValue->AsShaderResource();
	if (nullptr == pValueTex)
		return E_FAIL;

	// �ؽ�ó ID3D11ShaderResourceView Ÿ������ ����
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

	// pInputLayout ��ü�� ���׷� AddRef�� �ȵȴ�. ���� Ŭ���϶��� �������� �ʴ´�.
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
