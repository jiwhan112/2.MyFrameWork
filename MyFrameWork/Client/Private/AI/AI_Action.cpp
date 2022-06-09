#include "stdafx.h"
#include "AI/AI_Action.h"

#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_Mine.h"

CAction_DynamicBase::CAction_DynamicBase(const char * str, CGameObject_3D_Dynamic * obj)
	:CNode_Action(str), mDynamicObject(obj)
{
}

CAction_DynamicBase::CAction_DynamicBase(const CAction_DynamicBase & rhs)
	:CNode_Action(rhs)
{
	mDynamicObject = rhs.mDynamicObject;
}

HRESULT CAction_DynamicBase::ReStart(void* pArg)
{
	Init_Parent();
	return S_OK;
}

void CAction_DynamicBase::Free()
{
	mDynamicObject = nullptr;
}

//CDecorator_DynamicBase::CDecorator_DynamicBase(const char * str, CGameObject_3D_Dynamic * obj)
//	:CNode_Decorator(str), mDynamicObject(obj)
//
//{
//
//}
//
//void CDecorator_DynamicBase::Free()
//{
//	mDynamicObject = nullptr;
//}



CAction_DEALY::CAction_DEALY(const char * str, CGameObject_3D_Dynamic * obj)
	:CAction_DynamicBase(str, obj)
{}

CAction_DEALY::CAction_DEALY(const CAction_DEALY & rhs)
	: CAction_DynamicBase(rhs)
{
	meDealyType = rhs.meDealyType;
	meAnimation = rhs.meAnimation;
	mTimeMax = rhs.mTimeMax;

}

HRESULT CAction_DEALY::ReStart(void* pArg)
{
	if (mDynamicObject == nullptr)
		return S_FALSE;

	__super::ReStart(pArg);
	mCurrentTimer = 0;

	if (meDealyType == CAction_DEALY::DEALY_ANI)
	{
		mDynamicObject->Set_AniEnum(meAnimation);
	}

	return S_OK;
}

HRESULT CAction_DEALY::Action(_double timer)
{
	mCurrentTimer += timer;

	if (meDealyType == CAction_DEALY::DEALY_NONE)
	{
		if (mCurrentTimer > mTimeMax)
		{
			End_Succed();
			return S_OK;
		}
	}
	else if (meDealyType == CAction_DEALY::DEALY_ANI)
	{
		_bool aniend = mDynamicObject->Get_ComModel()->Get_Animaitor()->Get_CurrentAniEnd();
		if (aniend)
		{
			End_Succed();
			return S_OK;
		}
	}
	return S_OK;
}

void CAction_DEALY::Set_Animation(CAnimatior::E_COMMON_ANINAME e)
{
	meDealyType = CAction_DEALY::DEALY_ANI;
	meAnimation = e;
}

void CAction_DEALY::Set_TimeMax(_double timeMax)
{
	mTimeMax = timeMax;
}

CAction_DEALY * CAction_DEALY::Create(const char * str, CGameObject_3D_Dynamic* obj)
{
	CAction_DEALY* pInstance = NEW CAction_DEALY(str, obj);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_DEALY * CAction_DEALY::Clone()
{
	CAction_DEALY* pInstance = NEW CAction_DEALY(*this);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAction_DEALY::Free()
{
	__super::Free();
	
}

CAction_MOVE::CAction_MOVE(const char * str, CGameObject_3D_Dynamic * obj)
	:CAction_DynamicBase(str, obj)

{
}

CAction_MOVE::CAction_MOVE(const CAction_MOVE & rhs)
	: CAction_DynamicBase(rhs)
{
	mTimeMax = rhs.mTimeMax;
	meMoveAni = rhs.meMoveAni;
	meMoveType = rhs.meMoveType;

}

HRESULT CAction_MOVE::ReStart(void* pArg)
{
	if (mDynamicObject == nullptr)
		return S_FALSE;

	__super::ReStart(pArg);


	// 목표위치 설정
	// 1. 랜덤
	// 2. 특정 위치
	// 3. 마우스 위치
	switch (meMoveType)
	{
	case Client::CAction_MOVE::MOVE_POS_NEAR:
		mDynamicObject->FindPathRandAblePostition(3, &mGoalPosition);
		mGoalPosition = mDynamicObject->Get_PathGoalPostition();
		break;
	case Client::CAction_MOVE::MOVE_POS_TILE:
		// 이미 탐색된 정보 활용
		mGoalPosition = mDynamicObject->Get_PathGoalPostition();
		break;
	case Client::CAction_MOVE::MOVE_POS_PICK:
		mGoalPosition = GetSingle(CGameManager)->Get_PickPos();
		mDynamicObject->FindPathForCurrentNavi(mGoalPosition);
		break;
	case Client::CAction_MOVE::MOVE_POS_END:
		break;
	default:
		break;
	}

	mIsMoveNaviPath = true;
	mIsMoveCell = false;
	mCurrentTimer = 0;

	return S_OK;
}

HRESULT CAction_MOVE::Action(_double TimeDelta)
{
	// 패스 이동 후 End 체크
	if (mIsMoveNaviPath)
	{
		if (mIsMoveCell)
		{
			// 애니메이션 결정
			if (meMoveAni == CAction_MOVE::MOVE_ANI_RUN)
				mDynamicObject->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_RUN,0);

			else if (meMoveAni == CAction_MOVE::MOVE_ANI_WALK)
				mDynamicObject->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_WALK,0);

			// _float3 newLook = _float3(mNextGoalPosition.x, mStartPosition.y, mNextGoalPosition.z);
			// mDynamicObject->Get_ComTransform()->LookAt(newLook);
			mDynamicObject->Set_LookPos(mNextGoalPosition);

			mCurrentTimer += TimeDelta;
			_float3 CurrentPosition = GetSingle(CGameInstance)->Easing3(TYPE_Linear, mStartPosition, mNextGoalPosition, mCurrentTimer, mTimeMax);

			if (mCurrentTimer > mTimeMax)
			{
				mIsMoveCell = false;
			}
			if (_float3::Distance(CurrentPosition, mNextGoalPosition) < 0.3f)
			{
				mIsMoveCell = false;
			}
			mDynamicObject->Set_Position(CurrentPosition);
		}

		else
		{
			// 셀 설정
			if (mDynamicObject->Get_PathList_Frontpop(&mNextGoalPosition))
			{
				mStartPosition = mDynamicObject->Get_WorldPostition();
				mCurrentTimer = 0;
				mIsMoveCell = true;
			}

			else
			{
				mIsMoveNaviPath = false;
				End_Succed();
			//	mDynamicObject->Set_Position(mGoalPosition);
				// mDynamicObject->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE);

			}
		}
		return S_OK;
	}
}

CAction_MOVE * CAction_MOVE::Create(const char * str, CGameObject_3D_Dynamic * obj)
{
	CAction_MOVE* pInstance = NEW CAction_MOVE(str, obj);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_MOVE * CAction_MOVE::Clone()
{
	CAction_MOVE* pInstance = NEW CAction_MOVE(*this);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAction_MOVE::Free()
{
	__super::Free();
	
}

CAction_Function::CAction_Function(const char * str, CGameObject_3D_Dynamic * obj)
	:CAction_DynamicBase(str,obj)
{
}

CAction_Function::CAction_Function(const CAction_Function & rhs)
	: CAction_DynamicBase(rhs)
	, meFuncion(rhs.meFuncion)

{
}

HRESULT CAction_Function::ReStart(void* pArg)
{
	if (mDynamicObject == nullptr)
		return S_FALSE;

	__super::ReStart(pArg);
	
	return S_OK;
}

HRESULT CAction_Function::Action(_double timer)
{
	switch (meFuncion)
	{
	case CAction_Function::FUNCION_REMOVE_TILE:
		((CGameObject_Mine*)mDynamicObject)->RemoveTile();
		break;
	case CAction_Function::FUNCION_LOOK:
		((CGameObject_Mine*)mDynamicObject)->LookTile();
	default:
		break;
	}

	End_Succed();

	return S_OK;
}

CAction_Function * CAction_Function::Create(const char * str, CGameObject_3D_Dynamic * obj)
{
	CAction_Function* pInstance = NEW CAction_Function(str, obj);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_Function * CAction_Function::Clone()
{
	CAction_Function* pInstance = NEW CAction_Function(*this);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CAction_Function::Free()
{
	__super::Free();
}

CAction_MOVE_TARGET::CAction_MOVE_TARGET(const char * str, CGameObject_3D_Dynamic * obj)
	:CAction_DynamicBase(str, obj)
	
{
	
}

CAction_MOVE_TARGET::CAction_MOVE_TARGET(const CAction_MOVE_TARGET & rhs)
	: CAction_DynamicBase(rhs)
{
	mCurrentTimer = 0;
	mTimeMax = 2;

	meEasingID = rhs.meEasingID;
//	meAniFlag = rhs.meAniFlag;
	meMoveTargetFlag = rhs.meMoveTargetFlag;
}

HRESULT CAction_MOVE_TARGET::ReStart(void* pArg)
{
	if (mDynamicObject == nullptr)
		return S_FALSE;

	__super::ReStart(pArg);
	
	mCurrentTimer = 0;
	// 타일을 설정하면 위에서 이동
	if (meMoveTargetFlag == CAction_MOVE_TARGET::MOVETARGETFALG_CREATE_FALL)
	{
		mDynamicObject->Set_IsTerrainHeight(false);
		mStartPosition = mDynamicObject->Get_WorldPostition();
		mStartPosition.y = 10;
		mGoalPosition = mDynamicObject->Get_TerrainHeightPostition();
	}
	else if (meMoveTargetFlag == CAction_MOVE_TARGET::MOVETARGETFALG_MOUSEPOS_FALL)
	{
		mDynamicObject->Set_IsTerrainHeight(false);
		mStartPosition = GetSingle(CGameManager)->Get_PickPos();
		mStartPosition.y += 5;
		mGoalPosition = mDynamicObject->Get_TerrainHeightPostition();
	}
	else if (meMoveTargetFlag == CAction_MOVE_TARGET::MOVETARGETFALG_OBJECTTARGET)
	{
		mDynamicObject->Set_IsTerrainHeight(false);
		mStartPosition = mDynamicObject->Get_WorldPostition();
		mGoalPosition = mDynamicObject->Get_CustomMovePosition();
	}

	return S_OK;
}

HRESULT CAction_MOVE_TARGET::Action(_double TimeDelta)
{
	mCurrentTimer += TimeDelta;
	_float3 CurrentPosition = GetSingle(CGameInstance)->Easing3(meEasingID, mStartPosition, mGoalPosition, mCurrentTimer, mTimeMax);

	if (meMoveTargetFlag == CAction_MOVE_TARGET::MOVETARGETFALG_OBJECTTARGET)
	{
		mDynamicObject->Get_ComTransform()->LookAt(CurrentPosition);
	}
	

	if (mCurrentTimer > mTimeMax)
	{
		mDynamicObject->Set_Position(mGoalPosition);
		mDynamicObject->Set_IsTerrainHeight(true);
		End_Succed();
	}
	else
		mDynamicObject->Set_Position(CurrentPosition);
	// if (_float3::Distance(CurrentPosition, mGoalPosition) < 0.2f)
	// {
	// IsMoving = false;
	// eturn S_OK;
	// }
	return S_OK;

}


CAction_MOVE_TARGET * CAction_MOVE_TARGET::Create(const char * str, CGameObject_3D_Dynamic * obj)
{
	CAction_MOVE_TARGET* pInstance = NEW CAction_MOVE_TARGET(str,obj);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_MOVE_TARGET * CAction_MOVE_TARGET::Clone()
{
	CAction_MOVE_TARGET* pInstance = NEW CAction_MOVE_TARGET(*this);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAction_MOVE_TARGET::Free()
{
	__super::Free();
}
