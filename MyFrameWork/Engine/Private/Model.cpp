#include "Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "HierarchyNode.h"
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
//	, m_iCurrentAniIndex(rhs.m_iCurrentAniIndex)
	, mDefaultMatrix(rhs.mDefaultMatrix)
	, m_HierarchyNodes(rhs.m_HierarchyNodes)
{
	// 모델과 재질은 얕은 복사로 사용한다.
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

	// 애니메이션에서 사용하는 계층이 아니기 때문에 복사해둔다.
	for (auto& hier : m_HierarchyNodes)
	{
		Safe_AddRef(hier);
	}
}

_uint CModel::Get_NumMaterials() const
{
	return m_iNumMaterials;
}

//HRESULT CModel::Set_AniString(string AniName)
//{
//	_uint index = Find_Animation(AniName);
//	SetUp_AnimIndex(index);
//	return S_OK;
//}
//
//_uint CModel::Find_Animation(string AniName) const
//{
//	_uint returnIndex = 0;
//
//	for (_uint i = 0; i < m_iNumAnimations; i++)
//	{
//		string name = m_Animations[i]->Get_Name();
//		if (AniName.compare(name) == 0)
//		{
//			returnIndex = i;
//			break;
//		}
//	}
//
//	return returnIndex;
//}


CModel::BONEMATRIX_PTR CModel::Get_BoneMatrixPtr(string bonename) 
{
	BONEMATRIX_PTR		BoneMatrixPtr;
	ZeroMemory(&BoneMatrixPtr, sizeof(BONEMATRIX_PTR));


	// 애니메이션의 노드를 찾아야한다.
	CHierarchyNode * 		pNode = mAnimator->Find_HierarchyNode(bonename.c_str());

	if (nullptr == pNode)
		return BONEMATRIX_PTR();


	BoneMatrixPtr.pOffsetMatrix = pNode->Get_OffsetMatrixPtr();
	BoneMatrixPtr.pCombinedMatrix = pNode->Get_CombinedTransformationMatrixPtr();

	return BoneMatrixPtr;
}

HRESULT CModel::NativeConstruct_Prototype(E_MODEL_TYPE eModelType, const char * pModelFilePath, const char * pModelFileName, _fmatrix defaultMatrix)
{
	char	szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	_uint		iFlag = 0;

	/* 칼, 그림자 */

	if (MODEL_NOANI == eModelType)
		iFlag = aiProcess_PreTransformVertices | /*aiProcess_GlobalScale | */aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate | aiProcess_CalcTangentSpace;

	m_pScene = m_Importer.ReadFile(szFullPath, iFlag);

	if (nullptr == m_pScene)
		return E_FAIL;

	m_eModelType = eModelType;
	XMStoreFloat4x4(&mDefaultMatrix, defaultMatrix);

	/* 모델의 머테리얼 정보. */
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

	// 메쉬의 버퍼를 생성한다. // VIBuffer를 상속받는다.
	// 애니메이션 객체라면 뼈의 정보도 같이 세팅
	if (FAILED(Ready_MeshContainers()))
		return E_FAIL;

	// 재질의 개수 == 메쉬의 모델 개수
	
	// 매쉬 컨테이너의 정점 버퍼를 세팅한다.
	// 정적 오브젝트는 버퍼를 정점만큼 버퍼를 생성한다. == 기존과 비슷하다.

	// 동적 오브젝트는 ㅇ 오브젝트는 추가적으로 BlendIndex를 추가한다.
	// BlendIndex는 애니메이션에서 영향을 주는 뼈의 가중치를 세팅 한다.
	
	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		for (auto& pMeshContainer : m_pMeshContainers[i])
			pMeshContainer->Ready_VertexIndexBuffer(m_eModelType, defaultMatrix);
	}
	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	// 복사시에 세팅 
	// 애니메이션에 해당하는 데이터는 사본에서도 개별적으로 수행되어야한다. 
	// 따라서 애니메이션에 해당하는 데이터는 사본에서 설정한다.


	mAnimator = CAnimatior::Create(m_pScene, m_pMeshContainers);

	//// 1. 다시 계층 설정
	//if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode, nullptr, 0)))
	//	return E_FAIL;

	//sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
	//{
	//	return pSour->Get_Depth() < pDest->Get_Depth();
	//});


	//// 2. 뼈의 이름을 가져와서 현재 계층구조에 동일한지 판단하고 OffsetMat 데이터를 계층에 넣어준다.
	//if (FAILED(Ready_OffsetMatrices()))
	//	return E_FAIL;
	//
	//// 3.pAnimation 객체로 애니메이션에 해당하는 채널(뼈)의 값을 프레임단으로 넣어준다.
	//// 각 애니메이션 객체에 이 키값이 있다.
	//if (FAILED(Ready_Animation()))
	//	return E_FAIL;

	//// 4. 3에서 세팅한 각 애니메이션 내부에 채널을 계층노드와 링크를 시켜준다.
	//// 해당 채널(뼈)의 이름을 계층(뼈)의 이름과 같다면 애니메이션 진행중에 같은 뼈를 움직여야하기 때문에 계층데이터를 넣어준다.
	//// 계층 데이터에는 부모에 따른 최종 행렬 정보가 들어있다.
	//if (FAILED(Link_ChannelToNode()))
	//	return E_FAIL;

	return S_OK;
}
//
//HRESULT CModel::SetUp_AnimIndex(_uint iAnimIndex)
//{
//	if (iAnimIndex >= m_iNumAnimations)
//		return E_FAIL;
//	if (misBlend)
//		return S_FALSE;
//
//	m_iNewAniIndex = iAnimIndex;
//	if (m_iCurrentAniIndex != m_iNewAniIndex)
//	{
//		misBlend = true;
//	}
//	else
//	{
//		m_iCurrentAniIndex = iAnimIndex;
//	}
//	return S_OK;
//}

HRESULT CModel::SetUp_AnimIndex(_uint iAnimIndex)
{
	if (mAnimator == nullptr)
		return E_FAIL;
	mAnimator->SetUp_AnimIndex(iAnimIndex);
	return S_OK;
}

HRESULT CModel::SetUp_AnimEnum(CAnimatior::E_COMMON_ANINAME e)
{
	mAnimator->Set_AniEnum(e);
	return S_OK;
}

HRESULT CModel::SetUp_AnimName(string tag)
{
	mAnimator->Set_AniString(tag);
	return S_OK;
}

HRESULT CModel::Update_CombinedTransformationMatrices(_double TimeDelta)
{
	// 애니메이션 블랜딩 
	mAnimator->Update_CombinedTransformationMatrices(TimeDelta);

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

			pMeshContainer->SetUp_Matrices(BoneMatrices, &mAnimator->Get_HierarchyNode(), mDefaultMatrix);

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

	m_pMeshContainers = NEW MESHCONTAINERS[m_iNumMaterials];

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

	m_iNumMaterials = m_pScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MESHMATERIALDESC*		pMeshMaterialDesc = NEW MESHMATERIALDESC;
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
//
//HRESULT CModel::Ready_OffsetMatrices()
//{
//	for (_uint i = 0; i < m_iNumMaterials; ++i)
//	{
//		for (auto& pMeshContainer : m_pMeshContainers[i])
//		{
//			_uint iNumBones = pMeshContainer->Get_NumBones();
//
//			aiMesh*		pAIMesh = pMeshContainer->Get_AIMesh();
//
//			for (_uint j = 0; j < iNumBones; ++j)
//			{
//				aiBone*		pBone = pAIMesh->mBones[j];
//
//				CHierarchyNode*		pHierarchyNode = Find_HierarchyNode(pBone->mName.data);
//
//				if (nullptr == pHierarchyNode)
//					return E_FAIL;
//
//				_float4x4		OffsetMatrix;
//				memcpy(&OffsetMatrix, &pBone->mOffsetMatrix, sizeof(_float4x4));
//
//				pHierarchyNode->Set_OffsetMatrix(OffsetMatrix);
//			}
//		}
//	}
//
//	return S_OK;
//}
//
//HRESULT CModel::Ready_Animation()
//{
//	if (nullptr == m_pScene)
//		return E_FAIL;
//
//	m_iNumAnimations = m_pScene->mNumAnimations;
//
//	for (_uint i = 0; i < m_iNumAnimations; ++i)
//	{
//		aiAnimation*	pAIAnimation = m_pScene->mAnimations[i];
//
//		CAnimation*		pAnimation = CAnimation::Create(pAIAnimation->mName.data, pAIAnimation->mDuration, pAIAnimation->mTicksPerSecond);
//		if (nullptr == pAnimation)
//			return E_FAIL;
//
//		pAnimation->Reserve(pAIAnimation->mNumChannels);
//
//		for (_uint j = 0; j < pAIAnimation->mNumChannels; ++j)
//		{
//			aiNodeAnim*	pAIChannel = pAIAnimation->mChannels[j];
//
//			CChannel*	pChannel = CChannel::Create(pAIChannel->mNodeName.data/*, pNode*/);
//			if (nullptr == pChannel)
//				return E_FAIL;
//
//			// 가장 큰 프레임으로 
//			_uint		iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
//			iNumKeyFrames = max(iNumKeyFrames, pAIChannel->mNumPositionKeys);
//
//			pChannel->Reserve(iNumKeyFrames);
//
//			_float3		vScale;
//			_float4		vRotation;
//			_float3		vPosition;
//			_double		Time;
//
//			// 각 프레임에 맞는 값을 넣어준다.
//			for (_uint k = 0; k < iNumKeyFrames; ++k)
//			{
//				KEYFRAME*			pKeyFrame = new KEYFRAME;
//				ZeroMemory(pKeyFrame, sizeof(KEYFRAME));
//
//				if (pAIChannel->mNumScalingKeys > k)
//				{
//					memcpy(&vScale, &pAIChannel->mScalingKeys[k].mValue, sizeof(_float3));
//					Time = pAIChannel->mScalingKeys[k].mTime;
//				}
//
//				if (pAIChannel->mNumRotationKeys > k)
//				{
//					vRotation.x = pAIChannel->mRotationKeys[k].mValue.x;
//					vRotation.y = pAIChannel->mRotationKeys[k].mValue.y;
//					vRotation.z = pAIChannel->mRotationKeys[k].mValue.z;
//					vRotation.w = pAIChannel->mRotationKeys[k].mValue.w;
//					Time = pAIChannel->mRotationKeys[k].mTime;
//				}
//
//				if (pAIChannel->mNumPositionKeys > k)
//				{
//					memcpy(&vPosition, &pAIChannel->mPositionKeys[k].mValue, sizeof(_float3));
//					Time = pAIChannel->mPositionKeys[k].mTime;
//				}
//
//				pKeyFrame->vScale = vScale;
//				pKeyFrame->vRotation = vRotation;
//				pKeyFrame->vPosition = vPosition;
//				pKeyFrame->Time = Time;
//
//				pChannel->Add_KeyFrame(pKeyFrame);
//			}
//
//			pAnimation->Add_Channel(pChannel);
//		}
//
//		m_Animations.push_back(pAnimation);
//	}
//	return S_OK;
//}
//
//HRESULT CModel::Link_ChannelToNode()
//{
//	for (auto& pAnimation : m_Animations)
//	{
//		const vector<CChannel*>* pChannels = pAnimation->Get_Channels();
//
//		for (auto& pChannel : *pChannels)
//		{
//			CHierarchyNode* pNode = Find_HierarchyNode(pChannel->Get_Name());
//			if (nullptr == pNode)
//				return E_FAIL;
//			pChannel->Set_HierarchyNodePtr(pNode);
//		}
//	}
//	return S_OK;
//}
//
//HRESULT CModel::AniMationBlend(int startindex, int endindex, _double delta)
//{
//	if (startindex >= m_iNumAnimations ||
//		endindex >= m_iNumAnimations)
//		return E_FAIL;
//
//	// 1. 1번 애니의 시작과 1번 애니의 끝과 2번애니의 끝의 위치를 가져와야한다.
//	// 1번 애니 2번 애니메이션의 채널리스트를 가져온다.
//	auto VecStart_AniChannel = m_Animations[startindex]->Get_Channels();
//	auto VecEnd_AniChannel = m_Animations[endindex]->Get_Channels();
////	m_HierarchyNodes
//
//	if (VecStart_AniChannel == nullptr||
//		VecEnd_AniChannel == nullptr)
//		return E_FAIL;
//
//	// 2. 시간 처리
//	mBlendTimer += delta;
//
//	if (mBlendTimer >= mBlendMaxTime)
//	{
//		mBlendTimer = 0.0;
//		misBlend = false;
//		m_iCurrentAniIndex = endindex;
//		m_Animations[startindex]->Set_AniMationTime(0);
//		m_Animations[endindex]->Set_AniMationTime(0);
//	}
//		
//	_float3			vScale;
//	_quaterion		vRotation;
//	_float3			vPosition;
//
//	// 현재 프레임에 해당하는 위치와 블랜딩 시킨다.
//	_uint boneSize = VecStart_AniChannel->size();
//
//	for (_uint i = 0; i < boneSize; ++i)
//	{
//		// 1. 각 뼈의 키프레임
//		const vector<KEYFRAME*>*	pKeyCurrentFrames = (*VecStart_AniChannel)[i]->Get_KeyFrames();
//		const vector<KEYFRAME*>*	pKeyNewFrames = (*VecEnd_AniChannel)[i]->Get_KeyFrames();
//		
//		// 현재 위치의 키 인덱스 가져오기
//		_uint CurrentFrameIndex = (*VecStart_AniChannel)[i]->Get_CurrentKeyFrame();
//
//		if (misBlend == false)
//		{
//			// pKeyNewFrames의 시작 프레임으로 이동
//			vScale = (*pKeyNewFrames)[0]->vScale;
//			vRotation = (*pKeyNewFrames)[0]->vRotation;
//			vPosition = (*pKeyNewFrames)[0]->vPosition;	
//			(*VecStart_AniChannel)[i]->Set_CurrentKeyFrame(0);
//			(*VecEnd_AniChannel)[i]->Set_CurrentKeyFrame(0);			
//		}
//
//		// 현재 프레임의 뼈들의 프레임을 가져와서 해당 키프레임의 위치로 업데이트
//		else
//		{
//			_double		Ratio = mBlendTimer / mBlendMaxTime;
//
//			_float3		vSourScale, vDestScale;
//			_quaterion	vSourRotation, vDestRotation;
//			_float3		vSourPosition, vDestPosition;
//
//			// mVec_CurrentAniEndFrame
//			vSourScale = (*pKeyCurrentFrames)[CurrentFrameIndex]->vScale;
//			vSourRotation = (*pKeyCurrentFrames)[CurrentFrameIndex]->vRotation;
//			vSourPosition = (*pKeyCurrentFrames)[CurrentFrameIndex]->vPosition;
//
//			// mVec_NewAniStartFrame
//			vDestScale = (*pKeyNewFrames)[0]->vScale;
//			vDestRotation = (*pKeyNewFrames)[0]->vRotation;
//			vDestPosition = (*pKeyNewFrames)[0]->vPosition;
//
//			vScale = _float3::Lerp(vSourScale, vDestScale, _float(Ratio));
//			vRotation = _quaterion::Slerp(vSourRotation, vDestRotation, _float(Ratio));
//			vPosition = _float3::Lerp(vSourPosition, vDestPosition, _float(Ratio));
//		}
//
//		_float4x4 newMat;
//		newMat = _float4x4::CreateScale(vScale) * _float4x4::CreateFromQuaternion(vRotation) * _float4x4::CreateTranslation(vPosition);
//		(*VecEnd_AniChannel)[i]->Set_TransformationMatrix(newMat);
//
//	}
//	int a = 5;
//
//}

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
	CModel*	pInstance = NEW CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eModelType, pModelFilePath, pModelFileName, TransformMatrix)))
	{
		MSGBOX("Failed to Created CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*	pInstance = NEW CModel(*this);

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

	Safe_Release(mAnimator);



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