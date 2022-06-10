#pragma once

#include "Base.h"

/* 하나의 애니메이션 */

BEGIN(Engine)

class ENGINE_DLL CAnimationClip final : public CBase
{
private:
	CAnimationClip();
	virtual ~CAnimationClip() = default;

public:
	void Reserve(_uint iNumChannels)
	{
		m_Channels.reserve(iNumChannels);
		m_iNumChannels = iNumChannels;
	}

	const vector<class CChannel*>* Get_Channels() const
	{
		return &m_Channels;
	}
	const char* Get_Name();

	void Set_AniMationTime(_double timer);
	_double Get_AniMationTime()const { return m_PlayTimeAcc; }

	_double Get_MaxAnimaionTime()const
	{
		return m_Duration;
	}

	_bool Get_Finished() const
	{
		return m_IsFinished;
	}


public:
	HRESULT NativeConstruct(const char* pName, _double Duration, _double TickPerSecond);
	HRESULT Update_TransformMatrices(_double TimeDelta);
	HRESULT Update_TransformMatrices_OnlyTime(_double TimeDelta);
	

	void Add_Channel(class CChannel* pChannel)
	{
		m_Channels.push_back(pChannel);
	}


private:
	char		m_szName[MAX_PATH] = "";

	/* 전체재생시간. */
	_double		m_Duration = 0.0;

	/* 시간당 애니메이션 재생 속도. */
	_double		m_TickPerSecond = 0.0;

	_double		m_PlayTimeAcc = 0.0;

	/* 이 애님에ㅣ션을 재생하는데 사용하는 뼈의 갯수. */
	_uint		m_iNumChannels = 0;

	/* 현재 애니메이션을 표현하기위해 사용하는 뼈들. */
	vector<class CChannel*>			m_Channels;
	typedef vector<class CChannel*>	CHANNELS;

	_bool		m_IsFinished = false;
	_bool		m_IsFrame = false;


public:
	static CAnimationClip* Create(const char* pName, _double Duration, _double TickPerSecond);
	virtual void Free() override;
};

END