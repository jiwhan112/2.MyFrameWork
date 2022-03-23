#include "stdafx.h"
#include "Level_GamePlay.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice,pDeviceContext)
{
	mLevelIndex = LEVEL_GAMEPLAY;
}


HRESULT CLevel_GamePlay::NativeConstruct()
{
	FAILED_CHECK(__super::NativeConstruct());

	return S_OK;
}

_int CLevel_GamePlay::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));


	return UPDATENONE;
}

_int CLevel_GamePlay::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	return UPDATENONE;
}

HRESULT CLevel_GamePlay::Render()
{
	FAILED_CHECK(__super::Render());

#ifdef  _DEBUG
	SetWindowText(g_hWnd, TEXT("Game Level"));

#endif //  _DEBUG
	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
	
}
