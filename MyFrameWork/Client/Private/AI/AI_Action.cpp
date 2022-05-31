#include "stdafx.h"
#include "AI/AI_Action.h"

#include "GameObject/GameObject_3D_Dynamic.h"

CAction_DynamicBase::CAction_DynamicBase(const char * str, CGameObject_3D_Dynamic * obj)
	:CNode_Action(str), mDynamicObject(obj)
{
	Safe_AddRef(mDynamicObject);
}

void CAction_DynamicBase::Free()
{
	Safe_Release(mDynamicObject);
}

//CDecorator_DynamicBase::CDecorator_DynamicBase(const char * str, CGameObject_3D_Dynamic * obj)
//	:CNode_Decorator(str), 
//	mDynamicObject(obj)
//{
//	Safe_AddRef(mDynamicObject);
//}
//
//void CDecorator_DynamicBase::Free()
//{
//	Safe_Release(mDynamicObject);
//}


CAction_DEALY::CAction_DEALY(const char * str, CGameObject_3D_Dynamic * obj)
	:CAction_DynamicBase(str, obj)
{}


HRESULT CAction_DEALY::NativeConstruct_Action(_double TimeMax)
{
	mTimeMax = TimeMax;
	mCurrentTimer = 0;

	return S_OK;
}

HRESULT CAction_DEALY::NativeConstruct()
{
	Init_Parent();
	mCurrentTimer = 0;
	return S_OK;
}

HRESULT CAction_DEALY::Action(_double timer)
{
	mCurrentTimer += timer;
	if (mCurrentTimer > mTimeMax)
	{
		mIsEnd = true;
		mIsSucceed = true;
		return S_OK;
	}

	return S_OK;
}

CAction_DEALY * CAction_DEALY::Create(const char * str, CGameObject_3D_Dynamic* obj, _double TimeMax)
{
	CAction_DEALY* pInstance = NEW CAction_DEALY(str, obj);

	if (FAILED(pInstance->NativeConstruct_Action(TimeMax)))
	{
		MSGBOX("Failed to Created CAction_DEALY");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_DEALY * CAction_DEALY::Clone(void * pArg)
{
	CAction_DEALY* pInstance = NEW CAction_DEALY(*this);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CAction_DEALY");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAction_DEALY::Free()
{
	__super::Free();
	
}
