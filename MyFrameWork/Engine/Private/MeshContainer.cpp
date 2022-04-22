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

HRESULT CMeshContainer::NativeConstruct_Prototype(aiMesh * pAIMesh)
{
	m_pAIMesh = pAIMesh;
	strcpy_s(m_szName, pAIMesh->mName.data);
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumBones = pAIMesh->mNumBones;

	return S_OK;
}

HRESULT CMeshContainer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CMeshContainer::Ready_VertexIndexBuffer(CModel::E_MODEL_TYPE eMeshtype, _fmatrix TransformMatrix)
{
#pragma region VERTICES
	m_iNumIndicesPerPrimitive = 3;
	m_iNumVertices = m_pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 1;


	HRESULT			hr = 0;

	if (CModel::MODEL_NOANI == eMeshtype)
		hr = Ready_NonAnimMeshContainer(m_pAIMesh, TransformMatrix);
	else
		hr = Ready_AnimMeshContainer(m_pAIMesh);

#pragma endregion


#pragma region INDICES
	m_iNumPrimitive = m_pAIMesh->mNumFaces;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(FACEINDICES32) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i]._0 = m_pAIMesh->mFaces[i].mIndices[0];
		pIndices[i]._1 = m_pAIMesh->mFaces[i].mIndices[1];
		pIndices[i]._2 = m_pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;
	mIndeces = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

//	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CMeshContainer::Add_HierarchyNode(_uint iNodeIndex)
{
	m_HierarchyNodeIndices.push_back(iNodeIndex);

	return S_OK;
}

HRESULT CMeshContainer::SetUp_Matrices(_float4x4 * pBoneMatrices, const vector<CHierarchyNode*>* pNodes, _fmatrix TransformMatrix)
{
	_uint	iIndex = 0;

	if(0 == m_iNumBones)
	{

		auto	iter = find_if(pNodes->begin(), pNodes->end(), [&](CHierarchyNode* pHierarchyNode) 
		{
			return !strcmp(m_szName, pHierarchyNode->Get_Name());			
		});

		if(iter == pNodes->end())
			XMStoreFloat4x4(&pBoneMatrices[0], XMMatrixIdentity());

		else
		{
			_matrix		OffsetMatrix = (*iter)->Get_OffsetMatrix();
			_matrix		CombinedTransformationMatrix = (*iter)->Get_CombinedTransformationMatrix();

			_matrix		BoneMatrix = OffsetMatrix * CombinedTransformationMatrix * TransformMatrix;

			XMStoreFloat4x4(&pBoneMatrices[iIndex++], XMMatrixTranspose(BoneMatrix));
		}

	}

	for (auto& iNodeIndex : m_HierarchyNodeIndices)
	{
		
		_matrix		OffsetMatrix = (*pNodes)[iNodeIndex]->Get_OffsetMatrix();
		_matrix		CombinedTransformationMatrix = (*pNodes)[iNodeIndex]->Get_CombinedTransformationMatrix();

		_matrix		BoneMatrix = OffsetMatrix * CombinedTransformationMatrix * TransformMatrix;

		XMStoreFloat4x4(&pBoneMatrices[iIndex++], XMMatrixTranspose(BoneMatrix));
	}

	return S_OK;
}

HRESULT CMeshContainer::Ready_NonAnimMeshContainer(aiMesh* pAIMesh, _fmatrix TransformMatrix)
{
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBDesc.ByteWidth = sizeof(VTXMODEL) * m_iNumVertices;
	m_VBDesc.StructureByteStride = sizeof(VTXMODEL);	
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	VTXMODEL*	pVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), TransformMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), TransformMatrix));

		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Ready_AnimMeshContainer(aiMesh* pAIMesh)
{
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

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

	if (FAILED(Ready_SkinnedInfo(pVertices)))
		return E_FAIL;

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);


	return S_OK;
}

HRESULT CMeshContainer::Ready_SkinnedInfo(VTXANIMODEL* pVertices)
{
	if (nullptr == m_pAIMesh)
		return E_FAIL;

	m_iNumBones = m_pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone*			pAIBone = m_pAIMesh->mBones[i];		

		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			/* 정점은 최대 네개뼈대의 영향ㅇ르 받는다. */
			if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.x = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.y = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.z = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.w = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
			}
		}		
	}	

	return S_OK;
}

CMeshContainer * CMeshContainer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, aiMesh * pAIMesh)
{
	CMeshContainer*	pInstance = new CMeshContainer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pAIMesh)))
	{
		MSGBOX("Failed to Created CMeshContainer");
		Safe_Release(pInstance);
	}
	return pInstance;
}
CComponent * CMeshContainer::Clone(void * pArg)
{
	CMeshContainer*	pInstance = new CMeshContainer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMeshContainer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshContainer::Free()
{
	__super::Free();

	//for (auto& pHierarchyNode : m_HierarchyNodes)
	//	Safe_Release(pHierarchyNode);

	m_HierarchyNodeIndices.clear();

}
