#include "BehaviorTree.h"

BEGIN(Client)
class CGameObject_3D_Dynamic;

#pragma region AI_Base


// Action_Base
class CDeco_DynamicBase
	:public CNode_Decorator

{
protected:
	explicit CDeco_DynamicBase(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CDeco_DynamicBase(const CDeco_DynamicBase& rhs);
	virtual ~CDeco_DynamicBase() = default;

public:

	virtual HRESULT ReStart(void* pArg = nullptr)override;


	void SetUp_Target(CGameObject_3D_Dynamic* targetobj)
	{
		mDynamicObject = targetobj;
	}

protected:
	CGameObject_3D_Dynamic* mDynamicObject = nullptr;

public:
	virtual void Free()override;
};

#pragma endregion AI_Base

END
