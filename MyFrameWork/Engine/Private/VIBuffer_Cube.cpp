#include "..\Public\VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube & rhs)
	: CVIBuffer(rhs)

{
}

HRESULT CVIBuffer_Cube::NativeConstruct_Prototype()
{
	// 버텍스 버퍼 초기화
#pragma region VERTEX_BUFFER
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumIndicesPerPrimitive = 3;
	m_iNumVertices = 8;
	m_iNumVertexBuffers = 1;

	// 버텍스 전체 크기 / 사용방식 / 버텍스 크기를 넣어준다.
	m_VBDesc.ByteWidth = sizeof(VTXCUBETEX) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;				// 정적 사용
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// 버텍스버퍼로 사용한다고 알려줘야한다.
	m_VBDesc.StructureByteStride = sizeof(VTXCUBETEX);

	// 이제 정점 세팅을 LOCK UNLOCK을 하지 않고 해준다.
	VTXCUBETEX*		pVertices = NEW VTXCUBETEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXCUBETEX) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexUV = pVertices[0].vPosition;

	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexUV = pVertices[1].vPosition;

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexUV = pVertices[2].vPosition;

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexUV = pVertices[3].vPosition;

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexUV = pVertices[4].vPosition;

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexUV = pVertices[5].vPosition;

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexUV = pVertices[6].vPosition;

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexUV = pVertices[7].vPosition;

	// 버퍼리소스 D3D11_SUBRESOURCE_DATA 데이터 텍스처 인덱스 정수등의 버퍼들은 리소스내에 실제 데이터들이다.
	// pVertices들의 버퍼를 서브 리소스의 포인터에 넣어주고 CreateData에서
	// 버퍼의 타입과 실제 데이터로 버텍스 버퍼를 생성한다.
	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	// m_pVB에 버텍스 버퍼 생성
	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	// 들어가는 인수는 DX9과 비슷한다.
	m_iNumPrimitive = 12;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	// m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(FACEINDICES16) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// 직접 Indices에 세팅
	FACEINDICES16*	pIndices = NEW FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	/* +x */
	pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
	pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

	/* -x */
	pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
	pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

	/* +y */
	pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
	pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

	/* -y */
	pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
	pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

	/* +z */
	pIndices[8]._0 = 5; pIndices[8]._1 = 4; pIndices[8]._2 = 7;
	pIndices[9]._0 = 5; pIndices[9]._1 = 7; pIndices[9]._2 = 6;

	/* -z */
	pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
	pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;

	// DX11에서는 버퍼정보를 모두 서브 데이터 넣어 세팅해준다.
	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;
	mIndeces = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	// m_pIB에 인덱스 버퍼 세팅
//	Safe_Delete_Array(pIndices);
#pragma endregion
	return S_OK;
}

HRESULT CVIBuffer_Cube::NativeConstruct(void * pArg)
{
	return S_OK;
}
CVIBuffer_Cube * CVIBuffer_Cube::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CVIBuffer_Cube*	pInstance = NEW CVIBuffer_Cube(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CVIBuffer_Cube");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Cube::Clone(void * pArg)
{
	CVIBuffer_Cube*	pInstance = NEW CVIBuffer_Cube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_Cube");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}