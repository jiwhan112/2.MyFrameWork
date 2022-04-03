#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)

{
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar* heightmap)
{
	// 버텍스 버퍼 초기화
#pragma region VERTEX_BUFFER

	HANDLE		hFile = CreateFile(heightmap, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_ulong					dwByte = 0;
	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	miNumX = ih.biWidth;
	miNumZ = ih.biHeight;

	_ulong*		pPixel = new _ulong[miNumX * miNumZ];
	ReadFile(hFile, pPixel, sizeof(_ulong) * miNumX * miNumZ, &dwByte, nullptr);

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumIndicesPerPrimitive = 3;
	m_iNumVertices = miNumX * miNumZ;
	m_iNumVertexBuffers = 1;

	// 버텍스 전체 크기 / 사용방식 / 버텍스 크기를 넣어준다.
	m_VBDesc.ByteWidth = sizeof(VTXNORTEX) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;				// 정적 사용
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// 버텍스버퍼로 사용한다고 알려줘야한다.
	m_VBDesc.StructureByteStride = sizeof(VTXNORTEX);

	// 이제 정점 세팅을 LOCK UNLOCK을 하지 않고 해준다.
	VTXNORTEX*		pVertices = DBG_NEW VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	mpVertexPos = DBG_NEW _float3[m_iNumVertices];
	ZeroMemory(mpVertexPos, sizeof(_float3));

	for (_uint z = 0; z < miNumZ; z++)
	{
		for (_uint x = 0; x < miNumX; x++)
		{
			_uint iIndex = z * miNumX + x;

			pVertices[iIndex].vPosition = mpVertexPos[iIndex] = _float3(x, (pPixel[iIndex] & 0x000000ff) / 10.f, z);
			pVertices[iIndex].vNormal = _float3(0.0f, 0.0f, 0.0f); // 노말 구해 주기
			pVertices[iIndex].vTexUV = _float2(x / (miNumX - 1.f), z / (miNumZ - 1.f));
		}
	}

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	// m_pVB에 버텍스 버퍼 생성
	Safe_Delete_Array(pPixel);
	CloseHandle(hFile);

#pragma endregion

#pragma region INDEX_BUFFER
	// 인덱스 버퍼는 사각형에 삼각형이 두개씩 들어간다.
	m_iNumPrimitive = (miNumX - 1) * (miNumZ - 1) * 2;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(FACEINDICES32) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// 직접 Indices에 세팅
	FACEINDICES32*	pIndices = DBG_NEW FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_uint		iNumFace = 0;

	for (_uint z = 0; z < miNumZ - 1; z++)
	{
		for (_uint x = 0; x < miNumX - 1; x++)
		{
			_uint		iIndex = z * miNumX + x;

			_uint		iIndices[4] = {
				iIndex + miNumX,
				iIndex + miNumX + 1,
				iIndex + 1,
				iIndex
			};
			_vector P[2], vNormal;

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[1];
			pIndices[iNumFace]._2 = iIndices[2];

			// 노말 채우기
			// 폴리곤의 두 벡터를 외적해서 노말벡터를 구한다.
			P[0] = XMLoadFloat3(&mpVertexPos[pIndices[iNumFace]._1]) - XMLoadFloat3(&mpVertexPos[pIndices[iNumFace]._0]);
			P[1] = XMLoadFloat3(&mpVertexPos[pIndices[iNumFace]._2]) - XMLoadFloat3(&mpVertexPos[pIndices[iNumFace]._1]);
			vNormal = XMVector3Normalize(XMVector3Cross(P[0], P[1]));

			// 구한 노말을 노말라이즈해서 더해주고 다시 노말을 해준다.
			XMStoreFloat3(&pVertices[pIndices[iNumFace]._0].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._0].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._2].vNormal) + vNormal));

			++iNumFace;

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[2];
			pIndices[iNumFace]._2 = iIndices[3];

			// 노말 채우기
			P[0] = XMLoadFloat3(&mpVertexPos[pIndices[iNumFace]._1]) - XMLoadFloat3(&mpVertexPos[pIndices[iNumFace]._0]);
			P[1] = XMLoadFloat3(&mpVertexPos[pIndices[iNumFace]._2]) - XMLoadFloat3(&mpVertexPos[pIndices[iNumFace]._1]);
			vNormal = XMVector3Normalize(XMVector3Cross(P[0], P[1]));

			XMStoreFloat3(&pVertices[pIndices[iNumFace]._0].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._0].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._2].vNormal) + vNormal));

			++iNumFace;
		}
	}

	// DX11에서는 버퍼정보를 모두 서브 데이터 넣어 세팅해준다.
	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion
	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * HeightMap)
{
	CVIBuffer_Terrain*	pInstance = DBG_NEW CVIBuffer_Terrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(HeightMap)))
	{
		MSGBOX("Failed to Created CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = DBG_NEW CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}