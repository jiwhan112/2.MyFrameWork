#include "stdafx.h"
#include "Tool/Imgui_InGame.h"


CImgui_InGame::CImgui_InGame(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_InGame::NativeConstruct()
{

	meCreateTERRAIN_Layer = LAY_TERRAIN;
	meCreateOBJ_Layer = LAY_OBJECT;

	return S_OK;
}

HRESULT CImgui_InGame::Update(_double time)
{
	FAILED_CHECK(Render_UI());

	return S_OK;
}

HRESULT CImgui_InGame::Render_UI()
{

	if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_TEST)))
	{
		if (ImGui::CollapsingHeader("Terrain"))
		{
			// PATHMODE();
		}

		ImGui::End();
	}

	return S_OK;
}


CImgui_InGame * CImgui_InGame::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_InGame*	pInstance = NEW CImgui_InGame(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_InGame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_InGame::Free()
{
	__super::Free();

}
