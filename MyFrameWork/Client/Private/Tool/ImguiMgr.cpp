#include "stdafx.h"
#include "Tool/ImguiMgr.h"
#include "Tool/Imgui_CommonUI.h"

HRESULT CImguiMgr::NativeConstruct(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	// IMGUI ?ʱ?ȭ
	 // Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(pDevice, pDeviceContext);

	Add_IMGUI(mCommonUI = CImgui_CommonUI::Create(pDevice, pDeviceContext));
	Safe_AddRef(mCommonUI);

	return S_OK;
}

HRESULT CImguiMgr::Add_IMGUI(CImgui_Base * imgui)
{
	if (imgui == nullptr)
		return E_FAIL;

	mVecIMGUI.push_back(imgui);
	return S_OK;
}

HRESULT CImguiMgr::Remove_IMGUI(_uint idx)
{
	if (idx >= mVecIMGUI.size())
		return E_FAIL;

	Safe_Release(mVecIMGUI[idx]);
	misUpdate = false;
	return S_OK;
}

//void CImguiMgr::UpdateGUI()
//{
//	// Load Fonts
//	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
//	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
//	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
//	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
//	// - Read 'docs/FONTS.md' for more instructions and details.
//	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//	//io.Fonts->AddFontDefault();
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
//	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
//	//IM_ASSERT(font != NULL);
//
//	// Our state
//	bool show_demo_window = true;
//	bool show_another_window = false;
//	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//
//	// Main loop
//	bool done = false;
//	while (!done)
//	{
//		// Poll and handle messages (inputs, window resize, etc.)
//		// See the WndProc() function below for our to dispatch events to the Win32 backend.
//		MSG msg;
//		while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
//		{
//			::TranslateMessage(&msg);
//			::DispatchMessage(&msg);
//			if (msg.message == WM_QUIT)
//				done = true;
//		}
//		if (done)
//			break;
//
//		// Start the Dear ImGui frame
//		ImGui_ImplDX11_NewFrame();
//		ImGui_ImplWin32_NewFrame();
//		ImGui::NewFrame();
//
//		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//		if (show_demo_window)
//			ImGui::ShowDemoWindow(&show_demo_window);
//
//		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
//		{
//			static float f = 0.0f;
//			static int counter = 0;
//
//			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//			ImGui::Checkbox("Another Window", &show_another_window);
//
//			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//				counter++;
//			ImGui::SameLine();
//			ImGui::Text("counter = %d", counter);
//
//			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//			ImGui::End();
//		}
//
//		// 3. Show another simple window.
//		if (show_another_window)
//		{
//			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//			ImGui::Text("Hello from another window!");
//			if (ImGui::Button("Close Me"))
//				show_another_window = false;
//			ImGui::End();
//		}
//
//		// Rendering
//		ImGui::Render();
//	}
//}

HRESULT CImguiMgr::Update(_double time)
{
	misUpdate = true;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto& gui : mVecIMGUI)
	{
		if (gui == nullptr)
			continue;;
		gui->Update(time);
	}

	ImGui::EndFrame();
	return S_OK;
}

HRESULT CImguiMgr::Render()
{
	if (misUpdate == false)
		return S_OK;

	if (mVecIMGUI.empty() == false)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	return S_OK;
}

HRESULT CImguiMgr::ClearScene()
{
	// Common ?̿? ?? ????
	for (auto& gui: mVecIMGUI)
	{
		if (mCommonUI == gui)
			continue;
		Safe_Release(gui);
	}
	return S_OK;
}

CImgui_Base * CImguiMgr::Get_IMGUI(_uint index)
{
	if (mVecIMGUI.size() >= index)
		return nullptr;
	return mVecIMGUI[index];
}

CGameObject * CImguiMgr::Get_SelectObject() const
{
	if (mCommonUI)
		return mCommonUI->Get_SelectObject();
	return nullptr;
}

void CImguiMgr::DestroyIMGUI()
{
	// IMGUI ????

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

CImguiMgr * CImguiMgr::Create(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CImguiMgr*	pInstance = NEW CImguiMgr();

	if (FAILED(pInstance->NativeConstruct(hwnd, pDevice, pDeviceContext)))
	{
		MSGBOX("Failed to Creating CImguiMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImguiMgr::Free()
{
	Safe_Release(mCommonUI);

	for (auto& gui : mVecIMGUI)
	{
		Safe_Release(gui);
	}
	mVecIMGUI.clear();
	DestroyIMGUI();
}