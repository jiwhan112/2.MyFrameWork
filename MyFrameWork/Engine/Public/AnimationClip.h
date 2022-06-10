#pragma once

#include "Base.h"

/* �ϳ��� �ִϸ��̼� */

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

	/* ��ü����ð�. */
	_double		m_Duration = 0.0;

	/* �ð��� �ִϸ��̼� ��� �ӵ�. */
	_double		m_TickPerSecond = 0.0;

	_double		m_PlayTimeAcc = 0.0;

	/* �� �ִԿ��Ӽ��� ����ϴµ� ����ϴ� ���� ����. */
	_uint		m_iNumChannels = 0;

	/* ���� �ִϸ��̼��� ǥ���ϱ����� ����ϴ� ����. */
	vector<class CChannel*>			m_Channels;
	typedef vector<class CChannel*>	CHANNELS;

	_bool		m_IsFinished = false;
	_bool		m_IsFrame = false;


public:
	static CAnimationClip* Create(const char* pName, _double Duration, _double TickPerSecond);
	virtual void Free() override;
};

END