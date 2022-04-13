#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "Shader.h"
#include "HierarchyNode.h"
#include "Animation.h"
#include "Channel.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, mNumMeshContainer(rhs.mNumMeshContainer)
	, mpVectorMeshContainers(rhs.mpVectorMeshContainers)
	, meModelType(rhs.meModelType)
	, mNumMaterials(rhs.mNumMaterials)
	, mVectorMaterials(rhs.mVectorMaterials)
	, miCurrentAnim(rhs.miCurrentAnim)
	, mNumAnimations(rhs.mNumAnimations)
	, mVectorAnimations(rhs.mVectorAnimations)

{
	
	// ���� ���� ����
	for (auto& mmatrial : mVectorMaterials)
	{
		for (auto& tex : mmatrial->pTexture)
		{
			Safe_AddRef(tex);
		}
	}

	// �޽� ���͵� ����
	if(mpVectorMeshContainers != nullptr)
	{
		for (_uint i = 0; i<mNumMaterials;++i)
		{
			for (auto& mesh : mpVectorMeshContainers[i])
			{
				Safe_AddRef(mesh);
			}
		}

	}

	for (auto& ani : mVectorAnimations)
	{
		Safe_AddRef(ani);
	}


}



HRESULT CModel::NativeConstruct_Prototype(E_MODEL_TYPE type, const char * ModelPath, const char * ModelName, _fmatrix defaultMatrix)
{
	char	szFullPath[MAX_PATH] = "";

	if (type == CModel::MODEL_END)
		return E_FAIL;

	meModelType = type;
	XMStoreFloat4x4(&mTransformMatrix, defaultMatrix);

	strcpy_s(szFullPath, ModelPath);
	strcat_s(szFullPath, ModelName);

	// ���� �ҷ��� �� �ִϸ��̼� / �ִϰ� ���� ���� �и��ؼ� �ҷ��´�.
	_uint		iFlag = 0;

	// ��ǥ�� ������Ʈ / �ﰢ������ �׸� / ź��Ʈ���� ��� / 
	// �ִϸ��̼��� �ƴϸ� ������ ���� ���� �ٿ���
	
	if (meModelType == CModel::MODEL_NOANI)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;

	
	// aiScene ��ü�� ��� �޽� �����͸� �����Ѵ�. 
	// �� �����ڴ� aiScene�� �ʱ�ȭ�ϴ� �Ͱ� �ٸ� ����.
	mScene = mImporter.ReadFile(szFullPath, iFlag);
	if (mScene == nullptr)
		return E_FAIL;

	// Scene ������ �� ���� �޽� ������ �����Ѵ�.
	FAILED_CHECK(Ready_Materials(ModelPath));
	FAILED_CHECK(Ready_MeshContainers());

	// �ִϸ��̼��� ���� ���
	if (meModelType == CModel::MODEL_NOANI)
	{
		for (_uint i =0; i<mNumMaterials;++i)
		{
			for (auto& a: mpVectorMeshContainers[i])
			{
				a->Ready_VertexIndexBuffer(meModelType, defaultMatrix);
			}
		}
		return S_OK;
	}

	// �ִϸ��̼��� �ִٸ� ���� / �ִϸ��̼� / ������ ��� ����
	FAILED_CHECK(Ready_HierarchyNodes(mScene->mRootNode,nullptr,0));
	sort(mVectorHierarchyNodes.begin(), mVectorHierarchyNodes.end(), [](CHierarchyNode* a, CHierarchyNode* b)
	{
		// �������� ����
		return a->Get_Depth() < b->Get_Depth();
	});

	FAILED_CHECK(Ready_OffsetMatrices());

	for (_uint i = 0; i < mNumMaterials; ++i)
	{
		for (auto& pMeshContainer : mpVectorMeshContainers[i])
			pMeshContainer->Ready_VertexIndexBuffer(meModelType, defaultMatrix);
	}

	FAILED_CHECK(Ready_Animation());

	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	return S_OK;
}



HRESULT CModel::Bind_OnShader(CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char * pValueName)
{
	if (iMaterialIndex >= mNumMaterials)
		return E_FAIL;

	// �� �ؽ�ó ���ε�
	CTexture*		pTexture = mVectorMaterials[iMaterialIndex]->pTexture[eTextureType];
	if (nullptr == pTexture)
		return E_FAIL;
	
	return pTexture->SetUp_OnShader(pShader, pValueName);
}

// �������� �ٸ� ���̴��� �׷��� �� ����.`
HRESULT CModel::Render(CShader* shader,_uint pass, _uint iMaterialIndex)
{
	// ���ε��� ������ �޽��� ������ �ƴ� ������ ������ ������ ����.
	if (iMaterialIndex >= mNumMaterials ||
		mpVectorMeshContainers == nullptr)
		return E_FAIL;


	// �޽������̳ʵ��� �����ϴ� �����̳ʸ� ���� ������ŭ �����
	// �޽������̳ʸ� ���� ������ŭ ȣ���Ų��.
	for (auto& pMeshContainer : mpVectorMeshContainers[iMaterialIndex])
	{
		// ���� ���� �޽ù��� ������
		if (pMeshContainer != nullptr)
			pMeshContainer->Render(shader, pass);
	}
	return S_OK;
}



HRESULT CModel::Ready_MeshContainers()
{
	// �޽� ���� �ʱ�ȭ

	if (nullptr == mScene)
		return E_FAIL;

	// �޽� �����̳� ���͸� ������ �� ������ŭ �����Ҵ�.
	mNumMeshContainer = mScene->mNumMeshes;
	mpVectorMeshContainers = NEW MESHCONTAINERS[mNumMaterials];

	// �޽� ���� ��ŭ
	for (_uint i = 0; i < mNumMeshContainer; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, mScene->mMeshes[i]);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		// �� �޽ð� ������ �ִ� ���� ��ȣ�� �޽ø� �ִ´�.
		// �� ���� ��ȣ�� �ؽ�ó�� ����� �׸��� ��ü�� ���� ����[ID]�� ����ȴ�.
		mpVectorMeshContainers[mScene->mMeshes[i]->mMaterialIndex].push_back(pMeshContainer);
	}
	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	// ���� �ʱ�ȭ
	if (mScene == nullptr)
		return E_FAIL;
	
	mNumMaterials = mScene->mNumMaterials;
	for (_uint i = 0; i < mNumMaterials; ++i)
	{
		MESHMATERIALDESC* meshDesc = NEW MESHMATERIALDESC;
		ZeroMemory(meshDesc, sizeof(MESHMATERIALDESC));

		// ���� �� Ÿ���� ��ȸ�ϸ鼭 �´� ���� ������Ʈ
		for (_uint k = 0; k < AI_TEXTURE_TYPE_MAX; ++k)
		{
			// mseh�� �ؽ�ó ��� ��ȯ
			aiString texturepath;
			if (FAILED(mScene->mMaterials[i]->GetTexture(aiTextureType(k), 0, &texturepath)))
				continue;

			// �ؽ�ó �̸� ����
			char szFullPath[MAX_PATH] = "";
			char szFileName[MAX_PATH] = "";
			char szEXE[MAX_PATH] = "";

			_splitpath_s(texturepath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXE, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXE);

			// �����ڵ� ����
			_tchar		szTextureFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szTextureFullPath, MAX_PATH);

			meshDesc->pTexture[k] = CTexture::Create(m_pDevice, m_pDeviceContext,szTextureFullPath,1);
			if (nullptr == meshDesc->pTexture[k])
				return E_FAIL;

		}
		mVectorMaterials.push_back(meshDesc);
	}

	return S_OK;
}

HRESULT CModel::Ready_HierarchyNodes(aiNode * pAiNode, CHierarchyNode * pParent, _uint iDepth)
{
	if (nullptr == pAiNode)
		return E_FAIL;

	// ���� ������ �����.
	_float4x4 TransformMat;
	memcpy(&TransformMat,&pAiNode->mTransformation,sizeof(_float4x4));

	// ai��� �����ͷ� ��Ʈ������ �θ� -> �ڽ� ���踦 ������ش�.
	CHierarchyNode* node = CHierarchyNode::Create(pAiNode->mName.data, TransformMat, iDepth, pParent);
	mVectorHierarchyNodes.push_back(node);

	// �ڽĿ� ���� �θ� �ֿ��鼭 ó���Ѵ�.

	for (_uint i=0; i<pAiNode->mNumChildren; ++i)
	{
		Ready_HierarchyNodes(pAiNode, node, iDepth + 1);
	}

	return S_OK;
}

HRESULT CModel::Ready_OffsetMatrices()
{
	// ���� �ִ� Offset ��� ����
	for (_uint i=0; i<mNumMaterials;++i)
	{
		for (auto& mesh : mpVectorMeshContainers[i])
		{
			_uint numBones = mesh->Get_NumBones();
			aiMesh* pAiMesh = mesh->Get_AIMesh();

			for (_uint j=0; j<numBones; ++j)
			{
				aiBone* pBone = pAiMesh->mBones[j];
				CHierarchyNode* pHier = Find_HeirarchyNode(pBone->mName.data);

				if (nullptr == pHier)
					return E_FAIL;
				_float4x4 offsetMat;
				memcpy(&offsetMat , &pBone->mOffsetMatrix,sizeof(_float4x4));

				pHier->Set_OffsetMat(offsetMat);
				mesh->Add_HierarchyNode(pHier);
			}
		}
	}
	return S_OK;
}

HRESULT CModel::Ready_Animation()
{
	if (mScene == nullptr)
		return E_FAIL;

	// �ִϸ��̼� ������ŭ �ִϸ��̼� ����
	mNumAnimations = mScene->mNumAnimations;
	for (_uint i=0;i<mNumAnimations;++i)
	{
		// aiAnimation ������ �ִϸ��̼� ���� 
		aiAnimation* pAiani = mScene->mAnimations[i];
		CAnimation* pAni = CAnimation::Create(pAiani->mName.data, pAiani->mDuration, pAiani->mTicksPerSecond);
		if (nullptr == pAni)
			return E_FAIL;

		pAni->Reserve(pAiani->mNumChannels);

		for (_uint j=0;j<pAiani->mNumChannels;++j)
		{
			// �� �ִϸ��̼��� �� ���� ��ŭ �����ؼ� ä���� ������ �����Ѵ�.

			// aiNodeAnim �ִϸ��̼� ��� == ä��
			aiNodeAnim* pAniChannel = pAiani->mChannels[j];
			CHierarchyNode* pNode = Find_HeirarchyNode(pAniChannel->mNodeName.data);
			if (pNode == nullptr)
				return E_FAIL;

			CChannel* pChannel = CChannel::Create(pAniChannel->mNodeName.data, pNode);
			if (pChannel == nullptr)
				return E_FAIL;

			_uint NumKetyrames = max(pAniChannel->mNumScalingKeys, pAniChannel->mNumRotationKeys);
			NumKetyrames = max(NumKetyrames, pAniChannel->mNumPositionKeys);

			pChannel->Reserve(NumKetyrames);

			_float3 vScale;
			_float4 vRot;
			_float3 vPos;
			_double Time;
			
			for (_uint k = 0; k < NumKetyrames; i++)
			{
				KEYFRAME* pKeyFrame = NEW KEYFRAME;
				ZeroMemory(pKeyFrame,sizeof(KEYFRAME));

				// ���� Ű Ž��
				if (pAniChannel->mNumScalingKeys > k)
				{
					// ũ�� / �ð�
					memcpy(&vScale, &pAniChannel->mScalingKeys[k].mValue, sizeof(_float3));
					Time = pAniChannel->mScalingKeys[k].mTime;
				}
				if (pAniChannel->mNumRotationKeys > k)
				{
					// ȸ�� ���� �ݴ�� ���ĵǾ��־ ���� �ִ´�.
					vRot.x = pAniChannel->mRotationKeys[k].mValue.x;
					vRot.y = pAniChannel->mRotationKeys[k].mValue.y;
					vRot.z = pAniChannel->mRotationKeys[k].mValue.z;
					vRot.w = pAniChannel->mRotationKeys[k].mValue.w;
					Time = pAniChannel->mRotationKeys[k].mTime;

				}
				if (pAniChannel->mNumScalingKeys > k)
				{
					// ��ġ / �ð�
					memcpy(&vScale, &pAniChannel->mPositionKeys[k].mValue, sizeof(_float3));
					Time = pAniChannel->mPositionKeys[k].mTime;
				}
				pKeyFrame->vScale = vScale;
				pKeyFrame->vRotation = vRot;
				pKeyFrame->vPosition = vPos;
				pKeyFrame->Time = Time;

				pChannel->Add_KeyFrame(pKeyFrame);
			}
			// �ִϸ��̼��� �� ���� ����
			pAni->Add_Channel(pChannel);
		}
		// �ִϸ��̼� ����
		mVectorAnimations.push_back(pAni);
	}

	return S_OK;
}

CHierarchyNode * CModel::Find_HeirarchyNode(const char * name)
{
	auto iter = find_if(mVectorHierarchyNodes.begin(), mVectorHierarchyNodes.end(), [&](CHierarchyNode* node)
	{
		return !strcmp(node->Get_Name(),name);
	});
	if (iter == mVectorHierarchyNodes.end())
		return nullptr;

	return *iter;

}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, E_MODEL_TYPE eModelType, const char * modelPath, const char * modelName, _fmatrix defaultMatrix)
{
	CModel*	pInstance = NEW CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eModelType, modelPath, modelName, defaultMatrix)))
	{
		MSGBOX("Failed to Creating CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CModel * CModel::Clone(void * pArg)
{
	CModel*	pInstance = NEW CModel(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	// ���� ����
	for (auto& mat : mVectorMaterials)
	{
		for (auto& tex : mat->pTexture)
			Safe_Release(tex);

		if (m_isCloned == false)
			Safe_Delete(mat);	
	}
	mVectorMaterials.clear();


	// �޽� ����
	if (mpVectorMeshContainers != nullptr)
	{
		for (_uint i = 0; i < mNumMaterials; ++i)
		{
			for (auto& mesh : mpVectorMeshContainers[i])
				Safe_Release(mesh);

			if (m_isCloned == false)
				mpVectorMeshContainers[i].clear();
		}
	}

	if (false == m_isCloned)
	{
		Safe_Delete_Array(mpVectorMeshContainers);
		mImporter.FreeScene();

	}

	// ���̾��Ű ����
	for (auto& pNode : mVectorHierarchyNodes)
		Safe_Release(pNode);


	for (auto& ani : mVectorAnimations)
		Safe_Release(ani);



}
