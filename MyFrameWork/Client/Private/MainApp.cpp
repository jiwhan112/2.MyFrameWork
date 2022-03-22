#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"

#include "ImguiMgr.h"

#include "Level_Logo.h"
#include "Level_Loader.h"


CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);

}

HRESULT CMainApp::NativeConstruct()
{
	CGraphic_Device::GRAPHICDESC GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = CGraphic_Device::MODE_WIN;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;


	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;	

	// IMGUI INIT
	GetSingle(CImguiMgr)->InitImGUI(GraphicDesc.hWnd,m_pDevice, m_pDeviceContext);
	

	
	FAILED_CHECK(Ready_Prototype_Components());
	FAILED_CHECK(Open_Level(LEVEL_LOGO));

	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	m_pGameInstance->Tick_Engine(TimeDelta);
	return _int();
}

HRESULT CMainApp::Render()
{

	if (nullptr == m_pGameInstance)
		return E_FAIL;

	// 백버퍼 / 깊이버퍼 클리어

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.0f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pGameInstance->Render_Level();

	// 스왑체인
	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(E_LEVEL eLevelIndex)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	HRESULT		hr = 0;

	switch (eLevelIndex)
	{
	case LEVEL_LOGO:
		hr = m_pGameInstance->OpenLevel(eLevelIndex, CLevel_Logo::Create(m_pDevice, m_pDeviceContext));
		break;
	default:
		hr = m_pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loader::Create(m_pDevice, m_pDeviceContext, eLevelIndex));
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;

}

HRESULT CMainApp::Ready_Prototype_Components()
{
	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);

	GetSingle(CImguiMgr)->DestroyInstance();

	CGameInstance::Release_Engine();
}
