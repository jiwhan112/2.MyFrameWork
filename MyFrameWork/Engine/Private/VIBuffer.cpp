#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	// 버퍼 렌더링시 여러개의 버퍼를 한번에 그릴 수 있기 때문에 배열로 세팅된다.
	ID3D11Buffer*	pVertexBuffers[] = {
		m_pVB
	};

	_uint			iStrides[] = {
		m_VBDesc.StructureByteStride
	};

	_uint			iOffsets[] = {
		0, 
	};


	// 버텍스 인덱스 Topology 타입을 context에 바인딩해서 사용한다.
	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_eTopology);

//  셰이더를 만약 여기서 빌드 한다면 Layout 객체가 필요하다.
//	m_pDevice->CreateInputLayout(내가 셰이더에 전달하고자하는 정점의 구성정보, 갯수, 셰이더에서 받고자하는 정점의 구성정보, 길이);
//	m_pDeviceContext->IASetInputLayout(pInputLayout);

	m_pDeviceContext->DrawIndexed(m_iNumIndicesPerPrimitive * m_iNumPrimitive, 0, 0);

	return S_OK;

}


HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	// DESC의 정보로 DESC를 설정한다.
	return m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubResourceData, &m_pVB);	
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	// DESC의 정보로 DESC를 설정한다.
	return m_pDevice->CreateBuffer(&m_IBDesc, &m_IBSubResourceData,&m_pIB);
}

void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
