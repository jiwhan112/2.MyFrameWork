#include "stdafx.h"
#include "AI/AI_Deco.h"

#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_Mine.h"

CDeco_DynamicBase::CDeco_DynamicBase(const char * str, CGameObject_3D_Dynamic * obj)
	:CNode_Decorator(str), mDynamicObject(obj)

{

}

CDeco_DynamicBase::CDeco_DynamicBase(const CDeco_DynamicBase & rhs)
	:CNode_Decorator(rhs)

{
	mDynamicObject = rhs.mDynamicObject;
}

HRESULT CDeco_DynamicBase::ReStart(void * pArg)
{
	Init_Parent();
	return S_OK;
}

void CDeco_DynamicBase::Free()
{
	mDynamicObject = nullptr;

}
