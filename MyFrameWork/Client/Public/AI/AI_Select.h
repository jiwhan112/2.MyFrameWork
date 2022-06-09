#include "BehaviorTree.h"

BEGIN(Client)
class CGameObject_3D_Dynamic;

#pragma region AI_Base

class CSelect_DynamicBase
	:public CNode_Selector

{
protected:
	explicit CSelect_DynamicBase(const char* str, CGameObject_3D_Dynamic* obj);
	explicit CSelect_DynamicBase(const CSelect_DynamicBase& rhs);
	virtual ~CSelect_DynamicBase() = default;

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