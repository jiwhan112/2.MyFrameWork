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
	meDecoID = rhs.meDecoID;
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

E_DECOTYPE CDeco_Count::IsCorect(_double timer)
{
	if (MaxCount <= 0)
		return DECOTYPE_NEXT;

	E_DECOTYPE e = DECOTYPE_NEXT;


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

CDeco_Minus::CDeco_Minus(const char * str, CGameObject_3D_Dynamic * obj)
	:CDeco_DynamicBase(str, obj)

{
	meDecoID = CDeco_DynamicBase::E_DECO_MINUS;
}

CDeco_Minus::CDeco_Minus(const CDeco_Minus & rhs)
	: CDeco_DynamicBase(rhs)

{
}

HRESULT CDeco_Minus::ReStart(void * pArg)
{
	__super::ReStart(pArg);
	

	return S_OK;
}

E_DECOTYPE CDeco_Minus::IsCorect(_double timer)
{
	if (pValue == nullptr)
		return DECOTYPE_NEXT;

	// ¸ØÃã
	if (*pValue <=0)
	{
		return E_DECOTYPE::DECOTYPE_BREAK;
	}
	
	return E_DECOTYPE::DECOTYPE_BACKFIRST;
}

CDeco_Minus * CDeco_Minus::Create(const char * str, CGameObject_3D_Dynamic * obj)
{
	CDeco_Minus* pInstance = NEW CDeco_Minus(str, obj);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CDeco_Minus");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CDeco_Minus * CDeco_Minus::Clone()
{
	CDeco_Minus* pInstance = NEW CDeco_Minus(*this);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CDeco_Minus");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDeco_Minus::Free()
{
	__super::Free();
	
}

CDeco_Distance::CDeco_Distance(const char * str, CGameObject_3D_Dynamic * obj)
	:CDeco_DynamicBase(str, obj)

{
	meDecoID = CDeco_DynamicBase::E_DECO_DISTANCE;

}

CDeco_Distance::CDeco_Distance(const CDeco_Distance & rhs)
	: CDeco_DynamicBase(rhs)

{
}

HRESULT CDeco_Distance::ReStart(void * pArg)
{

	__super::ReStart(pArg);
	return S_OK;
}

E_DECOTYPE CDeco_Distance::IsCorect(_double timer)
{

	CGameObject_3D_Dynamic* target = mDynamicObject->Get_AttackTarget();
	if (target == nullptr)
		return DECOTYPE_NEXT;

	_float3 a = mDynamicObject->Get_WorldPostition();
	_float3 b = target->Get_WorldPostition();
	a.y = b.y = 0;

	float distance = _float3::Distance(a, b);
	float disMax = 2.5f;
	if (target->Get_UnitType() == UNIT_BOSS)
		disMax = 3.5f;
	// °Å¸® Ã¼Å©
	if (distance <= disMax)
	{
		return E_DECOTYPE::DECOTYPE_NEXT;
	}
	return E_DECOTYPE::DECOTYPE_BACKFIRST;
}

CDeco_Distance * CDeco_Distance::Create(const char * str, CGameObject_3D_Dynamic * obj)
{
	CDeco_Distance* pInstance = NEW CDeco_Distance(str, obj);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CDeco_Distance");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CDeco_Distance * CDeco_Distance::Clone()
{
	CDeco_Distance* pInstance = NEW CDeco_Distance(*this);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CDeco_Distance");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDeco_Distance::Free()
{
	__super::Free();


}
