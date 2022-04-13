#include "..\Public\MeshContainer.h"
#include "HierarchyNode.h"

CMeshContainer::CMeshContainer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CMeshContainer::CMeshContainer(const CMeshContainer & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CMeshContainer::NativeConstruct_Prototype(aiMesh * pAIMesh )
{

	mpAiMesh = pAIMesh;
	mIndexMaterial = pAIMesh->mMaterialIndex;
	mNumBones = pAIMesh->mNumBones;
	return S_OK;

}

HRESULT CMeshContainer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CMeshContainer::Ready_VertexIndexBuffer(CModel::E_MODEL_TYPE eMeshtype, _fmatrix defaultmatrix)
{
	// 메쉬 버텍스 생성

#pragma region VERTICES
	// 모델의 aiMesh 정보로 모델의 정점을 생성한다.
	m_iNumIndicesPerPrimitive = 3;
	m_iNumVertices = mpAiMesh->mNumVertices;
	m_iNumVertexBuffers = 1;
	HRESULT			hr = 0;

	// 모델의 애니메이션 정보로 생성하는 정점이 다르다.
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	if (CModel::E_MODEL_TYPE::MODEL_NOANI == eMeshtype)
		hr = Ready_NonAnimMeshContainer(mpAiMesh, defaultmatrix);
	else
		hr = Ready_AnimMeshContainer(mpAiMesh);

	FAILED_CHECK(hr);
#pragma endregion


#pragma region INDICES
	// 인덱스 생성은 크게 다르지 않음
	m_iNumPrimitive = mpAiMesh->mNumFaces;
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
		pIndices[i]._0 = mpAiMesh->mFaces[i].mIndices[0];
		pIndices[i]._1 = mpAiMesh->mFaces[i].mIndices[1];
		pIndices[i]._2 = mpAiMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	FAILED_CHECK(Create_IndexBuffer());

	Safe_Delete_Array(pIndices);
#pragma endregion
	return S_OK;
}

HRESULT CMeshContainer::Add_HierarchyNode(CHierarchyNode * pNode)
{
	mHierarchyNodes.push_back(pNode);
	Safe_AddRef(pNode);

	return S_OK;
}

HRESULT CMeshContainer::SetUp_Matrices(_float4x4 * pBoneMatrices)
{
	// MeshContainer에 연결된 계층 노드의 정보를 통해 뼈의 최종 위치를 만든다.
	// Render에서 만들어진 최종 뼈위치를 셰이더에 넘긴다.
	_uint index = 0;
	for (auto& hierNode: mHierarchyNodes)
	{
		_matrix offsetMat = hierNode->Get_OffsetMat();
		_matrix CombinedMat = hierNode->Get_CombinedTransformMat();

		_matrix BoneMat = offsetMat * CombinedMat;

		XMStoreFloat4x4(&pBoneMatrices[index++],XMMatrixTranspose(BoneMat));
	}

	return S_OK;
}

HRESULT CMeshContainer::Ready_NonAnimMeshContainer(aiMesh * pAIMesh,_fmatrix defaultmatrix)
{
	// 애니메이션이 없는 메쉬 초기화
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
	// 애니메이션이 있는 모델 초기화

	m_VBDesc.ByteWidth = sizeof(VTXANIMODEL) * m_iNumVertices;
	m_VBDesc.StructureByteStride = sizeof(VTXANIMODEL);
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	VTXANIMODEL*	pVertices = new VTXANIMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	// 추가된 Blend 값은 여기서 초기화한다.
	FAILED_CHECK(Ready_SkinnedInfo(pVertices));

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	FAILED_CHECK(Create_VertexBuffer());


	Safe_Delete_Array(pVertices);

	return S_OK;
}


CMeshContainer * CMeshContainer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, aiMesh * mesh)
{
	CMeshContainer*	pInstance = NEW CMeshContainer(pDevice,pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(mesh)))
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


HRESULT CMeshContainer::Ready_SkinnedInfo(VTXANIMODEL* pVertices)
{
	// 정점의 BlendIndex / BlendWeight 초기화 
	if (mpAiMesh == nullptr)
		return E_FAIL;

	mNumBones = mpAiMesh->mNumBones;
	for (_uint i=0; i<mNumBones;++i)
	{
		aiBone* pAiBone = mpAiMesh->mBones[i];

		for (_uint j = 0; j < pAiBone->mNumWeights; ++j)
		{
			// 각 뼈에 영향을 주는 뼈는 최대 4개 설정할 수 있다.
			// 각 뼈의 영향을 주는 뼈는 index.x / weight.x 에 세트로 세팅된다.
			// 해당 뼈에 영향을주는 뼈가 이미 있다면 i와 weight는 y에 세팅된다.

			if (0.0f == pVertices[pAiBone->mWeights[j].mVertexId].vBlendWeight.x)
			{
				pVertices[pAiBone->mWeights[j].mVertexId].vBlendIndex.x = i;
				pVertices[pAiBone->mWeights[j].mVertexId].vBlendWeight.x = pAiBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAiBone->mWeights[j].mVertexId].vBlendWeight.y)
			{
				pVertices[pAiBone->mWeights[j].mVertexId].vBlendIndex.y = i;
				pVertices[pAiBone->mWeights[j].mVertexId].vBlendWeight.y = pAiBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAiBone->mWeights[j].mVertexId].vBlendWeight.z)
			{
				pVertices[pAiBone->mWeights[j].mVertexId].vBlendIndex.z = i;
				pVertices[pAiBone->mWeights[j].mVertexId].vBlendWeight.z = pAiBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAiBone->mWeights[j].mVertexId].vBlendWeight.w)
			{
				pVertices[pAiBone->mWeights[j].mVertexId].vBlendIndex.w = i;
				pVertices[pAiBone->mWeights[j].mVertexId].vBlendWeight.w = pAiBone->mWeights[j].mWeight;
			}
		}
	}
	return S_OK;
}



void CMeshContainer::Free()
{
	__super::Free();
	for (auto& pHier: mHierarchyNodes)
	{
		Safe_Release(pHier);
	}
	mHierarchyNodes.clear();
}

