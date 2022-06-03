#include "BehaviorTree.h"

BEGIN(Client)
class CGameObject_3D_Dynamic;

#pragma region AI_Base


// Action_Base
class CAction_DynamicBase
	:public CNode_Action

{
protected:
	explicit CAction_DynamicBase(const char* str, CGameObject_3D_Dynamic* obj);
	virtual ~CAction_DynamicBase() = default;

public:
	virtual HRESULT					NativeConstruct()override;
	virtual CAction_DynamicBase*	Clone(void* pArg = nullptr) = 0;
	virtual HRESULT					Action(_double timer/*,void* pArg = nullptr*/) = 0;

	HRESULT	SetUp_Target(CGameObject_3D_Dynamic* targetobj)
	{
		mDynamicObject = targetobj;
	}

protected:
	CGameObject_3D_Dynamic* mDynamicObject = nullptr;

public:
	virtual void Free()override;
};

// Decoator_Base
class CDecorator_DynamicBase
	:public CNode_Decorator

{
protected:
	explicit CDecorator_DynamicBase(const char* str, CGameObject_3D_Dynamic* obj);
	virtual ~CDecorator_DynamicBase() = default;

public:
	virtual HRESULT									NativeConstruct() = 0;
	virtual CDecorator_DynamicBase*					Clone(void* pArg = nullptr) = 0;
	virtual E_DECOTYPE								IsCorect(_double timer/*,void* pArg = nullptr*/) = 0;

public:
	HRESULT	SetUp_Target(CGameObject_3D_Dynamic* targetobj)
	{
		mDynamicObject = targetobj;
	}

private:
	CGameObject_3D_Dynamic* mDynamicObject = nullptr;

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
	// 생성 초기화
	HRESULT NativeConstruct_Action();	
public:
	virtual HRESULT NativeConstruct();
	virtual HRESULT Action(_double timer/*,void* pArg = nullptr*/);
	void Set_Animation(CAnimatior::E_COMMON_ANINAME e);
	void Set_TimeMax(_double timeMax);

private:
	_double	mTimeMax;
	_double	mCurrentTimer;

	E_DEALY_FALG					meDealyType = DEALY_NONE;
	CAnimatior::E_COMMON_ANINAME	meAnimation = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_IDLE;

public:
	static	CAction_DEALY*				Create(const char* str, CGameObject_3D_Dynamic* obj);
	virtual CAction_DEALY*				Clone(void* pArg = nullptr) override;
	virtual void Free()override;
};


// 이동 위치만 받으면 해당 네비메시를 탐색해서 이동해준다.
// Run
class CAction_MOVE
	:public CAction_DynamicBase
{
public:
	enum E_MOVEFLAG
	{
		MOVE_WALK_ANI,
		MOVE_RUN_ANI,
		MOVE_END,

	};

protected:
	explicit CAction_MOVE(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CAction_MOVE(const CAction_MOVE& rhs);
	virtual ~CAction_MOVE() = default;

public:
	// 생성 초기화
	HRESULT NativeConstruct_Action(_float3 GoalPostition, _double TimeMax);
public:
	virtual HRESULT NativeConstruct();
	virtual HRESULT Action(_double timer/*,void* pArg = nullptr*/);
	void Set_AniType(E_MOVEFLAG anitype)
	{
		meMoveAni = anitype;
	}


private:
	_double						mTimeMax;
	_double						mCurrentTimer;

	_float3						mGoalPosition;
	_float3						mNextGoalPosition;

	_float3						mStartPosition;

	bool						mIsMoveNaviPath = false;
	bool						mIsMoveCell = false;

	E_MOVEFLAG					meMoveAni = MOVE_WALK_ANI;

public:
	static	CAction_MOVE*				Create(const char* str, CGameObject_3D_Dynamic* obj, _float3 goalpos, _double TimeMax);
	virtual CAction_MOVE*				Clone(void* pArg = nullptr) override;
	virtual void Free()override;
};

#pragma endregion ACTIONS

#pragma region DECO

//class CDecorator_INT
//	:public CNode_Decorator
//{
//	virtual ~CDecorator_INT() = default;
//
//public:
//	// CNode_Decorator을(를) 통해 상속됨
//	virtual E_DECOTYPE IsCorect(_double timer) override;
//
//};

#pragma endregion DECO

END