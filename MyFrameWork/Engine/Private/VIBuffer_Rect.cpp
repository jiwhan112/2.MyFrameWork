#include "..\Public\VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect::NativeConstruct_Prototype()
{
	// ���ؽ� ���� �ʱ�ȭ
#pragma region VERTEX_BUFFER
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumIndicesPerPrimitive = 3;
	m_iNumVertices = 4;
	m_iNumVertexBuffers = 1;

	// ���ؽ� ��ü ũ�� / ����� / ���ؽ� ũ�⸦ �־��ش�.
	m_VBDesc.ByteWidth = sizeof(VTXTEX) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;				// ���� ���
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// ���ؽ����۷� ����Ѵٰ� �˷�����Ѵ�.
	m_VBDesc.StructureByteStride = sizeof(VTXTEX);

	// ���� ���� ������ LOCK UNLOCK�� ���� �ʰ� ���ش�.
	VTXTEX*		pVertices = NEW VTXTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXTEX) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.f, 1.f);

	// ���۸��ҽ� D3D11_SUBRESOURCE_DATA ������ �ؽ�ó �ε��� �������� ���۵��� ���ҽ����� ���� �����͵��̴�.
	// pVertices���� ���۸� ���� ���ҽ��� �����Ϳ� �־��ְ� CreateData����
	// ������ Ÿ�԰� ���� �����ͷ� ���ؽ� ���۸� �����Ѵ�.
	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	// m_pVB�� ���ؽ� ���� ����
	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	// ���� �μ��� DX9�� ����Ѵ�.
	m_iNumPrimitive = 2;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	// m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(FACEINDICES16) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// ���� Indices�� ����
	FACEINDICES16*	pIndices = NEW FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	// DX11������ ���������� ��� ���� ������ �־� �������ش�.
	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	mIndeces = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	// m_pIB�� �ε��� ���� ����
//	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Rect::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CVIBuffer_Rect*	pInstance = NEW CVIBuffer_Rect(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CVIBuffer_Rect*	pInstance = NEW CVIBuffer_Rect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
}