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
	// �� ä���� Ű������ ������ �ִϸ��̼� ���� ������Ʈ
	// Ű ������ ���� 

	// ���� �ִϸ��̼� �ð� = �ִϸ��̼� ��� * �ð�
	mPlayTimeAcc += mTickPerAniTimeScale * TimeDelta;

	// �ִϸ��̼� �ð����� Finish üũ
	if (mPlayTimeAcc >= mDuration)
	{
		mPlayTimeAcc = 0;
		misFinished = true;
	}
	
	// �� �ִϸ��̼��� Ű������ ������ ������Ʈ
	_vector vScale, vRot, vPos;

	// ������ �ִ� ä�� ���� ��ŭ ����
	for (_uint i = 0; i<mNumChannels; ++i)
	{
		// �� ä���� Ű������ ������ �����´�.
		const vector<KEYFRAME*>* pVecKey = mVectorCheannels[i]->Get_VecKeyFrame();
		if (pVecKey == nullptr)
			return E_FAIL;

	
		_uint NumKeyFrame = pVecKey->size();
		_uint CurrentKeyFrameIndex = mVectorCheannels[i]->Get_CurrentKeyFrame();

		// ������ Ű������ ó��
		if (mPlayTimeAcc >= (*pVecKey)[NumKeyFrame - 1]->Time)
		{
			vScale = XMLoadFloat3(&(*pVecKey)[NumKeyFrame - 1]->vScale);
			vRot = XMLoadFloat4(&(*pVecKey)[NumKeyFrame - 1]->vRotation);
			vPos = XMLoadFloat3(&(*pVecKey)[NumKeyFrame - 1]->vPosition);
		}
		else
		{
			// ���� �����Ӱ� ���� �������� �����ͼ� ���� �������� �˾ƿ���

			// ���� �ð��� ���������� ���� ũ�ٸ� ���� ���������� ��������.
			// �ð��� ũ�� ���´ٸ� �� �������������α��� �̵��� �� ����
			while(mPlayTimeAcc >= (*pVecKey)[CurrentKeyFrameIndex + 1]->Time)
			{
				mVectorCheannels[i]->Set_CurrentKeyFrame(++CurrentKeyFrameIndex);
			}

			// ���� �ִϸ��̼� ���� �ð� ���� = (����ð�) / (�ѽð�)
			// ���� = (���� �����ӽð�) / (���������ӽð� - ���������ӽð�)
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

			// ���� Ű������ ��ġ ~ ���� Ű������ ��ġ �����Ѵ�.
			// ȸ���� ���ʹϾ� �����Լ��� ����Ѵ�.
			vScale = XMVectorLerp(srcScale, dstScale, Ratio);
			vRot = XMQuaternionSlerp(srcScale, dstScale, Ratio);
			vPos = XMVectorLerp(srcPos, dstPos, Ratio);

		}

		// W �ֱ�
		vPos = XMVectorSetW(vPos, 1.0f);

		// ���ɺ�ȯ ��� ����
		// ä�ο� �� ��ȯ�� ũ���� ������� ����
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
