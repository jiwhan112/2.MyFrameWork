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

	void Set_AniMationTime(_float timer);
	_float Get_AniMationTime()const { return m_PlayTimeAcc; }

	_float Get_MaxAnimaionTime()const
	{
		return m_Duration;
	}



public:
	HRESULT NativeConstruct(const char* pName, _double Duration, _double TickPerSecond);
	HRESULT Update_TransformMatrices(_double TimeDelta);

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

	_bool		m_isFinished = false;


public:
	static CAnimationClip* Create(const char* pName, _double Duration, _double TickPerSecond);
	virtual void Free() override;
};

END