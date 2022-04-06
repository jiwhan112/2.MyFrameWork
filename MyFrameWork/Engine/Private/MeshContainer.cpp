#include "..\Public\MeshContainer.h"

CMeshContainer::CMeshContainer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CMeshContainer::CMeshContainer(const CMeshContainer & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CMeshContainer::NativeConstruct_Prototype(CModel::E_MODEL_TYPE eMeshtype, aiMesh * pAIMesh,_fmatrix defaultmatrix)
{

#pragma region VERTICES
	// 모델의 aiMesh 정보로 모델의 정점을 생성한다.
	m_iNumIndicesPerPrimitive = 3;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 1;

	HRESULT			hr = 0;

	// 모델의 애니메이션 정보로 생성하는 정점이 다르다.
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	if (CModel::E_MODEL_TYPE::MODEL_NOANI == eMeshtype)
		hr = Ready_NonAnimMeshContainer(pAIMesh, defaultmatrix);
	else
		hr = Ready_AnimMeshContainer(pAIMesh);

	FAILED_CHECK(hr);
#pragma endregion


#pragma region INDICES
	// 인덱스 생성은 크게 다르지 않음
	m_iNumPrimitive = pAIMesh->mNumFaces;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(FACEINDICES32) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	FACEINDICES32* pIndices = NEW FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i]._0 = pAIMesh->mFaces[i].mIndices[0];
		pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[1];
		pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	FAILED_CHECK(Create_IndexBuffer());

	Safe_Delete_Array(pIndices);
#pragma endregion
	return S_OK;

}

HRESULT CMeshContainer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CMeshContainer::Ready_NonAnimMeshContainer(aiMesh * pAIMesh,_fmatrix defaultmatrix)
{
	m_VBDesc.ByteWidth = sizeof(VTXMODEL) * m_iNumVertices;
	m_VBDesc.StructureByteStride = sizeof(VTXMODEL);
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	VTXMODEL*	pVertices = NEW VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMODEL) * m_iNumVertices);

	// 버텍스의 위치 노말 UV 탄젠트를 저장한다.
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		// 초기 위치 곱 / 저장
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, 
			XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), defaultmatrix));
		
		// 초기 노말 곱 / 저장
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, 
			XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), defaultmatrix));

	//	memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));

		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vTangent,
			XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), defaultmatrix));


		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	FAILED_CHECK(Create_VertexBuffer());

	Safe_Delete_Array(pVertices);
	return S_OK;
}

HRESULT CMeshContainer::Ready_AnimMeshContainer(aiMesh * pAIMesh)
{
	return S_OK;
}


CMeshContainer * CMeshContainer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, CModel::E_MODEL_TYPE type, aiMesh * mesh, _fmatrix defaultmatrix)
{
	CMeshContainer*	pInstance = NEW CMeshContainer(pDevice,pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(type,mesh, defaultmatrix)))
	{
		MSGBOX("Failed to Creating CMeshContainer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMeshContainer::Clone(void * pArg)
{
	CMeshContainer*	pInstance = NEW CMeshContainer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CMeshContainer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshContainer::Free()
{
	__super::Free();
}
