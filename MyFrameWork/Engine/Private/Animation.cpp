#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::NativeConstruct(const char * name, _double Duration, _double TickperSecond)
{
	strcpy_s(mSzName,name);
	mDuration = Duration;
	mTickPerAniTimeScale = TickperSecond;

	return S_OK;
}

HRESULT CAnimation::Update_TransformMatrices(_double TimeDelta)
{
	// 각 채널의 키프레임 정보로 애니메이션 정보 업데이트
	// 키 프레임 정보 

	// 현재 애니메이션 시간 = 애니메이션 배속 * 시간
	mPlayTimeAcc += mTickPerAniTimeScale * TimeDelta;

	// 애니메이션 시간으로 Finish 체크
	if (mPlayTimeAcc >= mDuration)
	{
		mPlayTimeAcc = 0;
		misFinished = true;
	}
	
	// 각 애니메이션의 키프레임 데이터 업데이트
	_vector vScale, vRot, vPos;

	// 가지고 있는 채널 개수 만큼 루프
	for (_uint i = 0; i<mNumChannels; ++i)
	{
		// 각 채널의 키프레임 정보를 가져온다.
		const vector<KEYFRAME*>* pVecKey = mVectorCheannels[i]->Get_VecKeyFrame();
		if (pVecKey == nullptr)
			return E_FAIL;

	
		_uint NumKeyFrame = pVecKey->size();
		_uint CurrentKeyFrameIndex = mVectorCheannels[i]->Get_CurrentKeyFrame();

		// 마지막 키프레임 처리
		if (mPlayTimeAcc >= (*pVecKey)[NumKeyFrame - 1]->Time)
		{
			vScale = XMLoadFloat3(&(*pVecKey)[NumKeyFrame - 1]->vScale);
			vRot = XMLoadFloat4(&(*pVecKey)[NumKeyFrame - 1]->vRotation);
			vPos = XMLoadFloat3(&(*pVecKey)[NumKeyFrame - 1]->vPosition);
		}
		else
		{
			// 현재 프레임과 다음 프레임을 가져와서 현재 프레임을 알아오자

			// 현재 시간이 다음프레임 보다 크다면 뼈의 프레임으로 맞춰주자.
			// 시간이 크게 들어온다면 그 다음프레임으로까지 이동할 수 있음
			while(mPlayTimeAcc >= (*pVecKey)[CurrentKeyFrameIndex + 1]->Time)
			{
				mVectorCheannels[i]->Set_CurrentKeyFrame(++CurrentKeyFrameIndex);
			}

			// 현재 애니메이션 진행 시간 비율 = (진행시간) / (총시간)
			// 비율 = (현재 프레임시간) / (다음프레임시간 - 현재프레임시간)
			_double Ratio = (mPlayTimeAcc -(*pVecKey)[CurrentKeyFrameIndex]->Time)
				/ ((*pVecKey)[CurrentKeyFrameIndex+1]->Time - (*pVecKey)[CurrentKeyFrameIndex]->Time);


			_vector srcScale, srcRot, srcPos;
			_vector dstScale, dstRot, dstPos;


			srcScale = XMLoadFloat3(&(*pVecKey)[CurrentKeyFrameIndex]->vScale);
			srcRot = XMLoadFloat4(&(*pVecKey)[CurrentKeyFrameIndex]->vRotation);
			srcPos = XMLoadFloat3(&(*pVecKey)[CurrentKeyFrameIndex]->vPosition);


			dstScale = XMLoadFloat3(&(*pVecKey)[CurrentKeyFrameIndex + 1]->vScale);
			dstRot = XMLoadFloat4(&(*pVecKey)[CurrentKeyFrameIndex + 1]->vRotation);
			dstPos = XMLoadFloat3(&(*pVecKey)[CurrentKeyFrameIndex + 1]->vPosition);

			// 현재 키프레임 위치 ~ 다음 키프레임 위치 보간한다.
			// 회전은 쿼터니언 보간함수를 사용한다.
			vScale = XMVectorLerp(srcScale, dstScale, Ratio);
			vRot = XMQuaternionSlerp(srcScale, dstScale, Ratio);
			vPos = XMVectorLerp(srcPos, dstPos, Ratio);

		}

		// W 넣기
		vPos = XMVectorSetW(vPos, 1.0f);

		// 아핀변환 행렬 세팅
		// 채널에 위 변환의 크자이 상태행렬 세팅
		_matrix TransformMat = XMMatrixAffineTransformation
		(vScale, XMVectorSet(0, 0, 0, 1.0f),vRot, vPos);

		mVectorCheannels[i]->Set_TransformationMat(TransformMat);


	}


	return S_OK;
}

CAnimation * CAnimation::Create(const char * name, _double Duration, _double TickperSecond)
{
	CAnimation*	pInstance = NEW CAnimation();

	if (FAILED(pInstance->NativeConstruct(name, Duration,TickperSecond)))
	{
		MSGBOX("Failed to Creating CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
}
