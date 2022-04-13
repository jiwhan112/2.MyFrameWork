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
	
	// 재질 벡터 복사
	for (auto& mmatrial : mVectorMaterials)
	{
		for (auto& tex : mmatrial->pTexture)
		{
			Safe_AddRef(tex);
		}
	}

	// 메시 벡터들 복사
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

	// 모델을 불러올 때 애니메이션 / 애니가 없는 모델을 분리해서 불러온다.
	_uint		iFlag = 0;

	// 좌표계 업데이트 / 삼각형으로 그림 / 탄젠트공간 계산 / 
	// 애니메이션이 아니면 파츠를 본에 같이 붙여줌
	
	if (meModelType == CModel::MODEL_NOANI)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;

	
	// aiScene 객체는 모든 메쉬 데이터를 저장한다. 
	// 모델 생성자는 aiScene을 초기화하는 것과 다름 없다.
	mScene = mImporter.ReadFile(szFullPath, iFlag);
	if (mScene == nullptr)
		return E_FAIL;

	// Scene 정보로 한 모델의 메쉬 파츠를 저장한다.
	FAILED_CHECK(Ready_Materials(ModelPath));
	FAILED_CHECK(Ready_MeshContainers());

	// 애니메이션이 없는 경우
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

	// 애니메이션이 있다면 계층 / 애니메이션 / 오프셋 모두 세팅
	FAILED_CHECK(Ready_HierarchyNodes(mScene->mRootNode,nullptr,0));
	sort(mVectorHierarchyNodes.begin(), mVectorHierarchyNodes.end(), [](CHierarchyNode* a, CHierarchyNode* b)
	{
		// 오름차순 정렬
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

	// 각 텍스처 바인딩
	CTexture*		pTexture = mVectorMaterials[iMaterialIndex]->pTexture[eTextureType];
	if (nullptr == pTexture)
		return E_FAIL;
	
	return pTexture->SetUp_OnShader(pShader, pValueName);
}

// 재질마다 다른 셰이더로 그려줄 수 있음.`
HRESULT CModel::Render(CShader* shader,_uint pass, _uint iMaterialIndex)
{
	// 바인딩이 끝나면 메쉬의 개수가 아닌 재질의 개수로 루프를 돈다.
	if (iMaterialIndex >= mNumMaterials ||
		mpVectorMeshContainers == nullptr)
		return E_FAIL;


	// 메시컨테이너들을 저장하는 컨테이너를 재질 개수만큼 만들고
	// 메시컨테이너를 재질 개수만큼 호출시킨다.
	for (auto& pMeshContainer : mpVectorMeshContainers[iMaterialIndex])
	{
		// 기존 같이 메시버퍼 랜더링
		if (pMeshContainer != nullptr)
			pMeshContainer->Render(shader, pass);
	}
	return S_OK;
}



HRESULT CModel::Ready_MeshContainers()
{
	// 메쉬 파츠 초기화

	if (nullptr == mScene)
		return E_FAIL;

	// 메시 컨테이너 벡터를 재질의 총 개수만큼 동적할당.
	mNumMeshContainer = mScene->mNumMeshes;
	mpVectorMeshContainers = NEW MESHCONTAINERS[mNumMaterials];

	// 메시 개수 만큼
	for (_uint i = 0; i < mNumMeshContainer; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, mScene->mMeshes[i]);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		// 각 메시가 가지고 있는 재질 번호로 메시를 넣는다.
		// 즉 같은 번호의 텍스처를 사용해 그리는 객체는 같은 벡터[ID]에 저장된다.
		mpVectorMeshContainers[mScene->mMeshes[i]->mMaterialIndex].push_back(pMeshContainer);
	}
	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	// 재질 초기화
	if (mScene == nullptr)
		return E_FAIL;
	
	mNumMaterials = mScene->mNumMaterials;
	for (_uint i = 0; i < mNumMaterials; ++i)
	{
		MESHMATERIALDESC* meshDesc = NEW MESHMATERIALDESC;
		ZeroMemory(meshDesc, sizeof(MESHMATERIALDESC));

		// 재질 총 타입을 순회하면서 맞는 재질 업데이트
		for (_uint k = 0; k < AI_TEXTURE_TYPE_MAX; ++k)
		{
			// mseh의 텍스처 경로 반환
			aiString texturepath;
			if (FAILED(mScene->mMaterials[i]->GetTexture(aiTextureType(k), 0, &texturepath)))
				continue;

			// 텍스처 이름 추출
			char szFullPath[MAX_PATH] = "";
			char szFileName[MAX_PATH] = "";
			char szEXE[MAX_PATH] = "";

			_splitpath_s(texturepath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXE, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXE);

			// 유니코드 변경
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

	// 계층 구조를 만든다.
	_float4x4 TransformMat;
	memcpy(&TransformMat,&pAiNode->mTransformation,sizeof(_float4x4));

	// ai노드 데이터로 루트노드부터 부모 -> 자식 관계를 만들어준다.
	CHierarchyNode* node = CHierarchyNode::Create(pAiNode->mName.data, TransformMat, iDepth, pParent);
	mVectorHierarchyNodes.push_back(node);

	// 자식에 같은 부모를 넣오면서 처리한다.

	for (_uint i=0; i<pAiNode->mNumChildren; ++i)
	{
		Ready_HierarchyNodes(pAiNode, node, iDepth + 1);
	}

	return S_OK;
}

HRESULT CModel::Ready_OffsetMatrices()
{
	// 뼈에 있는 Offset 행렬 세팅
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

	// 애니메이션 개수만큼 애니메이션 생성
	mNumAnimations = mScene->mNumAnimations;
	for (_uint i=0;i<mNumAnimations;++i)
	{
		// aiAnimation 정보로 애니메이션 생성 
		aiAnimation* pAiani = mScene->mAnimations[i];
		CAnimation* pAni = CAnimation::Create(pAiani->mName.data, pAiani->mDuration, pAiani->mTicksPerSecond);
		if (nullptr == pAni)
			return E_FAIL;

		pAni->Reserve(pAiani->mNumChannels);

		for (_uint j=0;j<pAiani->mNumChannels;++j)
		{
			// 각 애니메이션의 뼈 개수 만큼 루프해서 채널의 정보를 저장한다.

			// aiNodeAnim 애니메이션 노드 == 채널
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

				// 현재 키 탐색
				if (pAniChannel->mNumScalingKeys > k)
				{
					// 크기 / 시간
					memcpy(&vScale, &pAniChannel->mScalingKeys[k].mValue, sizeof(_float3));
					Time = pAniChannel->mScalingKeys[k].mTime;
				}
				if (pAniChannel->mNumRotationKeys > k)
				{
					// 회전 값은 반대로 정렬되어있어서 직접 넣는다.
					vRot.x = pAniChannel->mRotationKeys[k].mValue.x;
					vRot.y = pAniChannel->mRotationKeys[k].mValue.y;
					vRot.z = pAniChannel->mRotationKeys[k].mValue.z;
					vRot.w = pAniChannel->mRotationKeys[k].mValue.w;
					Time = pAniChannel->mRotationKeys[k].mTime;

				}
				if (pAniChannel->mNumScalingKeys > k)
				{
					// 위치 / 시간
					memcpy(&vScale, &pAniChannel->mPositionKeys[k].mValue, sizeof(_float3));
					Time = pAniChannel->mPositionKeys[k].mTime;
				}
				pKeyFrame->vScale = vScale;
				pKeyFrame->vRotation = vRot;
				pKeyFrame->vPosition = vPos;
				pKeyFrame->Time = Time;

				pChannel->Add_KeyFrame(pKeyFrame);
			}
			// 애니메이션의 뼈 정보 삽입
			pAni->Add_Channel(pChannel);
		}
		// 애니메이션 삽입
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

	// 재질 해제
	for (auto& mat : mVectorMaterials)
	{
		for (auto& tex : mat->pTexture)
			Safe_Release(tex);

		if (m_isCloned == false)
			Safe_Delete(mat);	
	}
	mVectorMaterials.clear();


	// 메쉬 해제
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

	// 하이어라키 해제
	for (auto& pNode : mVectorHierarchyNodes)
		Safe_Release(pNode);


	for (auto& ani : mVectorAnimations)
		Safe_Release(ani);



}
