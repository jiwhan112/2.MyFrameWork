#include "stdafx.h"
#include "Tool/Imgui_Terrain.h"

// #TODO: ¸ÊÅø Á¦ÀÛ
// ¸Ê ¹öÅØ½º ¼öÁ¤
// ¸Ê ÀúÀå ºÒ·¯¿À±â
// ¸Ê ¿ÀºêÁ§Æ® ¹èÄ¡
// Å¥ºê ¼¿ ±ò±â



CImgui_Terrain::CImgui_Terrain(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_Terrain::NativeConstruct()
{

	meCreateTERRAIN_Layer = LAY_TERRAIN;
	meCreateOBJ_Layer = LAY_OBJECT;

	return S_OK;
}

HRESULT CImgui_Terrain::Update(_double time)
{
	FAILED_CHECK(Render_UI());

	return S_OK;
}

HRESULT CImgui_Terrain::Render_UI()
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


CImgui_Terrain * CImgui_Terrain::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_Terrain*	pInstance = NEW CImgui_Terrain(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_Terrain::Free()
{
	__super::Free();

}
