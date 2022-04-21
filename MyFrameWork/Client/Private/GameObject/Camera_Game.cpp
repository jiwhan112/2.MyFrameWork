#include "stdafx.h"
#include "GameObject/Camera_Game.h"

CCamera_Game::CCamera_Game(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CCamera(pDevice, pDeviceContext)
{
}

CCamera_Game::CCamera_Game(const CCamera_Game & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Game::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Game::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	// 직교 투영 행렬 초기화 
	FAILED_CHECK(GetSingle(CGameInstance)->SetTransform(CPipeLine::D3DTS_PROJ_ORI, XMMatrixOrthographicLH((float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f)));
	
	return S_OK;
}

_int CCamera_Game::Tick(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		mComTransform->GO_WorldVec(_float3(0,0,1),45, CTransform::ROTTYPE_Y, TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		mComTransform->GO_WorldVec(_float3(0, 0, -1), 45, CTransform::ROTTYPE_Y, TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		mComTransform->GO_Left(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		mComTransform->GO_Right(TimeDelta);
	}

	return __super::Tick(TimeDelta);

	


}

_int CCamera_Game::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	return UPDATENONE;
}

HRESULT CCamera_Game::Render()
{
	FAILED_CHECK(__super::Render());
	return S_OK;
}

CCamera_Game * CCamera_Game::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CCamera_Game*	pInstance = NEW CCamera_Game(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CCamera_Game");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera_Game * CCamera_Game::Clone(void * pArg)
{
	CCamera_Game*	pInstance = NEW CCamera_Game(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCamera_Game");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Game::Free()
{
	__super::Free();
}