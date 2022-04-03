#include "stdafx.h"
#include "Tool/Imgui_UI.h"


CImgui_UI::CImgui_UI(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_UI::NativeConstruct()
{
	bDemo = true;
	f = 0;
	return S_OK;
}

HRESULT CImgui_UI::Update(_double time)
{
	DemoTest();

	ImGui::Begin("Hello, world!");
	ImGui::Text("aaa");

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::End();



	return S_OK;
}


void CImgui_UI::DemoTest()
{
	if (bDemo)
	{
		ImGui::ShowDemoWindow(&bDemo);

	}
}

void CImgui_UI::UISaver()
{
}

void CImgui_UI::TextureSaver()
{
}


CImgui_UI * CImgui_UI::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_UI*	pInstance = DBG_NEW CImgui_UI(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_UI::Free()
{
//	__super::Free();

}
