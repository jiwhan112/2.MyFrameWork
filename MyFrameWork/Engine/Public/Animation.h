#pragma once

#include "Base.h"

BEGIN(Engine)
class CChannel;

class CAnimation final : public CBase
{
public:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	virtual HRESULT NativeConstruct(const char* name, _double Duration, _double TickperSecond);
	HRESULT Update_TransformMatrices(_double TimeDelta);

public:

	void Add_Channel(CChannel* pChannel)
	{
		mVectorCheannels.push_back(pChannel);
	}
	void Reserve(_uint MaxChannel)
	{
		mVectorCheannels.reserve(MaxChannel);
		mNumChannels = MaxChannel;
	}
	void Set_TickPerAniTimeScale(_double timer) { mTickPerAniTimeScale = timer; }

private:
	// �ִϸ��̼� �̸�
	char		mSzName[MAX_PATH] = "";
	// ��ü �ð�
	_double		mDuration = 0.0;
	// ��� �ӵ�
	_double		mTickPerAniTimeScale = 0.0;
	_double		mPlayTimeAcc = 0.0;

	// ���� �޴� ���� ������ ���� ����
	_uint							mNumChannels = 0;
	vector<CChannel*>				mVectorCheannels;
	typedef vector<CChannel*>		CHANNELS;
	// EndCheck
	_bool		misFinished= false;


public:
	static CAnimation* Create(const char* name,_double Duration, _double TickperSecond);
	virtual void Free() override;
};

END