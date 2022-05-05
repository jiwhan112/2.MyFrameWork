#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "HierarchyNode.h"
#include "Animation.h"
#include "Channel.h"
#include "Shader.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_pScene(rhs.m_pScene)
	, m_pMeshContainers(rhs.m_pMeshContainers)
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
	, m_eModelType(rhs.m_eModelType)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Materials(rhs.m_Materials)
	, m_iCurrentAnim(rhs.m_iCurrentAnim)
	, m_TransformMatrix(rhs.m_TransformMatrix)
{
	for (auto& pMaterial : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(pMaterial->pTexture[i]);
	}

	if (nullptr != m_pMeshContainers)
	{
		for (_uint i = 0; i < m_iNumMaterials; ++i)
		{
			for (auto& pMeshContainer : m_pMeshContainers[i])
				Safe_AddRef(pMeshContainer);
		}
	}
}

_uint CModel::Get_NumMaterials() const
{
	return m_iNumMaterials;
}

HRESULT CModel::Set_AniString(string AniName)
{
	_uint index = Find_Animation(AniName);
	SetUp_AnimIndex(index);
	return S_OK;
}

_uint CModel::Find_Animation(string AniName) const
{
	_uint returnIndex = 0;

	for (_uint i = 0; i < m_iNumAnimations; i++)
	{
		string name = m_Animations[i]->Get_Name();
		if (AniName.compare(name) == 0)
		{
			returnIndex = i;
			break;
		}
	}

	return returnIndex;
}

HRESULT CModel::NativeConstruct_Prototype(E_MODEL_TYPE eModelType, const char * pModelFilePath, const char * pModelFileName, _fmatrix TransformMatrix)
{
	char	szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	_uint		iFlag = 0;

	/* 칼, 그림자 */

	if (MODEL_NOANI == eModelType)
		iFlag = aiProcess_PreTransformVertices | /*aiProcess_GlobalScale | */aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;

	m_pScene = m_Importer.ReadFile(szFullPath, iFlag);

	if (nullptr == m_pScene)
		return E_FAIL;

	m_eModelType = eModelType;
	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix);

	/* 모델의 머테리얼 정보를 셋ㅌ이한다. */
	/* 머테리얼? 디퓨즈, 스펙큘러, 노멀, 이미시즈 */
	/* 텍스쳐에 담아서 로드할것이다. */
	/* 텍스쳐 로드한다. */
	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	if (MODEL_ANI == m_eModelType)
	{
		if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode, nullptr, 0)))
			return E_FAIL;

		sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
		{
			return pSour->Get_Depth() < pDest->Get_Depth();
		});
	}

	if (FAILED(Ready_MeshContainers()))
		return E_FAIL;

	//if (TYPE_NONANIM == m_eModelType)
	//{
	//	for (_uint i = 0; i < m_iNumMaterials; ++i)
	//	{
	//		for (auto& pMeshContainer : m_pMeshContainers[i])
	//			pMeshContainer->Ready_VertexIndexBuffer(m_eModelType, TransformMatrix);
	//	}
	//
	//	return S_OK;
	//}

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		for (auto& pMeshContainer : m_pMeshContainers[i])
			pMeshContainer->Ready_VertexIndexBuffer(m_eModelType, TransformMatrix);
	}

	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	if (FAILED(Ready_OffsetMatrices()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	if (FAILED(Link_ChannelToNode()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::SetUp_AnimIndex(_uint iAnimIndex)
{
	if (iAnimIndex >= m_iNumAnimations)
		return E_FAIL;

	m_iCurrentAnim = iAnimIndex;

	return S_OK;
}

HRESULT CModel::Update_CombinedTransformationMatrices(_double TimeDelta)
{
	/* 모든 뼈들의 혅2ㅐ 시간에 맞는 상태를 저장시킨다.(채널에)  */
	m_Animations[m_iCurrentAnim]->Update_TransformMatrices(TimeDelta);

	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		/* 부모로부터 자식까지 행렬상태를 누적시키며 셋팅해준다. */
		pHierarchyNode->Update_CombinedTransformationMatrix();
	}

	return S_OK;
}

HRESULT CModel::Bind_OnShader(CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char * pValueName)
{
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	CTexture*		pTexture = m_Materials[iMaterialIndex]->pTexture[eTextureType];
	if (nullptr == pTexture)
		return E_FAIL;

	return pTexture->SetUp_OnShader(pShader, pValueName);
}

HRESULT CModel::Render(CShader * pShader, _uint iPassIndex, _uint iMaterialIndex, const char* pBoneValueName)
{
	if (iMaterialIndex >= m_iNumMaterials ||
		nullptr == m_pMeshContainers)
		return E_FAIL;

	_float4x4		BoneMatrices[MAX_BONES];

	for (auto& pMeshContainer : m_pMeshContainers[iMaterialIndex])
	{
		if (MODEL_ANI == m_eModelType)
		{
			ZeroMemory(&BoneMatrices, sizeof(_float4x4) * MAX_BONES);

			pMeshContainer->SetUp_Matrices(BoneMatrices, &m_HierarchyNodes, m_TransformMatrix);

			pShader->Set_RawValue(pBoneValueName, BoneMatrices, sizeof(_float4x4) * MAX_BONES);
		}

		if (nullptr != pMeshContainer)
			pMeshContainer->Render(pShader, iPassIndex);
	}

	return S_OK;
}

HRESULT CModel::Ready_MeshContainers()
{
	if (nullptr == m_pScene)
		return E_FAIL;

	m_iNumMeshContainers = m_pScene->mNumMeshes;

	m_pMeshContainers = new MESHCONTAINERS[m_iNumMaterials];

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, m_pScene->mMeshes[i]);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		for (_uint j = 0; j < m_pScene->mMeshes[i]->mNumBones; ++j)
		{
			_uint		iIndex = 0;
			CHierarchyNode*		pHierarchyNode = Find_HierarchyNode(m_pScene->mMeshes[i]->mBones[j]->mName.data, &iIndex);

			if (nullptr == pHierarchyNode)
				return E_FAIL;

			pMeshContainer->Add_HierarchyNode(iIndex);
		}

		m_pMeshContainers[m_pScene->mMeshes[i]->mMaterialIndex].push_back(pMeshContainer);
	}
	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	if (nullptr == m_pScene)
		return E_FAIL;

	/* */
	m_iNumMaterials = m_pScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MESHMATERIALDESC*		pMeshMaterialDesc = new MESHMATERIALDESC;
		ZeroMemory(pMeshMaterialDesc, sizeof(MESHMATERIALDESC));

		for (_uint j = 1; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString	TexturePath;

			if (FAILED(m_pScene->mMaterials[i]->GetTexture(aiTextureType(j), 0, &TexturePath)))
				continue;

			char		szFullPath[MAX_PATH] = "";
			char		szFileName[MAX_PATH] = "";
			char		szExt[MAX_PATH] = "";

			_splitpath_s(TexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			string str = szFullPath;
			wstring wfullpath = CHelperClass::Convert_str2wstr(str);

			pMeshMaterialDesc->pTexture[j] = CTexture::Create(m_pDevice, m_pDeviceContext, wfullpath.c_str());
			if (nullptr == pMeshMaterialDesc->pTexture[j])
				return S_FALSE;
		}

		m_Materials.push_back(pMeshMaterialDesc);
	}

	return S_OK;
}

HRESULT CModel::Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth)
{
	if (nullptr == pNode)
		return E_FAIL;

	_float4x4		TransformationMatrix;
	memcpy(&TransformationMatrix, &pNode->mTransformation, sizeof(_float4x4));

	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pNode->mName.data, TransformationMatrix, iDepth, pParent);
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);

	for (_uint i = 0; i < pNode->mNumChildren; ++i)
	{
		Ready_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth + 1);
	}

	return S_OK;
}

HRESULT CModel::Ready_OffsetMatrices()
{
	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		for (auto& pMeshContainer : m_pMeshContainers[i])
		{
			_uint iNumBones = pMeshContainer->Get_NumBones();

			aiMesh*		pAIMesh = pMeshContainer->Get_AIMesh();

			for (_uint j = 0; j < iNumBones; ++j)
			{
				aiBone*		pBone = pAIMesh->mBones[j];

				CHierarchyNode*		pHierarchyNode = Find_HierarchyNode(pBone->mName.data);

				if (nullptr == pHierarchyNode)
					return E_FAIL;

				_float4x4		OffsetMatrix;
				memcpy(&OffsetMatrix, &pBone->mOffsetMatrix, sizeof(_float4x4));

				pHierarchyNode->Set_OffsetMatrix(OffsetMatrix);
			}
		}
	}

	return S_OK;
}

HRESULT CModel::Ready_Animation()
{
	if (nullptr == m_pScene)
		return E_FAIL;

	m_iNumAnimations = m_pScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation*	pAIAnimation = m_pScene->mAnimations[i];

		CAnimation*		pAnimation = CAnimation::Create(pAIAnimation->mName.data, pAIAnimation->mDuration, pAIAnimation->mTicksPerSecond);
		if (nullptr == pAnimation)
			return E_FAIL;

		pAnimation->Reserve(pAIAnimation->mNumChannels);

		for (_uint j = 0; j < pAIAnimation->mNumChannels; ++j)
		{
			aiNodeAnim*	pAIChannel = pAIAnimation->mChannels[j];

			CChannel*	pChannel = CChannel::Create(pAIChannel->mNodeName.data/*, pNode*/);
			if (nullptr == pChannel)
				return E_FAIL;

			_uint		iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
			iNumKeyFrames = max(iNumKeyFrames, pAIChannel->mNumPositionKeys);

			pChannel->Reserve(iNumKeyFrames);

			_float3		vScale;
			_float4		vRotation;
			_float3		vPosition;
			_double		Time;

			for (_uint k = 0; k < iNumKeyFrames; ++k)
			{
				KEYFRAME*			pKeyFrame = new KEYFRAME;
				ZeroMemory(pKeyFrame, sizeof(KEYFRAME));

				if (pAIChannel->mNumScalingKeys > k)
				{
					memcpy(&vScale, &pAIChannel->mScalingKeys[k].mValue, sizeof(_float3));
					Time = pAIChannel->mScalingKeys[k].mTime;
				}

				if (pAIChannel->mNumRotationKeys > k)
				{
					vRotation.x = pAIChannel->mRotationKeys[k].mValue.x;
					vRotation.y = pAIChannel->mRotationKeys[k].mValue.y;
					vRotation.z = pAIChannel->mRotationKeys[k].mValue.z;
					vRotation.w = pAIChannel->mRotationKeys[k].mValue.w;
					Time = pAIChannel->mRotationKeys[k].mTime;
				}

				if (pAIChannel->mNumPositionKeys > k)
				{
					memcpy(&vPosition, &pAIChannel->mPositionKeys[k].mValue, sizeof(_float3));
					Time = pAIChannel->mPositionKeys[k].mTime;
				}

				pKeyFrame->vScale = vScale;
				pKeyFrame->vRotation = vRotation;
				pKeyFrame->vPosition = vPosition;
				pKeyFrame->Time = Time;

				pChannel->Add_KeyFrame(pKeyFrame);
			}

			pAnimation->Add_Channel(pChannel);
		}

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

HRESULT CModel::Link_ChannelToNode()
{
	for (auto& pAnimation : m_Animations)
	{
		const vector<CChannel*>* pChannels = pAnimation->Get_Channels();

		for (auto& pChannel : *pChannels)
		{
			CHierarchyNode* pNode = Find_HierarchyNode(pChannel->Get_Name());
			if (nullptr == pNode)
				return E_FAIL;

			pChannel->Set_HierarchyNodePtr(pNode);
		}
	}

	return S_OK;
}

CHierarchyNode * CModel::Find_HierarchyNode(const char * pName, _uint* pOut)
{
	_uint		iIndex = 0;
	CHierarchyNode*	pNode = nullptr;

	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		if (!strcmp(pHierarchyNode->Get_Name(), pName))
		{
			pNode = pHierarchyNode;
			if (nullptr != pOut)
				*pOut = iIndex;

			break;
		}

		iIndex++;
	}
	return pNode;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, E_MODEL_TYPE eModelType, const char * pModelFilePath, const char * pModelFileName, _fmatrix TransformMatrix)
{
	CModel*	pInstance = new CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eModelType, pModelFilePath, pModelFileName, TransformMatrix)))
	{
		MSGBOX("Failed to Created CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*	pInstance = new CModel(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	for (auto& pMaterial : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(pMaterial->pTexture[i]);

		if (false == m_isCloned)
			Safe_Delete(pMaterial);
	}

	m_Materials.clear();

	if (nullptr != m_pMeshContainers)
	{
		for (_uint i = 0; i < m_iNumMaterials; ++i)
		{
			for (auto& pMeshContainer : m_pMeshContainers[i])
				Safe_Release(pMeshContainer);

			if (false == m_isCloned)
				m_pMeshContainers[i].clear();
		}
	}

	if (false == m_isCloned)
	{
		m_Importer.FreeScene();
		Safe_Delete_Array(m_pMeshContainers);
	}

	for (auto& pNode : m_HierarchyNodes)
		Safe_Release(pNode);

	m_HierarchyNodes.clear();
}