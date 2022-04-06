#include "stdafx.h"
#include "Tool/Imgui_UI.h"


CImgui_UI::CImgui_UI(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_UI::NativeConstruct()
{
	bDemo = true;
	mCurrentFrame = 0;
	mFrameCount = 0;
	dClock = 0;
	f = 0;
	return S_OK;
}

HRESULT CImgui_UI::Update(_double time)
{
	DemoTest();

	if (ImGui::Begin(TAG_IMGUI(CImgui_Base::IMGUI_CHANEL_TEST)))
	{
		ImGui::End();
	}


	// TestCode
//	TestMenuBar();
	TestWidgets();	

	TestUI3();	
	TestUI4();

	// 프레TestUI3();임 테스트
	// FraTestUI4();meUI(time);
	// ImGui::Begin("Hello, world!");
	// ImGui::Text("aaa");
	// 
	// ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	// ImGui::End();



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
	// UI 이미지
	// 
	// UI 텍스처가 보여지고 

}

void CImgui_UI::FrameUI(_double time)
{

	dClock += time;
	mFrameCount++;


	if (dClock >= 1)
	{
		mCurrentFrame = mFrameCount;
		dClock = 0;
		mFrameCount = 0;
	}	
	
	ImGui::Begin("Frame");
	ImGui::Text("Frame: %d", mCurrentFrame);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f 
		/ ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void CImgui_UI::TextureSaver()
{
}

#define IMGUI_DEMO_MARKER(section)  do { if (GImGuiDemoMarkerCallback != NULL) GImGuiDemoMarkerCallback(__FILE__, __LINE__, section, GImGuiDemoMarkerCallbackUserData); } while (0)


void CImgui_UI::TestMenuBar()
{
	// 메뉴바
	ImGuiWindowFlags window_flags = 0;
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool unsaved_document = false;

	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
	//	if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

	bool t = true;

	//	ImGui::Text("dear imgui says hello. (%s)", IMGUI_VERSION);
	//	ImGui::Spacing();

	// Early out if the window is collapsed, as an optimization.
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "CTRL+S")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Examples"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

}

void CImgui_UI::TestWidgets()
{
	// 창 테스트

	if (ImGui::Begin(TAG_IMGUI(CImgui_Base::IMGUI_CHANEL_TEST)))
	{
		if (ImGui::CollapsingHeader("Text"))
		{
			ImGui::Text("TestBar");
			ImGui::BulletText("GamePlay ImGui.");
			ImGui::Separator();

			ImGui::Text("Somthing:");
			ImGui::BulletText("See comments in imgui.cpp.");
			ImGui::Separator();
		}

		ImGui::End();
	}
	
	// 위젯 테스트
	if (ImGui::Begin(TAG_IMGUI(CImgui_Base::IMGUI_CHANEL_TEST)))
	{
		if (ImGui::CollapsingHeader("Widgets"))
		{
			if (ImGui::CollapsingHeader("Widgets/Basic"))
			{
				if (ImGui::TreeNode("Basic"))
				{


					ImGui::TreePop();
				}
			}
		}

		ImGui::End();
	}

}

void CImgui_UI::TestUI3()
{
	//if (ImGui::Begin(TAG_IMGUI(CImgui_Base::IMGUI_CHANEL_TEST)))
	//{
	//	if (ImGui::CollapsingHeader("Text"))
	//	{
	//	}
	//	ImGui::End();
	//}

}

void CImgui_UI::TestUI4()
{
}


CImgui_UI * CImgui_UI::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_UI*	pInstance = NEW CImgui_UI(deviec, context);

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
