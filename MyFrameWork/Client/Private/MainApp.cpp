#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"


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

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, GraphicDesc, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;	

	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	return _int();
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	// 백버퍼 / 깊이버퍼 클리어
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.0f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	// 스왑체인
	m_pGameInstance->Present();

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

	
	CGameInstance::Release_Engine();
}
