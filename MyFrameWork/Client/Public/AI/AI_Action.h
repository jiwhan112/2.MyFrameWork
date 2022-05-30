#include "BehaviorTree.h"

BEGIN(Client)
class CGameObject_3D_Dynamic;

// 클라에서 액션 베이스
class CAction_DynamicBase
	:public CNode_Action

{
protected:
	explicit CAction_DynamicBase(const char* str, CGameObject_3D_Dynamic* obj);
	virtual ~CAction_DynamicBase() = default;

public:
	// CNode_Action을(를) 통해 상속됨
	virtual HRESULT Action(_double timer)=0;

private:
	CGameObject_3D_Dynamic* mDynamicObject;
public:
	virtual void Free()override;
};

// 클라에서 데코 베이스
class CDecorator_DynamicBase
	:public CNode_Decorator

{
protected:
	explicit CDecorator_DynamicBase(const char* str, CGameObject_3D_Dynamic* obj);
	virtual ~CDecorator_DynamicBase() = default;

public:
	// CNode_Action을(를) 통해 상속됨
	virtual E_DECOTYPE IsCorect(_double timer)=0;

private:
	CGameObject_3D_Dynamic* mDynamicObject;
public:
	virtual void Free()override;
};

//class CAction_MOVE
//		:public CNode_Action
//{
//	virtual ~CAction_MOVE() = default;
//
//public:
//	// CNode_Action을(를) 통해 상속됨
//	virtual HRESULT Action(_double timer) override;
//
//private:
////	_double	mTimeMax;
////	_double	mCurrentTimer;
//
//};

class CAction_DEALY
	:public CAction_DynamicBase
{
protected:
	explicit CAction_DEALY(const char* str, CGameObject_3D_Dynamic* obj);
	virtual ~CAction_DEALY() = default;
	
public: // 복사 구조 안만듬
	HRESULT NativeConstruct_Action(_double timer);

	// CNode_Action을(를) 통해 상속됨
	virtual HRESULT Action(_double timer) override;

private:
	_double	mTimeMax;
	_double	mCurrentTimer;

public:
	static CAction_DEALY* Create(const char* str, CGameObject_3D_Dynamic* obj, _double TimeMax);
	virtual void Free()override;
};


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
END