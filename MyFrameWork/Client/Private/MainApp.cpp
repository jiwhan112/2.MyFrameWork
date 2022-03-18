#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"

#include "ImguiMgr.h"


ID3D11Device*			g_D3D11Device;
ID3D11DeviceContext*	g_D3D11DeviceContext;


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

	g_D3D11Device = m_pDevice;
	g_D3D11DeviceContext = m_pDeviceContext;


	// IMGUI INIT
	GetSingle(CImguiMgr)->InitImGUI(GraphicDesc.hWnd,m_pDevice, m_pDeviceContext);
	
	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	bool btrue = true;
	ImGui::ShowDemoWindow(&btrue);


	return _int();
}

HRESULT CMainApp::Render()
{

	if (nullptr == m_pGameInstance)
		return E_FAIL;

	// ����� / ���̹��� Ŭ����

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.0f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();


	// IMGUITEST
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// ����ü��
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

	GetSingle(CImguiMgr)->DestroyInstance();

	CGameInstance::Release_Engine();
}
