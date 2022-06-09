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

CDeco_Count::CDeco_Count(const char * str, CGameObject_3D_Dynamic * obj)
	:CDeco_DynamicBase(str,obj)
{

}

CDeco_Count::CDeco_Count(const CDeco_Count & rhs)
	: CDeco_DynamicBase(rhs)
{

}

HRESULT CDeco_Count::ReStart(void * pArg)
{
	MaxCount = 10;
	   
	return S_OK;
}

CDeco_DynamicBase::E_DECOTYPE CDeco_Count::IsCollect(_double timer)
{
	if (MaxCount <= 0)
		return DECOTYPE_NEXT;

	E_DECOTYPE e = CNode_Decorator::DECOTYPE_PREV;


	return e;
}

CDeco_Count * CDeco_Count::Create(const char * str, CGameObject_3D_Dynamic * obj)
{
	return nullptr;
}

CDeco_Count * CDeco_Count::Clone()
{
	return nullptr;
}

void CDeco_Count::Free()
{
	__super::Free();
	
}
