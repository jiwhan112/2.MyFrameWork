#include "BehaviorTree.h"

BEGIN(Client)
class CGameObject_3D_Dynamic;

#pragma region AI_Base


// Action_Base
class CDeco_DynamicBase
	:public CNode_Decorator

{
public:
	enum E_DecoID
	{
		E_DECI_MINUS,
		E_DECI_END
	};


protected:
	explicit CDeco_DynamicBase(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CDeco_DynamicBase(const CDeco_DynamicBase& rhs);
	virtual ~CDeco_DynamicBase() = default;

public:

	virtual HRESULT ReStart(void* pArg = nullptr)override;
	virtual E_DECOTYPE IsCorect(_double timer) = 0;


	void SetUp_Target(CGameObject_3D_Dynamic* targetobj)
	{
		mDynamicObject = targetobj;
	}

	E_DecoID Get_DECOID() const { return meDecoID; };


protected:
	E_DecoID meDecoID = E_DECI_END;
	CGameObject_3D_Dynamic* mDynamicObject = nullptr;

public:
	virtual void Free()override;
};

#pragma endregion AI_Base

#pragma region DECO

// ���� üũ

class CDeco_Count
	:public CDeco_DynamicBase
{
public:
	struct DECO
	{

	};
	/*
	enum E_DECOTYPE
	{
		DECOTYPE_NEXT,
		DECOTYPE_PREV,
		DECOTYPE_BREAK,
		DECOTYPE_BACKFIRST,
		DECOTYPE_END,
	};	

	*/

protected:
	explicit CDeco_Count(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CDeco_Count(const CDeco_Count& rhs);
	virtual ~CDeco_Count() = default;

public:
	virtual HRESULT ReStart(void* pArg = nullptr)override;
	virtual E_DECOTYPE IsCorect(_double timer);

protected:
	int MaxCount = -1;
	int CurrentCont = 0;


public:
	static	CDeco_Count*				Create(const char* str, CGameObject_3D_Dynamic* obj);
	virtual CDeco_Count*				Clone()override;
	virtual void Free()override;
};


// ���� üũ
class CDeco_Minus
	:public CDeco_DynamicBase
{
protected:
	explicit CDeco_Minus(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CDeco_Minus(const CDeco_Minus& rhs);
	virtual ~CDeco_Minus() = default;

public:
	virtual HRESULT ReStart(void* pArg = nullptr)override;
	virtual E_DECOTYPE IsCorect(_double timer) override;

	void Set_Value(int i) { Value = i; }
protected:
	int Value = 0;


public:
	static	CDeco_Minus*				Create(const char* str, CGameObject_3D_Dynamic* obj);
	virtual CDeco_Minus*				Clone()override;
	virtual void Free()override;
};




#pragma endregion DECO



END
