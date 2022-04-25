#include "stdafx.h"
#include "Tool/IMGUI_Terrain.h"
#include "FIleIO/ObjectIO.h"


CIMGUI_Terrain::CIMGUI_Terrain(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CIMGUI_Terrain::NativeConstruct()
{


	return S_OK;
}

HRESULT CIMGUI_Terrain::Update(_double time)
{
	FAILED_CHECK(Render_UI());

	return S_OK;
}

HRESULT CIMGUI_Terrain::Render_UI()
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


CIMGUI_Terrain * CIMGUI_Terrain::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CIMGUI_Terrain*	pInstance = NEW CIMGUI_Terrain(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CIMGUI_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIMGUI_Terrain::Free()
{
	__super::Free();

}
