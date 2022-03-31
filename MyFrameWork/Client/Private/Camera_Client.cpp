#include "stdafx.h"
#include "Camera_Client.h"

CCamera_Client::CCamera_Client(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CCamera(pDevice,pDeviceContext)
{

}

CCamera_Client::CCamera_Client(const CCamera_Client & rhs)
	:CCamera(rhs)
{
}



HRESULT CCamera_Client::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Client::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	return S_OK;
}


_int CCamera_Client::Tick(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		mComTransform->GO_Straight(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
		mComTransform->GO_Backward(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		mComTransform->GO_Left(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
		mComTransform->GO_Right(TimeDelta);
	}

	_long		MouseMove = 0;

	const _float MouseSpeed = 1.0f * 0.1f;
	if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
	{
		mComTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * MouseSpeed);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
	{
		mComTransform->Turn(mComTransform->GetState(CTransform::STATE_RIGHT), TimeDelta * MouseMove *MouseSpeed);
	}

	return __super::Tick(TimeDelta);

}

_int CCamera_Client::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	return UPDATENONE;
}

HRESULT CCamera_Client::Render()
{
	FAILED_CHECK(__super::Render());
	return S_OK;
}


CCamera_Client * CCamera_Client::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CCamera_Client*	pInstance = DBG_NEW CCamera_Client(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CCamera_Client");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera_Client * CCamera_Client::Clone(void * pArg)
{
	CCamera_Client*	pInstance = DBG_NEW CCamera_Client(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCamera_Client");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Client::Free()
{
	__super::Free();
	
}
