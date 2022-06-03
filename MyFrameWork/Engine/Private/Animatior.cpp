#include "Animatior.h"
#include "HierarchyNode.h"
#include "AnimationClip.h"
#include "Channel.h"
#include "MeshContainer.h"


CAnimatior::CAnimatior(const aiScene* _aiscene)
	:m_pScene(_aiscene)
{
}

HRESULT CAnimatior::NativeConstruct(const vector<CMeshContainer*>* meshcon)
{
	// 애니메이션 클립 초기화
	if (m_pScene == nullptr || meshcon == nullptr)
		return E_FAIL;

	// 1. 다시 계층 설정
	if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	// 2. 뼈의 이름을 가져와서 현재 계층구조에 동일한지 판단하고 OffsetMat 데이터를 계층에 넣어준다.
	if (FAILED(Ready_OffsetMatrices(meshcon)))
		return E_FAIL;

	// 3.pAnimation 객체로 애니메이션에 해당하는 채널(뼈)의 값을 프레임단으로 넣어준다.
	// 각 애니메이션 객체에 이 키값이 있다.
	if (FAILED(Ready_Animation()))
		return E_FAIL;

	// 4. 3에서 세팅한 각 애니메이션 내부에 채널을 계층노드와 링크를 시켜준다.
	// 해당 채널(뼈)의 이름을 계층(뼈)의 이름과 같다면 애니메이션 진행중에 같은 뼈를 움직여야하기 때문에 계층데이터를 넣어준다.
	// 계층 데이터에는 부모에 따른 최종 행렬 정보가 들어있다.
	if (FAILED(Link_ChannelToNode()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimatior::Update_CombinedTransformationMatrices(_double timer)
{
	if (misBlend)
	{
		AniMationBlend(m_iCurrentAniIndex, m_iNewAniIndex, timer);
		mIsCurrentAniEnd = false;
	}

	else
	{
		// 현재 애니메이션을 시간에 맞게 업데이트를 시켜준다.
		// 애니메이션 채널의 해당 시간에 따른 뼈 위치 갱신
		mVecAnimations[m_iCurrentAniIndex]->Update_TransformMatrices(timer);
		if(mVecAnimations[m_iCurrentAniIndex]->Get_Finished())
			mIsCurrentAniEnd = true;
	}

	// 갱신된 뼈행렬을 계층으로 업데이트 시켜준다.
	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Update_CombinedTransformationMatrix();
	}
	return S_OK;
}


HRESULT CAnimatior::Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth)
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

HRESULT CAnimatior::Ready_OffsetMatrices(const vector<CMeshContainer*>* meshcon)
{
	for (_uint i = 0; i < m_pScene->mNumMaterials; ++i)
	{
		for (auto& pMeshContainer : meshcon[i])
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


HRESULT CAnimatior::Set_AniString(string AniName)
{
	int index = Find_Animation(AniName);
	SetUp_AnimIndex(index);
	return S_OK;
}

void CAnimatior::Set_CurrentAnimaionTime(_float time)
{
	mVecAnimations[m_iCurrentAniIndex]->Set_AniMationTime(time);
}

CAnimationClip* CAnimatior::Get_CurrentAnimaion() const
{
	return mVecAnimations[m_iCurrentAniIndex];
}

_double CAnimatior::Get_CurrentAnimationTimeRatio() const
{
	_double anitime = mVecAnimations[m_iCurrentAniIndex]->Get_AniMationTime();
	_double anitimeMax = mVecAnimations[m_iCurrentAniIndex]->Get_MaxAnimaionTime();
	return (anitime / anitimeMax);
}

HRESULT CAnimatior::SetUp_AnimIndex(_uint iAnimIndex)
{
	if (iAnimIndex >= m_iNumAnimations)
		return E_FAIL;
	if (misBlend)
		return S_FALSE;

	m_iNewAniIndex = iAnimIndex;
	if (m_iCurrentAniIndex != m_iNewAniIndex)
	{
		misBlend = true;
	}
	else
	{
		m_iCurrentAniIndex = iAnimIndex;
		mIsCurrentAniEnd = false;
	}
	return S_OK;
}

HRESULT CAnimatior::Set_AniEnum(E_COMMON_ANINAME AniName)
{
	// 공통적인 애니메이션 재성
	_int aniIndex =  Get_AniEnum2Index(AniName);
	if (aniIndex < 0)
		return E_FAIL;

	SetUp_AnimIndex(aniIndex);
	return S_OK;
}

_int CAnimatior::Get_AniEnum2Index(E_COMMON_ANINAME AniName)
{
	if (mVecAnimations.empty())
		return -1;
	string aniname = STR_CommonAniName(AniName);

	_int index = 0;
	_int GetIndex = -1;
	vector<int>		VecInts;
	for (auto& ani : mVecAnimations)
	{
		string STR_FullNames = ani->Get_Name();
		vector<string> Name = CHelperClass::String_Split(STR_FullNames, '@');
		string ModelName = Name[0];
		string Full_AniNames = Name[1];

		vector<string> AniNames = CHelperClass::String_Split(Full_AniNames, '_');
		
	
		if (aniname == AniNames[0])
		{
			VecInts.push_back(index);
		}
		index++;
	}
	if (VecInts.empty())
		return -1;
	
	int randIndex = CHelperClass::RandomInt(0,VecInts.size());
	return VecInts[randIndex];

}

_double CAnimatior::Get_AniIndex2AniMaxTime(_int index)
{
	return mVecAnimations[index]->Get_MaxAnimaionTime();
}
_double CAnimatior::Get_NewIndexAniMaxTime() const
{
	return mVecAnimations[m_iNewAniIndex]->Get_MaxAnimaionTime();
}

HRESULT CAnimatior::Ready_Animation()
{
	m_iNumAnimations = m_pScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation*	pAIAnimation = m_pScene->mAnimations[i];

		CAnimationClip*		pAnimation = CAnimationClip::Create(pAIAnimation->mName.data, pAIAnimation->mDuration, pAIAnimation->mTicksPerSecond);
		if (nullptr == pAnimation)
			return E_FAIL;

		pAnimation->Reserve(pAIAnimation->mNumChannels);

		for (_uint j = 0; j < pAIAnimation->mNumChannels; ++j)
		{
			aiNodeAnim*	pAIChannel = pAIAnimation->mChannels[j];

			CChannel*	pChannel = CChannel::Create(pAIChannel->mNodeName.data/*, pNode*/);
			if (nullptr == pChannel)
				return E_FAIL;

			// 가장 큰 프레임으로 
			_uint		iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
			iNumKeyFrames = max(iNumKeyFrames, pAIChannel->mNumPositionKeys);

			pChannel->Reserve(iNumKeyFrames);

			_float3		vScale;
			_float4		vRotation;
			_float3		vPosition;
			_double		Time;

			// 각 프레임에 맞는 값을 넣어준다.
			for (_uint k = 0; k < iNumKeyFrames; ++k)
			{
				KEYFRAME*			pKeyFrame = NEW KEYFRAME;
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

		mVecAnimations.push_back(pAnimation);
	}
	return S_OK;
}

HRESULT CAnimatior::Link_ChannelToNode()
{
	for (auto& pAnimation : mVecAnimations)
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

HRESULT CAnimatior::AniMationBlend(int startindex, int endindex, _double delta)
{
	if (startindex >= m_iNumAnimations ||
		endindex >= m_iNumAnimations)
		return E_FAIL;

	// 1. 1번 애니의 시작과 1번 애니의 끝과 2번애니의 끝의 위치를 가져와야한다.
	// 1번 애니 2번 애니메이션의 채널리스트를 가져온다.
	auto VecStart_AniChannel = mVecAnimations[startindex]->Get_Channels();
	auto VecEnd_AniChannel = mVecAnimations[endindex]->Get_Channels();
//	m_HierarchyNodes

	if (VecStart_AniChannel == nullptr||
		VecEnd_AniChannel == nullptr)
		return E_FAIL;

	// 2. 시간 처리
	mBlendTimer += delta;

	if (mBlendTimer >= mBlendMaxTime)
	{
		mBlendTimer = 0.0;
		misBlend = false;
		m_iCurrentAniIndex = endindex;
		mVecAnimations[startindex]->Set_AniMationTime(0);
		mVecAnimations[endindex]->Set_AniMationTime(0);
	}
		
	_float3			vScale;
	_quaterion		vRotation;
	_float3			vPosition;

	// 현재 프레임에 해당하는 위치와 블랜딩 시킨다.
	_uint boneSize = VecStart_AniChannel->size();

	for (_uint i = 0; i < boneSize; ++i)
	{
		// 1. 각 뼈의 키프레임
		const vector<KEYFRAME*>*	pKeyCurrentFrames = (*VecStart_AniChannel)[i]->Get_KeyFrames();
		const vector<KEYFRAME*>*	pKeyNewFrames = (*VecEnd_AniChannel)[i]->Get_KeyFrames();
		
		// 현재 위치의 키 인덱스 가져오기
		_uint CurrentFrameIndex = (*VecStart_AniChannel)[i]->Get_CurrentKeyFrame();

		if (misBlend == false)
		{
			// pKeyNewFrames의 시작 프레임으로 이동
			vScale = (*pKeyNewFrames)[0]->vScale;
			vRotation = (*pKeyNewFrames)[0]->vRotation;
			vPosition = (*pKeyNewFrames)[0]->vPosition;				
		}

		// 현재 프레임의 뼈들의 프레임을 가져와서 해당 키프레임의 위치로 업데이트
		else
		{
			_double		Ratio = mBlendTimer / mBlendMaxTime;

			_float3		vSourScale, vDestScale;
			_quaterion	vSourRotation, vDestRotation;
			_float3		vSourPosition, vDestPosition;

			// mVec_CurrentAniEndFrame
			vSourScale = (*pKeyCurrentFrames)[CurrentFrameIndex]->vScale;
			vSourRotation = (*pKeyCurrentFrames)[CurrentFrameIndex]->vRotation;
			vSourPosition = (*pKeyCurrentFrames)[CurrentFrameIndex]->vPosition;

			// mVec_NewAniStartFrame
			vDestScale = (*pKeyNewFrames)[0]->vScale;
			vDestRotation = (*pKeyNewFrames)[0]->vRotation;
			vDestPosition = (*pKeyNewFrames)[0]->vPosition;

			vScale = _float3::Lerp(vSourScale, vDestScale, _float(Ratio));
			vRotation = _quaterion::Slerp(vSourRotation, vDestRotation, _float(Ratio));
			vPosition = _float3::Lerp(vSourPosition, vDestPosition, _float(Ratio));
		}

		_float4x4 newMat;
		newMat = _float4x4::CreateScale(vScale) * _float4x4::CreateFromQuaternion(vRotation) * _float4x4::CreateTranslation(vPosition);
		(*VecEnd_AniChannel)[i]->Set_TransformationMatrix(newMat);

	}
	return S_OK;
}

CHierarchyNode * CAnimatior::Find_HierarchyNode(const char * pName, _uint* pOut) const
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

_int  CAnimatior::Find_Animation(string AniName) const
{
	_int  returnIndex = -1;

	for (_uint i = 0; i < m_iNumAnimations; i++)
	{
		string name = mVecAnimations[i]->Get_Name();
		if (AniName.compare(name) == 0)
		{
			returnIndex = i;
			break;
		}
	}

	return returnIndex;
}

CAnimatior * CAnimatior::Create(const aiScene* scene, const vector<class CMeshContainer*>* con)
{
	CAnimatior*	pInstance = NEW CAnimatior(scene);

	if (FAILED(pInstance->NativeConstruct(con)))
	{
		MSGBOX("Failed to Created CAnimatior");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimatior::Free()
{
	for (auto& pAnimation : mVecAnimations)
		Safe_Release(pAnimation);

	for (auto& pHier : m_HierarchyNodes)
		Safe_Release(pHier);
	
	
}