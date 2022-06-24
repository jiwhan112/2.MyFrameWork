#pragma once

#include "GameObject_3D_Dynamic.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

// 마인 AI 설정
class CGameObject_BOSS final :
	public CGameObject_3D_Dynamic
{
public:

protected:
	explicit CGameObject_BOSS(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_BOSS(const CGameObject_BOSS& rhs);
	virtual ~CGameObject_BOSS() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual HRESULT Tick_World(_double TimeDelta);
	virtual HRESULT LateTick_World(_double TimeDelta);


	virtual HRESULT Init_Unit();
	virtual HRESULT Init_AI()override;
	virtual HRESULT Init_AI_Boss();

	HRESULT	Check_WorldPattern(_double Timer);


//	void Select_Idle();
	void Select_Move();
	void Select_Warrior(CAnimatior::E_COMMON_ANINAME type = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_SKILL,int index=-1); // Heal 사용
	void Select_Warrior_();


protected:
	_float3 mStartPostition = _float3();

private:
	_double mPatternTimer = 0;
	_double mPatternTimerMax = 5;

	_bool mIsWarriorSound = false;
	_double mWarriorSoundTime = 0;

public:
	static CGameObject_BOSS* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_BOSS* Clone(void* pArg);
	virtual void Free() override;
};

END