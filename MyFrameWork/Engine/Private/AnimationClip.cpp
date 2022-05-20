#include "..\Public\AnimationClip.h"
#include "Channel.h"

CAnimationClip::CAnimationClip()
{
}

const char * CAnimationClip::Get_Name()
{
	return m_szName;
}

void CAnimationClip::Set_AniMationTime(_double timer)
{
	if (m_Duration <= timer)
		m_PlayTimeAcc = m_Duration;

	m_PlayTimeAcc = timer;
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i]->Set_CurrentKeyFrame(m_PlayTimeAcc);
	}

}

HRESULT CAnimationClip::NativeConstruct(const char* pName, _double Duration, _double TickPerSecond)
{
	strcpy_s(m_szName, pName);

	m_Duration = Duration;
	m_TickPerSecond = TickPerSecond;

	return S_OK;
}

HRESULT CAnimationClip::Update_TransformMatrices(_double TimeDelta)
{
	/* 현재 내 애니메이션의 재생 위치. */
	m_PlayTimeAcc += m_TickPerSecond * TimeDelta;

	if (m_PlayTimeAcc >= m_Duration)
	{
		m_PlayTimeAcc = 0.0;
		m_isFinished = true;
	}
	else
		m_isFinished = false;

	_vector			vScale, vRotation, vPosition;

	/* 현재 내 애니메이션 상태에서 재생된 시간에 따른 모든 뼈의 상태를 갱신한다.  */
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		if (true == m_isFinished)
			m_Channels[i]->Set_CurrentKeyFrame(0);

		/* 각각의 뼈들이 시간값에 따른 상태값을 표현한 키프레임들을 가져온다. */
		const vector<KEYFRAME*>*	pKeyFrames = m_Channels[i]->Get_KeyFrames();
		if (nullptr == pKeyFrames)
			return E_FAIL;

		_uint iNumKeyFrame = pKeyFrames->size();

		_uint iCurrentKeyFrameIndex = m_Channels[i]->Get_CurrentKeyFrame();

		/* 마지막 키프레임을 넘어가면. */
		if (m_PlayTimeAcc >= (*pKeyFrames)[iNumKeyFrame - 1]->Time)
		{
			vScale = XMLoadFloat3(&(*pKeyFrames)[iNumKeyFrame - 1]->vScale);
			vRotation = XMLoadFloat4(&(*pKeyFrames)[iNumKeyFrame - 1]->vRotation);
			vPosition = XMLoadFloat3(&(*pKeyFrames)[iNumKeyFrame - 1]->vPosition);
		}
		/* 특정 키프레임 두개 사이에 존재할때?! */
		else
		{
			while (m_PlayTimeAcc >= (*pKeyFrames)[iCurrentKeyFrameIndex + 1]->Time)
			{
				m_Channels[i]->Set_CurrentKeyFrame(++iCurrentKeyFrameIndex);
			}

			_double		Ratio = (m_PlayTimeAcc - (*pKeyFrames)[iCurrentKeyFrameIndex]->Time)
				/ ((*pKeyFrames)[iCurrentKeyFrameIndex + 1]->Time - (*pKeyFrames)[iCurrentKeyFrameIndex]->Time);

			_vector		vSourScale, vDestScale;
			_vector		vSourRotation, vDestRotation;
			_vector		vSourPosition, vDestPosition;

			vSourScale = XMLoadFloat3(&(*pKeyFrames)[iCurrentKeyFrameIndex]->vScale);
			vSourRotation = XMLoadFloat4(&(*pKeyFrames)[iCurrentKeyFrameIndex]->vRotation);
			vSourPosition = XMLoadFloat3(&(*pKeyFrames)[iCurrentKeyFrameIndex]->vPosition);

			vDestScale = XMLoadFloat3(&(*pKeyFrames)[iCurrentKeyFrameIndex + 1]->vScale);
			vDestRotation = XMLoadFloat4(&(*pKeyFrames)[iCurrentKeyFrameIndex + 1]->vRotation);
			vDestPosition = XMLoadFloat3(&(*pKeyFrames)[iCurrentKeyFrameIndex + 1]->vPosition);

			vScale = XMVectorLerp(vSourScale, vDestScale, _float(Ratio));
			vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, _float(Ratio));
			vPosition = XMVectorLerp(vSourPosition, vDestPosition, _float(Ratio));
		}

		vPosition = XMVectorSetW(vPosition, 1.f);

		_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		m_Channels[i]->Set_TransformationMatrix(TransformationMatrix);
	}

	return S_OK;
}

CAnimationClip * CAnimationClip::Create(const char* pName, _double Duration, _double TickPerSecond)
{
	CAnimationClip*	pInstance = new CAnimationClip();

	if (FAILED(pInstance->NativeConstruct(pName, Duration, TickPerSecond)))
	{
		MSGBOX("Failed to Created CAnimationClip");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimationClip::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}