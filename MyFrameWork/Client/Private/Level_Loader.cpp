#include "stdafx.h"

#include "Level_Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Level_Tool.h"

#include "Loader.h"

CLevel_Loader::CLevel_Loader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Loader::NativeConstruct(E_LEVEL eNextLevel)
{
	FAILED_CHECK(__super::NativeConstruct());

	mNextLevel = eNextLevel;
	mpLoader = CLoader::Create(m_pDevice, m_pDeviceContext, eNextLevel);

	return S_OK;
}

_int CLevel_Loader::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	return _int();
}

_int CLevel_Loader::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	if (true == mpLoader->isFinished())
	{
		CLevel*		pLevel = nullptr;

		switch (mNextLevel)
		{
		case LEVEL_GAMEPLAY:
			//	pLevel = CLevel_Logo::Create(m_pDevice, m_pDeviceContext);
			pLevel = CLevel_GamePlay::Create(m_pDevice, m_pDeviceContext);
			break;
		case LEVEL_TOOL:
			pLevel = CLevel_Tool::Create(m_pDevice, m_pDeviceContext);
			break;
		}

		FAILED_CHECK(GetSingle(CGameInstance)->OpenLevel(mNextLevel, pLevel));
	}

	return 0;
}

HRESULT CLevel_Loader::Render()
{
	FAILED_CHECK(__super::Render());

#ifdef  _DEBUG
	SetWindowText(g_hWnd, TEXT("Loading Level"));
#endif //  _DEBUG

	return S_OK;
}

CLevel_Loader * CLevel_Loader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, E_LEVEL nextLevel)
{
	CLevel_Loader*	pInstance = NEW CLevel_Loader(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(nextLevel)))
	{
		MSGBOX("Failed to Creating CLevel_Loader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loader::Free()
{
	Safe_Release(mpLoader);

	__super::Free();
}