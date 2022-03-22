#include "stdafx.h"
#include "Level_Tool.h"

CLevel_Tool::CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice,pDeviceContext)
{

}


HRESULT CLevel_Tool::NativeConstruct()
{
	FAILED_CHECK(__super::NativeConstruct());
	bImguiInit = false;
	bImguiDemo = true;

	return S_OK;
}

_int CLevel_Tool::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	bImguiInit = true;

	if (bImguiInit)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow(&bImguiDemo);
		ImGui::EndFrame();

	}

	return _int();
}

_int CLevel_Tool::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
}

HRESULT CLevel_Tool::Render()
{
	FAILED_CHECK(__super::Render());

	if (bImguiInit)
	{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	}

#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("Toll Level"));
#endif

	return S_OK;
}

CLevel_Tool * CLevel_Tool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLevel_Tool*	pInstance = new CLevel_Tool(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Tool::Free()
{
	__super::Free();
	
}
