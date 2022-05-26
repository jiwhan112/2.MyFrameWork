#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, miNumX(rhs.miNumX)
	, miNumZ(rhs.miNumZ)

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

	_ulong*		pPixel = NEW _ulong[miNumX * miNumZ];
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
	VTXNORTEX*		pVertices = NEW VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	mpVertexPos = NEW _float3[m_iNumVertices];
	ZeroMemory(mpVertexPos, sizeof(_float3)*m_iNumVertices);

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
	FACEINDICES32*	pIndices = NEW FACEINDICES32[m_iNumPrimitive];
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

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(_uint x, _uint z)
{
	// 기본 정적 버퍼 생성
//	INIT_Default_VIBuffer(x, z);

	// 동적 버퍼 생성
	INIT_New_VIBuffer(x, z);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	return S_OK;
}

_float4 CVIBuffer_Terrain::Get_Height(_float4 TargetPos)
{
	_float4 vPos = TargetPos;

	// 현재 위치 찾기 z * width + x
	_uint iIndex = (_uint)vPos.z * miNumX + (_uint)vPos.x;

	_uint Indeces[] = {
		iIndex + miNumX,
		iIndex + miNumX + 1,
		iIndex + 1 ,
		iIndex
	};

	_float fWidth = vPos.x - mpVertexPos[Indeces[0]].x;
	_float fDeoth = mpVertexPos[Indeces[0]].z - vPos.z;

	_float fY = 0.f;
	_plane plane;

	// Width가 더 크면 위쪽 삼각형
	if (fWidth > fDeoth)
	{
		// 점 3개로 평면을 만든다.
		plane = _plane(mpVertexPos[Indeces[0]], mpVertexPos[Indeces[1]], mpVertexPos[Indeces[2]]);
	}
	// 아래쪽
	else
	{
		plane = _plane(mpVertexPos[Indeces[0]], mpVertexPos[Indeces[2]], mpVertexPos[Indeces[3]]);
	}

	// y = (-ax - cz - d) / b;
	fY = ((-plane.x * vPos.x) - (plane.z * vPos.z) - plane.D()) / plane.y;

	vPos.y = fY;
	return vPos;
}

_float CVIBuffer_Terrain::Get_HeightY(_float3 TargetPos)
{
	// 현재 위치 찾기 z * width + x
	_float3 Target = TargetPos;

	_uint iIndex = (_uint)Target.z * miNumX + (_uint)Target.x;

	_uint Indeces[] = {
		iIndex + miNumX,
		iIndex + miNumX + 1,
		iIndex + 1 ,
		iIndex
	};

	_float fWidth = Target.x - mpVertexPos[Indeces[0]].x;
	_float fDeoth = mpVertexPos[Indeces[0]].z - Target.z;

	_float fY = 0.f;
	_plane plane;

	// Width가 더 크면 위쪽 삼각형
	if (fWidth > fDeoth)
	{
		// 점 3개로 평면을 만든다.
		plane = _plane(mpVertexPos[Indeces[0]], mpVertexPos[Indeces[1]], mpVertexPos[Indeces[2]]);
	}
	// 아래쪽
	else
	{
		plane = _plane(mpVertexPos[Indeces[0]], mpVertexPos[Indeces[2]], mpVertexPos[Indeces[3]]);
	}

	// y = (-ax - cz - d) / b;
	fY = ((-plane.x * Target.x) - (plane.z * Target.z) - plane.D()) / plane.y;

	return fY;
}


_uint CVIBuffer_Terrain::Get_TileIndex(_float3 worldPos)
{
	_float3 vPos = worldPos;

	// 현재 위치 찾기 z * width + x
	int z = (int)vPos.z * (int)miNumX;
	int x = (int)vPos.x;
	int iIndex = z + x;

	return iIndex;
}

_float3 CVIBuffer_Terrain::Get_TileWorldPos(_uint TileIndex)
{
	// 타일의 월드 위치 반환
	_uint Indeces[] = {
		TileIndex + miNumX,
		TileIndex + miNumX + 1,
		TileIndex + 1 ,
		TileIndex
	};

	_float fWidth = mpVertexPos[Indeces[1]].x - mpVertexPos[Indeces[0]].x;
	_float fDeoth = mpVertexPos[Indeces[0]].z - mpVertexPos[Indeces[3]].z;

	_float fCenterX = mpVertexPos[TileIndex].x + fWidth * 0.5f;
	_float fCenterZ = mpVertexPos[TileIndex].z + fDeoth * 0.5f;;

	return _float3(fCenterX, 0, fCenterZ);
}
_float3 CVIBuffer_Terrain::Get_TileWorldPos(_uint x,_uint z)
{
	if (x >= miNumX && z >= miNumZ)
		return _float3(-1, 0, -1);

	int iIndex = z * miNumX + x;
	// 타일의 월드 위치 반환
	_uint Indeces[] = {
		iIndex + miNumX,
		iIndex + miNumX + 1,
		iIndex + 1 ,
		iIndex
	};

	_float fWidth = mpVertexPos[Indeces[1]].x - mpVertexPos[Indeces[0]].x;
	_float fDeoth = mpVertexPos[Indeces[0]].z - mpVertexPos[Indeces[3]].z;

	_float fCenterX = mpVertexPos[iIndex].x + fWidth * 0.5f;
	_float fCenterZ = mpVertexPos[iIndex].z + fDeoth * 0.5f;;

	return _float3(fCenterX, 0, fCenterZ);
}

_int CVIBuffer_Terrain::Get_TilIndex(_uint x, _uint z)
{
	if (x >= miNumX && z >= miNumZ)
		return -1;
	return z * miNumX + x;
}

HRESULT CVIBuffer_Terrain::INIT_Default_VIBuffer(_uint x, _uint z)
{
#pragma region VERTEX_BUFFER

	miNumX = x;
	miNumZ = z;

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
	VTXNORTEX*		pVertices = NEW VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	mpVertexPos = NEW _float3[m_iNumVertices];
	ZeroMemory(mpVertexPos, sizeof(_float3));

	for (_uint z = 0; z < miNumZ; z++)
	{
		for (_uint x = 0; x < miNumX; x++)
		{
			_uint iIndex = z * miNumX + x;

			pVertices[iIndex].vPosition = mpVertexPos[iIndex] = _float3(x, 0.0f, z);
			pVertices[iIndex].vNormal = _float3(0.0f, 1, 0.0f); // 노말 구해 주기
			pVertices[iIndex].vTexUV = _float2(x / (miNumX - 1.f), z / (miNumZ - 1.f));
		}
	}

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;
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
	FACEINDICES32*	pIndices = NEW FACEINDICES32[m_iNumPrimitive];
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
	mIndeces = pIndices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	//	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain::INIT_New_VIBuffer(_uint x, _uint z)
{
	// 최대 버퍼 129X129에서 해당 부분만 그린다.
	miNumX = x;
	miNumZ = z;

#pragma region NEWVERTEX

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_iNumIndicesPerPrimitive = 3;
	m_iNumVertices = miNumX * miNumZ;
	m_iNumVertexBuffers = 1;

	// 버텍스 전체 크기 / 사용방식 / 버텍스 크기를 넣어준다.
	m_VBDesc.ByteWidth = sizeof(VTXNORTEX) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;				// 동적 사용
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// 버텍스버퍼로 사용한다고 알려줘야한다.
	m_VBDesc.StructureByteStride = sizeof(VTXNORTEX);
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// 이제 정점 세팅을 LOCK UNLOCK을 하지 않고 해준다.
	VTXNORTEX*		pVertices = NEW VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	mpVertexPos = NEW _float3[m_iNumVertices];
	ZeroMemory(mpVertexPos, sizeof(_float3));

	for (_uint z = 0; z < miNumZ; z++)
	{
		for (_uint x = 0; x < miNumX; x++)
		{
			_uint iIndex = z * miNumX + x;

			pVertices[iIndex].vPosition = mpVertexPos[iIndex] = _float3(x, 0.0f, z);
			pVertices[iIndex].vNormal = _float3(0.0f, 1, 0.0f); // 노말 구해 주기
			pVertices[iIndex].vTexUV = _float2(x / (miNumX - 1.f), z / (miNumZ - 1.f));
		}
	}

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;
#pragma endregion NEWVERTEX

#pragma region NEWINDEX

	// 인덱스 버퍼는 사각형에 삼각형이 두개씩 들어간다.
	m_iNumPrimitive = (miNumX - 1) * (miNumZ - 1) * 2;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(FACEINDICES32) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// 직접 Indices에 세팅
	FACEINDICES32*	pIndices = NEW FACEINDICES32[m_iNumPrimitive];
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
	mIndeces = pIndices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	//	Safe_Delete_Array(pIndices);
#pragma endregion NEWINDEX
	return S_OK;
}


HRESULT CVIBuffer_Terrain::Set_HeightMap(const _tchar* filepath)
{
	// 만들어진 버텍스 수정
	// 무조건 BMP 파일

	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	HANDLE		hFile = CreateFile(filepath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_ulong					dwByte = 0;
	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	miNumX = ih.biWidth;
	miNumZ = ih.biHeight;
	m_iNumVertices = miNumX * miNumZ;

	_ulong*		pPixel = NEW _ulong[m_iNumVertices];
	ZeroMemory(pPixel, sizeof(_ulong)*m_iNumVertices);

	ReadFile(hFile, pPixel, sizeof(_ulong) * m_iNumVertices, &dwByte, nullptr);

	//// 버텍스 수정하기

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);
	for (_uint z = 0; z < miNumZ; z++)
	{
		for (_uint x = 0; x < miNumX; x++)
		{
			_uint		iIndex = z * miNumX + x;
			_float		NewY = ((pPixel[iIndex] & 0x000000ff));
			NewY -= 64;

			if (NewY < 2 && NewY>2)
				NewY = 0;
			if (NewY != 0)
				NewY /= 50;

			_float3		newPos = _float3(x, NewY, z);

			((VTXNORTEX*)SubResource.pData)[iIndex].vPosition = mpVertexPos[iIndex] = newPos;
			((VTXNORTEX*)SubResource.pData)[iIndex].vNormal = _float3(0, 1, 0);
			((VTXNORTEX*)SubResource.pData)[iIndex].vTexUV = _float2(x / (miNumX - 1.f), z / (miNumZ - 1.f));

		}
	}
	
	// #TODO: 노말 새로 구하기

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

		//	((VTXNORTEX*)SubResource.pData)[iIndex].vNormal

			// 노말 채우기
			// 폴리곤의 두 벡터를 외적해서 노말벡터를 구한다.
			P[0] = XMLoadFloat3(&mpVertexPos[iIndices[1]]) - XMLoadFloat3(&mpVertexPos[iIndices[0]]);
			P[1] = XMLoadFloat3(&mpVertexPos[iIndices[2]]) - XMLoadFloat3(&mpVertexPos[iIndices[1]]);
			vNormal = XMVector3Normalize(XMVector3Cross(P[0], P[1]));

			// 구한 노말을 노말라이즈해서 더해주고 다시 노말을 해준다.
			XMStoreFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[0]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[0]].vNormal) + vNormal));
			XMStoreFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[1]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[1]].vNormal) + vNormal));
			XMStoreFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[2]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[2]].vNormal) + vNormal));

			// 노말 채우기
			P[0] = XMLoadFloat3(&mpVertexPos[iIndices[1]]) - XMLoadFloat3(&mpVertexPos[iIndices[0]]);
			P[1] = XMLoadFloat3(&mpVertexPos[iIndices[2]]) - XMLoadFloat3(&mpVertexPos[iIndices[1]]);
			vNormal = XMVector3Normalize(XMVector3Cross(P[0], P[1]));

			// 구한 노말을 노말라이즈해서 더해주고 다시 노말을 해준다.
			XMStoreFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[0]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[0]].vNormal) + vNormal));
			XMStoreFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[2]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[1]].vNormal) + vNormal));
			XMStoreFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[3]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)SubResource.pData)[iIndices[2]].vNormal) + vNormal));

		}
	}


	m_pDeviceContext->Unmap(m_pVB, 0);

	Safe_Delete_Array(pPixel);
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CVIBuffer_Terrain::UpdateY(_float y)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;
	
	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		_float3		vPos = ((VTXNORTEX*)SubResource.pData)[i].vPosition;

		if(i%5 == 0)
			vPos.y += y;

		((VTXNORTEX*)SubResource.pData)[i].vPosition = vPos;
	}
	m_pDeviceContext->Unmap(m_pVB, 0);

	return S_OK;
}


HRESULT CVIBuffer_Terrain::Set_NewXZ(const _uint X, const _uint Z)
{
	// #BUG 인덱스 버퍼 재설정 다시보기
	if (nullptr == m_pDeviceContext)
		return E_FAIL;
	if (mpVertexPos != nullptr)
	{
		Safe_Delete_Array(mpVertexPos);
		mpVertexPos = nullptr;
	}

	miNumX = X;
	miNumZ = Z;
	m_iNumVertices = miNumX * miNumZ;

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	mpVertexPos = NEW _float3[m_iNumVertices];
	ZeroMemory(mpVertexPos, sizeof(_float3));

	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	// 버텍스 수정하기
	for (_uint z = 0; z < miNumZ; z++)
	{
		for (_uint x = 0; x < miNumX; x++)
		{
			_uint iIndex = z * miNumX + x;
			((VTXNORTEX*)SubResource.pData)[iIndex].vPosition = mpVertexPos[iIndex] = _float3(x, 0.0f, z);
			((VTXNORTEX*)SubResource.pData)[iIndex].vNormal = _float3(0.0f, 1, 0.0f);
			((VTXNORTEX*)SubResource.pData)[iIndex].vTexUV = _float2(x / (miNumX - 1.f), z / (miNumZ - 1.f));
		}
	}

	m_pDeviceContext->Unmap(m_pVB, 0);

	// 인덱스 버퍼 재생성

#pragma region INDEX

	ZeroMemory(mIndeces, sizeof(FACEINDICES32) * m_iNumPrimitive);

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

			((FACEINDICES32*)mIndeces)[iNumFace]._0 = iIndices[0];
			((FACEINDICES32*)mIndeces)[iNumFace]._1 = iIndices[1];
			((FACEINDICES32*)mIndeces)[iNumFace]._2 = iIndices[2];
			++iNumFace;

			((FACEINDICES32*)mIndeces)[iNumFace]._0 = iIndices[0];
			((FACEINDICES32*)mIndeces)[iNumFace]._1 = iIndices[2];
			((FACEINDICES32*)mIndeces)[iNumFace]._2 = iIndices[3];
			++iNumFace;
		}
	}

#pragma endregion INDEX

	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * HeightMap)
{
	CVIBuffer_Terrain*	pInstance = NEW CVIBuffer_Terrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(HeightMap)))
	{
		MSGBOX("Failed to Created CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint x, _uint z)
{
	CVIBuffer_Terrain*	pInstance = NEW CVIBuffer_Terrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(x, z)))
	{
		MSGBOX("Failed to Created CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = NEW CVIBuffer_Terrain(*this);

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