#include "stdafx.h"
#include "Level_Logo.h"
#include "Level_Loader.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice,pDeviceContext)
{

}


HRESULT CLevel_Logo::NativeConstruct()
{
	FAILED_CHECK(__super::NativeConstruct());
	FAILED_CHECK(Ready_Prototype_GameObject());


	FAILED_CHECK(Ready_Layer_Camera(TAGLAY(LAY_CAMERA)));
	FAILED_CHECK(Ready_Layer_BackGround(TAGLAY(LAY_BACKGROUND)));

	return S_OK;
}

_int CLevel_Logo::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	return 0;
}

_int CLevel_Logo::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance*	pGameInstance = GetSingle(CGameInstance);

		FAILED_CHECK(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loader::Create(m_pDevice, m_pDeviceContext, LEVEL_GAMEPLAY)));
	}

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance*	pGameInstance = GetSingle(CGameInstance);

		FAILED_CHECK(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loader::Create(m_pDevice, m_pDeviceContext, LEVEL_TOOL)));
	}


	return 0;
}

HRESULT CLevel_Logo::Render()
{
	FAILED_CHECK(__super::Render());

#ifdef  _DEBUG
	SetWindowText(g_hWnd, TEXT("LOGO Level"));

#endif //  _DEBUG



	return S_OK;
}

HRESULT CLevel_Logo::Ready_Prototype_GameObject()
{

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
	
}
