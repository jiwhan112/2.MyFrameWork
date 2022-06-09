#include "BehaviorTree.h"

BEGIN(Client)
class CGameObject_3D_Dynamic;

#pragma region AI_Base


// Action_Base
class CAction_DynamicBase
	:public CNode_Action

{
public:
	enum E_AcionID
	{
		E_ACION_DEALY,
		E_ACION_MOVEPATH,
		E_ACION_MOVETARGET,
		E_ACION_FUNCTION,
		E_ACION_END,
	};

protected:
	explicit CAction_DynamicBase(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CAction_DynamicBase(const CAction_DynamicBase& rhs);
	virtual ~CAction_DynamicBase() = default;

public:
	// 중복사용으로 초기화 필요
	virtual HRESULT ReStart(void* pArg = nullptr)override;

	void SetUp_Target(CGameObject_3D_Dynamic* targetobj)
	{
		mDynamicObject = targetobj;
	}
	E_AcionID Get_ACIONID() const { return meAcionID; };

protected:
	CGameObject_3D_Dynamic* mDynamicObject = nullptr;
	E_AcionID				meAcionID = E_ACION_END;

public:
	virtual void Free()override;
};

#pragma endregion AI_Base


#pragma region ACTIONS

// Dealy
// Animation Dealy
class CAction_DEALY
	:public CAction_DynamicBase
{
public:
	enum E_DEALY_FALG
	{
		DEALY_NONE,
		DEALY_ANI,
		DEALY_END,
	};

protected:
	explicit CAction_DEALY(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CAction_DEALY(const CAction_DEALY& rhs);
	virtual ~CAction_DEALY() = default;
	
public:
	virtual HRESULT ReStart(void* pArg = nullptr)override;
	virtual HRESULT Action(_double timer/*,void* pArg = nullptr*/);
	void Set_Animation(CAnimatior::E_COMMON_ANINAME e);
	void Set_TimeMax(_double timeMax);

private:
	_double	mTimeMax=1;
	_double	mCurrentTimer=0;

	E_DEALY_FALG					meDealyType = DEALY_NONE;
	CAnimatior::E_COMMON_ANINAME	meAnimation = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_IDLE;

public:
	static	CAction_DEALY*				Create(const char* str, CGameObject_3D_Dynamic* obj);
	virtual CAction_DEALY*				Clone()override;
	virtual void Free()override;
};

// 이동 위치만 받으면 해당 네비메시를 탐색해서 이동해준다.
// Run
class CAction_MOVE
	:public CAction_DynamicBase
{
public:
	// 움직이는 애니메이션
	enum E_MOVE_ANI_FLAG
	{
		MOVE_ANI_WALK,
		MOVE_ANI_RUN,
		MOVE_ANI_END,

	};

	// 움직이는 위치
	enum E_MOVEPOSFLAG
	{
		MOVE_POS_NEAR,
		MOVE_POS_TILE,
		MOVE_POS_PICK,
		MOVE_POS_END,
	};

protected:
	explicit CAction_MOVE(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CAction_MOVE(const CAction_MOVE& rhs);
	virtual ~CAction_MOVE() = default;

public:
	virtual HRESULT ReStart(void* pArg = nullptr)override;
	virtual HRESULT Action(_double timer/*,void* pArg = nullptr*/);
	void Set_AniType(E_MOVE_ANI_FLAG anitype)
	{
		meMoveAni = anitype;
	}
	void Set_TimeMax(_float TimeMax)
	{
		mTimeMax = TimeMax;
	}
	void Set_Postition(E_MOVEPOSFLAG eMoveType)
	{
		meMoveType = eMoveType;
	}

	void Set_Easing(EasingTypeID e)
	{
		meEasingID = e;
	}

private:
	_double						mTimeMax=1;
	_double						mCurrentTimer=0;

	_float3						mGoalPosition;
	_float3						mNextGoalPosition;

	_float3						mStartPosition;

	bool						mIsMoveNaviPath = false;
	bool						mIsMoveCell = false;


	E_MOVE_ANI_FLAG				meMoveAni = MOVE_ANI_WALK;
	E_MOVEPOSFLAG				meMoveType = MOVE_POS_NEAR;
	EasingTypeID				meEasingID = TYPE_Linear;

public:
	static	CAction_MOVE*				Create(const char* str, CGameObject_3D_Dynamic* obj);
	virtual CAction_MOVE*				Clone() override;
	virtual void Free()override;
};

// 네비메시 영향 X 움직임
class CAction_MOVE_TARGET
	:public CAction_DynamicBase
{
public:

protected:
	explicit CAction_MOVE_TARGET(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CAction_MOVE_TARGET(const CAction_MOVE_TARGET& rhs);
	virtual ~CAction_MOVE_TARGET() = default;

public:
	virtual HRESULT ReStart(void* pArg = nullptr)override;
	virtual HRESULT Action(_double timer/*,void* pArg = nullptr*/);

//	void Set_AniFlag(E_ANIFLAG e) { meAniFlag = e; };
	void Set_EasingFlag(EasingTypeID e) { meEasingID = e; };
	void Set_Postition(_float3 s, _float3 g)
	{
		mStartPosition = s;
		mGoalPosition = g;
	};

	void Set_MoveTimeMax(_float timeMax) { mTimeMax = timeMax; };
private:
	_double						mTimeMax=1;
	_double						mCurrentTimer=0;

	_float3						mGoalPosition;
	_float3						mStartPosition;

	EasingTypeID				meEasingID = TYPE_Linear;

public:
	static	CAction_MOVE_TARGET*				Create(const char* str, CGameObject_3D_Dynamic* obj);
	virtual CAction_MOVE_TARGET*				Clone() override;
	virtual void Free()override;

};

// 임의의 함수 실행
class CAction_Function
	:public CAction_DynamicBase
{
public:
	enum E_FUNCION
	{
		FUNCION_NONE,
		FUNCION_REMOVE_TILE,
		FUNCION_LOOKTILE,
		FUNCION_SETGAMEMODE,
		FUNCION_END,

	};

protected:
	explicit CAction_Function(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CAction_Function(const CAction_Function& rhs);
	virtual ~CAction_Function() = default;


public:
	virtual HRESULT ReStart(void* pArg = nullptr)override;
	virtual HRESULT Action(_double timer/*,void* pArg = nullptr*/);
	
	void Set_Funcion(E_FUNCION e) { meFuncion = e; }

private:
	E_FUNCION meFuncion = FUNCION_NONE;


public:
	static	CAction_Function*				Create(const char* str, CGameObject_3D_Dynamic* obj);
	virtual CAction_Function*				Clone() override;
	virtual void Free()override;

};

#pragma endregion ACTIONS



END