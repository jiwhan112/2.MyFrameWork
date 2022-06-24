#include "stdafx.h"
#include "AI/AI_Select.h"

#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_Mine.h"

CSelect_DynamicBase::CSelect_DynamicBase(const char * str, CGameObject_3D_Dynamic * obj)
	:CNode_Selector(str), mDynamicObject(obj)

{
}

CSelect_DynamicBase::CSelect_DynamicBase(const CSelect_DynamicBase & rhs)
	:CNode_Selector(rhs)
{
	mDynamicObject = rhs.mDynamicObject;

}

HRESULT CSelect_DynamicBase::ReStart(void * pArg)
{
	Init_Parent();
	return S_OK;
}

void CSelect_DynamicBase::Free()
{
}
