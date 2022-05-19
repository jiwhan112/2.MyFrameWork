#include "stdafx.h"
#include "GameObject/GameObject_Mine.h"

CGameObject_Mine::CGameObject_Mine(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_MINE;
}

CGameObject_Mine::CGameObject_Mine(const CGameObject_Mine& rhs)
	: CGameObject_3D_Dynamic(rhs)
{
	
}

HRESULT CGameObject_Mine::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());


	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_Mine::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	mComModel->SetUp_AnimIndex(0);
//	meAI = CGameObject_Mine::BASEAI_IDLE;

//	mComNaviMesh->Set_NaviObjType(CNavigation::NAVI_OBJTYPE_PLAYER);

	return S_OK;
}


//void CGameObject_Mine::GOMOVE(_double delta)
//{
//	switch (meAI)
//	{
//	case Client::CGameObject_Mine::BASEAI_IDLE:
//		mComModel->SetUp_AnimIndex(25);
//		break;
//	case Client::CGameObject_Mine::BASEAI_MOVE:
//	{
//		mComTransform->LookAt(mGoalPosition);
//
//		mComModel->SetUp_AnimIndex(29);
//		mTimer += delta;
//		_float3 CurrentPos = _float3::Lerp(mStartPosition, mGoalPosition, mTimer / mTimeMax);
//		_float4 CurrentPos4;
//		CurrentPos4 = CurrentPos;
//		CurrentPos4.w = 1;
//
//		if (mComNaviMesh->Move_OnNavigation(CurrentPos4))
//		{
//			mComTransform->Set_State(CTransform::STATE_POSITION, CurrentPos4);
//		}
//		else
//			meAI = CGameObject_Mine::BASEAI_IDLE;
//
//		//	_float distance =  _float3::Distance(CurrentPos, mGoalPosition);
//
//		if (mTimer > mTimeMax)
//			meAI = CGameObject_Mine::BASEAI_IDLE;
//	}
//
//	break;
//	case Client::CGameObject_Mine::BASEAI_END:
//		break;
//	default:
//		break;
//	}
//}

CGameObject_Mine * CGameObject_Mine::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Mine*	pInstance = NEW CGameObject_Mine(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Mine");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Mine* CGameObject_Mine::Clone(void* pArg)
{
	CGameObject_Mine*	pInstance = NEW CGameObject_Mine(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Mine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Mine::Free()
{
	__super::Free();

	Safe_Release(mComModel);

	for (auto& obj: *mComListCollider)
	{
		Safe_Release(obj);
	}
	mComListCollider->clear();
	Safe_Delete(mComListCollider);

	
	//	Safe_Release(mComNaviMesh);
}