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


// �̵� ��ġ�� ������ �ش� �׺�޽ø� Ž���ؼ� �̵����ش�.
// Run
class CAction_MOVE
	:public CAction_DynamicBase
{
public:
	// �����̴� �ִϸ��̼�
	enum E_MOVEFLAG
	{
		MOVE_WALK_ANI,
		MOVE_RUN_ANI,
		MOVE_END,

	};

	// �����̴� ��ġ
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
	virtual HRESULT NativeConstruct();
	virtual HRESULT Action(_double timer/*,void* pArg = nullptr*/);
	void Set_AniType(E_MOVEFLAG anitype)
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


private:
	_double						mTimeMax;
	_double						mCurrentTimer;

	_float3						mGoalPosition;
	_float3						mNextGoalPosition;

	_float3						mStartPosition;

	bool						mIsMoveNaviPath = false;
	bool						mIsMoveCell = false;

	E_MOVEFLAG					meMoveAni = MOVE_WALK_ANI;
	E_MOVEPOSFLAG				meMoveType = MOVE_POS_NEAR;

public:
	static	CAction_MOVE*				Create(const char* str, CGameObject_3D_Dynamic* obj);
	virtual CAction_MOVE*				Clone(void* pArg = nullptr) override;
	virtual void Free()override;
};

// ������ �Լ� ����
class CAction_Function
	:public CAction_DynamicBase
{
public:
	enum E_FUNCION
	{
		FUNCION_NONE,
		FUNCION_REMOVE_TILE,
		FUNCION_LOOK,
		FUNCION_END,

	};

protected:
	explicit CAction_Function(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CAction_Function(const CAction_Function& rhs);
	virtual ~CAction_Function() = default;


public:
	virtual HRESULT NativeConstruct();
	virtual HRESULT Action(_double timer/*,void* pArg = nullptr*/);
	
	void Set_Funcion(E_FUNCION e) { meFuncion = e; }

private:
	E_FUNCION meFuncion = FUNCION_NONE;


public:
	static	CAction_Function*				Create(const char* str, CGameObject_3D_Dynamic* obj);
	virtual CAction_Function*				Clone(void* pArg = nullptr) override;
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
//	// CNode_Decorator��(��) ���� ��ӵ�
//	virtual E_DECOTYPE IsCorect(_double timer) override;
//
//};

#pragma endregion DECO

END