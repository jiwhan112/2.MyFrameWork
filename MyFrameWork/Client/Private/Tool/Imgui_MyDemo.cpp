#include "stdafx.h"
#include "Tool/Imgui_MyDemo.h"

CImgui_MyDemo::CImgui_MyDemo(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_MyDemo::NativeConstruct()
{
	bDemo = true;
	mCurrentFrame = 0;
	mFrameCount = 0;
	dClock = 0;
	f = 0;
	return S_OK;
}

HRESULT CImgui_MyDemo::Update(_double time)
{
	// DemoTest();

	// TestUI
	IMGUI_TEST_MyDemo();

	// 프레TestUI3();임 테스트
	// FraTestUI4();meUI(time);
	// ImGui::Begin("Hello, world!");
	// ImGui::Text("aaa");
	// 
	// ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	// ImGui::End();



	return S_OK;
}


void CImgui_MyDemo::DemoTest()
{
	if (bDemo)
	{
		ImGui::ShowDemoWindow(&bDemo);

	}
}

void CImgui_MyDemo::FrameUI(_double time)
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

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}



void CImgui_MyDemo::IMGUI_TEST_Window()
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

void CImgui_MyDemo::IMGUI_TEST_MyDemo()
{
	// 이 함수 이외의 기능은 투머치 일단 이걸로 툴을 제작하고 필요한 기능은 찾아보자

	// 중복창 가능
	// 텍스트 출력 테스트
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

	// My TestUI
	if (ImGui::Begin(TAG_IMGUI(CImgui_Base::IMGUI_CHANEL_TEST)))
	{
		if (ImGui::CollapsingHeader("MyDemo"))
		{
			// 기본 기능 정리
			IMGUI_TEST_Basic();

			// 각 UI 세부 기능 정리
			IMGUI_TEST_Details();

			// Overaly
			static bool bAppOveraly = false;
			ImGui::Checkbox("over", &bAppOveraly);
			AppSimpleOverlay(&bAppOveraly);

		}

		ImGui::End();
	}
}
void CImgui_MyDemo::IMGUI_TEST_Basic()
{
	if (ImGui::Begin(TAG_IMGUI(CImgui_Base::IMGUI_CHANEL_TEST)))
	{

		IMGUI_TREE_BEGIN("Basic Button")
		{
			// Button
			static int clicked = 0;
			if (ImGui::Button("Button"))
			{
				// 버튼 이벤트
				clicked++;
			}

			if (clicked & 1)
			{
				ImGui::SameLine(); // 같은 위치에서 출력
				ImGui::Text("Clicked!");
			}

			// CheckBox
			static bool check = true;
			ImGui::Checkbox("checkbox", &check);

			// RadioButton
			// 해당 값으로 설정된다. 중복 X
			static int e = 0; // 초기값
			ImGui::RadioButton("radio a", &e, 0); ImGui::SameLine();
			ImGui::RadioButton("radio b", &e, 22); ImGui::SameLine();
			ImGui::RadioButton("radio c", &e, 33);

			// Color Button
			// 버튼 상태에 따른 스타일 설정
			ImGui::PushID(0);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(100, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(150, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(0, 0, 0));
			if (ImGui::Button("ColorButton"))
			{
				ImGui::SameLine();
				ImGui::Text("Clicked!");
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();

			// Arrow Button
			static int counter = 0;
			float spacing = ImGui::GetStyle().ItemInnerSpacing.x; // 화살표 간격
			ImGui::PushButtonRepeat(true); // Push - pop
			// strId로 버튼을 찾을 있음
			if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { counter--; }
			ImGui::SameLine(0.0f, spacing);
			if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { counter++; }
			ImGui::SameLine(0.0f, spacing);
			if (ImGui::ArrowButton("##donw", ImGuiDir_Down)) { counter++; }
			ImGui::SameLine(0.0f, spacing);
			if (ImGui::ArrowButton("##up", ImGuiDir_Up)) { counter++; }
			ImGui::PopButtonRepeat();
			ImGui::SameLine();
			ImGui::Text("%d", counter);

			// ToolTip
			ImGui::Text("up to me");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("ToolTip");
			ImGui::SameLine();
			ImGui::Text("- or IMG");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("I am a fancy tooltip");
				// 그래프 툴팁예시
				static float arr[] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f,0.8f,1.0f,0.5f,0.1f,0.0f };
				ImGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));
				ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr));
				ImGui::EndTooltip();
			}

			// Label
			ImGui::LabelText("BackText", "ValueText");

			// ComboBox
			const char* items[] = { "OBJ", "TEXTURE", "CAMERA" };
			static int item_current = 0;
			ImGui::Combo("ComboBox", &item_current, items, IM_ARRAYSIZE(items));
			ImGui::SameLine();
			switch (item_current)
			{
			case 0:
				HelpMarker("Select OBJ \n COMBOBOX");
				break;
			case 1:
				HelpMarker("Select TEXTURE \n COMBOBOX");
				break;
			case 2:
				HelpMarker("Select CAMERA \n COMBOBOX");
				break;
			}

			// Input Text
			static char str0[128] = "Default Text!";
			ImGui::InputText("hint text", str0, IM_ARRAYSIZE(str0));
			static char str1[128] = "";
			ImGui::InputTextWithHint("hint Text", "enter Obj Name", str1, IM_ARRAYSIZE(str1));

			// Input
			static int iValue = 123;
			ImGui::InputInt("input int", &iValue);
			// 증가량 / 부동 소수점 설정 가능
			static float fValue = 0.001f;
			ImGui::InputFloat("input float", &fValue, 0.01f, 1.0f, "%.3f");
			static double dValue = 10.50000;
			ImGui::InputDouble("input double", &dValue, 0.01f, 1.0f, "%.5f");
			static float vec4[4] = { 0.1f, 0.2f, 0.3f, 0.44f };
			ImGui::InputFloat3("input float3", vec4, "%.1f");

			// Drag 
			static int iValue2 = 1;
			static int iValue3 = 1;
			ImGui::DragInt("drag int", &iValue2, 1);
			ImGui::DragInt("drag int 0..100", &iValue3, 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);

			// Slider
			static float fValue2 = 0.1f;
			ImGui::SliderFloat("slider float", &fValue2, 0.0f, 1.0f, "ratio = %.3f");

			// static int iValue4=2;
			// ImGui::SliderInt("slider int", &iValue4, -10, 10);
			// static float angle = 0.0f; // -360~360
			// ImGui::SliderAngle("slider angle", &angle);

			// Slider Enum
			enum E_Element
			{
				E_Element_A, E_Element_B, E_Element_C, E_Element_D, E_Element_END
			};
			static int elem = E_Element_C;
			const char* elems_names[E_Element_END] = { "AAA", "BBB", "CCC", "DDD" };
			const char* elem_name = (elem >= 0 && elem < E_Element_END) ? elems_names[elem] : "Unknown";
			ImGui::SliderInt("slider enum", &elem, 0, E_Element_END - 1, elem_name);

			// Color
			static float col1[3] = { 1.0f, 0.0f, 0.2f };
			static float col2[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
			ImGui::ColorEdit3("color 1", col1);
			ImGui::ColorEdit4("color 2", col2);

			// ListBox
			const char* items_List[] = {
				"listBox1", "listBox2","listBox3",
				"listBox4", "listBox5","listBox6",
				"listBox7", "listBox8","listBox9"
			};
			static int items_List_current = 1;
			ImGui::ListBox("listbox", &items_List_current, items_List, IM_ARRAYSIZE(items_List), 2);
			IMGUI_TREE_END
		}

		// 스크롤
		IMGUI_TREE_BEGIN("Scrolling")
		{
			// 세로 스크롤 함수
			// 스크롤 추적 및 설명
			static int track_item = 50;
			static bool enable_track = true;
			static bool enable_extra_decorations = false;
			static float scroll_to_off_px = 0.0f;

			ImGui::Checkbox("Decoration", &enable_extra_decorations);
			ImGui::Checkbox("Track", &enable_track);

			// 아이템 100개 넣기
			// 아이템에 따른 추적
			ImGui::PushItemWidth(100);
			ImGui::SameLine(140); enable_track |= ImGui::DragInt("##item", &track_item, 0.25f, 0, 99, "Item = %d");

			// 스크롤에 따른 추적
			bool scroll_to_off = ImGui::Button("Scroll Offset");
			ImGui::SameLine(140); scroll_to_off |= ImGui::DragFloat("##off", &scroll_to_off_px, 1.00f, 0, FLT_MAX, "+%.0f px");


			if (scroll_to_off)
				enable_track = false;

			ImGuiStyle& style = ImGui::GetStyle();
			float child_w = (ImGui::GetContentRegionAvail().x - 4 * style.ItemSpacing.x) / 5;
			if (child_w < 1.0f)
				child_w = 1.0f;

			ImGui::PushID("##VerticalScrolling");
			for (int i = 0; i < 5; i++)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::BeginGroup();
				const char* names[] = { "Top", "25%", "Center", "75%", "Bottom" };
				ImGui::TextUnformatted(names[i]);

				const ImGuiWindowFlags child_flags = enable_extra_decorations ? ImGuiWindowFlags_MenuBar : 0;
				const ImGuiID child_id = ImGui::GetID((void*)(intptr_t)i);
				const bool child_is_visible = ImGui::BeginChild(child_id, ImVec2(child_w, 200.0f), true, child_flags);
				if (ImGui::BeginMenuBar())
				{
					ImGui::TextUnformatted("abc");
					ImGui::EndMenuBar();
				}
				if (scroll_to_off)
					ImGui::SetScrollY(scroll_to_off_px);

				if (child_is_visible) // Avoid calling SetScrollHereY when running with culled items
				{
					for (int item = 0; item < 100; item++)
					{
						if (enable_track && item == track_item)
						{
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "Item %d", item);
							ImGui::SetScrollHereY(i * 0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
						}
						else
						{
							ImGui::Text("Item %d", item);
						}
					}
				}
				float scroll_y = ImGui::GetScrollY();
				float scroll_max_y = ImGui::GetScrollMaxY();
				ImGui::EndChild();
				ImGui::Text("%.0f/%.0f", scroll_y, scroll_max_y);
				ImGui::EndGroup();
			}

			ImGui::PopID();
			IMGUI_TREE_END
		}

		// 필터링
		IMGUI_TREE_BEGIN("Filtering")
		{
			// 간단한 필터링
			// Helper class to easy setup a text filter.
			// You may want to implement a more feature-full filtering scheme in your own application.
			static ImGuiTextFilter filter;
			ImGui::Text("Filter usage:\n"
				"  \"\"         display all lines\n"
				"  \"xxx\"      display lines containing \"xxx\"\n"
				"  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
				"  \"-xxx\"     hide lines containing \"xxx\"");
			filter.Draw();
			const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
			for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
				if (filter.PassFilter(lines[i]))
					ImGui::BulletText("%s", lines[i]);
			IMGUI_TREE_END
		}

		ImGui::End();
	}
}

void CImgui_MyDemo::IMGUI_TEST_Details()
{

	// 각 기능의 세부기능 모음 
	// 쓸만한 기능만 추리기
	if (ImGui::Begin(TAG_IMGUI(CImgui_Base::IMGUI_CHANEL_TEST)))
	{

		IMGUI_TREE_BEGIN("Details")
		{
			// TreeNode
			IMGUI_TREE_BEGIN("Tree")
			{
				for (int i = 0; i < 5; i++)
				{
					// Flag 적용
					if (i == 0)
					{
						ImGui::SetNextItemOpen(true, ImGuiCond_Once);
					}

					if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
					{
						ImGui::Text("text");
						ImGui::SameLine();
						if (ImGui::SmallButton("NodeButton")) {}
						ImGui::TreePop();
					}
				}
				IMGUI_TREE_END
			}


			// TEXT
			IMGUI_TREE_BEGIN("Text")
			{
				// Bullet Text
				ImGui::BulletText("Bullet Text Test");
				ImGui::Bullet(); ImGui::Text("BulletText");
				ImGui::Bullet(); ImGui::SmallButton("Bullet Button");

				// Color Text
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Pink");

				// Wilde Text
				//static float wrap_width = 200.0f;
				//ImGui::SliderFloat("Wrap width", &wrap_width, 100, 500, "%.0f");
				//ImDrawList* draw_list = ImGui::GetWindowDrawList();
				//{
				//	ImVec2 pos = ImGui::GetCursorScreenPos();
				//	ImVec2 marker_min = ImVec2(pos.x + wrap_width, pos.y);
				//	ImVec2 marker_max = ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight());
				//	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
				//	ImGui::Text("aaaaaaaa bbbbbbbb, c cccccccc,dddddddd. d eeeeeeee   ffffffff. gggggggg!hhhhhhhh");

				//	// Draw actual text bounding box, following by marker of our expected limit (should not overlap!)
				//	draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
				//	draw_list->AddRectFilled(marker_min, marker_max, IM_COL32(255, 0, 255, 255));
				//	ImGui::PopTextWrapPos();
				//}



				IMGUI_TREE_END
			}

			// IMAGE
			IMGUI_TREE_BEGIN("IMAGE")
			{
				ImGuiIO& io = ImGui::GetIO();

				ImTextureID my_tex_id = io.Fonts->TexID;
				float my_tex_w = (float)io.Fonts->TexWidth;
				float my_tex_h = (float)io.Fonts->TexHeight;
				{
					// ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
					ImVec2 pos = ImGui::GetCursorScreenPos();
					ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
					ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
					ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
					ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
					ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);

					// 이미지 줌
					//if (ImGui::IsItemHovered())
					//{
					//	ImGui::BeginTooltip();
					//	float region_sz = 32.0f;
					//	float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
					//	float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
					//	float zoom = 4.0f;
					//	if (region_x < 0.0f) { region_x = 0.0f; }
					//	else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
					//	if (region_y < 0.0f) { region_y = 0.0f; }
					//	else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
					//	ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
					//	ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
					//	ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
					//	ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
					//	ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
					//	ImGui::EndTooltip();
					//}
				}

				IMGUI_TREE_END

			}

			IMGUI_TREE_BEGIN("COMBO")
			{

				// 한줄 콤보박스
				static int item_current_2 = 0;
				ImGui::Combo("combo 2 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

				// 접근자 함수 사용 콤보박스
				struct Funcs
				{
					static bool ItemGetter(void* data, int n, const char** out_str)
					{
						*out_str = ((const char**)data)[n]; return true;
					}
				};
				static int item_current_4 = 0;
				const char* items[] = { "aa","bb","cc" };
				ImGui::Combo("combo 4 (function)", &item_current_4, &Funcs::ItemGetter, items, IM_ARRAYSIZE(items));
				IMGUI_TREE_END
			}

			// 텍스트 선택
			IMGUI_TREE_BEGIN("Text Selectables")
			{
				// 중복 선텍 사능
				IMGUI_TREE_BEGIN("Basic Selectables")
				{
					static bool selection[5] = { false, false, false, false, false };
					ImGui::Selectable("1. I am selectable", &selection[0]);
					ImGui::Selectable("2. I am selectable", &selection[1]);
					ImGui::Text("(I am not selectable)");
					ImGui::Selectable("4. I am selectable", &selection[3]);

					// 더블클릭
					if (ImGui::Selectable("5. I am double clickable", selection[4], ImGuiSelectableFlags_AllowDoubleClick))
						if (ImGui::IsMouseDoubleClicked(0))
							selection[4] = !selection[4];
					IMGUI_TREE_END
				}

				// 싱글 선택
				IMGUI_TREE_BEGIN("Single Selectables")
				{
					static int selected = -1;
					for (int n = 0; n < 5; n++)
					{
						char buf[32];
						sprintf_s(buf, "Object %d", n);
						if (ImGui::Selectable(buf, selected == n))
							selected = n;
					}
					IMGUI_TREE_END
				}

				// 컨트롤키 중복선택
				IMGUI_TREE_BEGIN("CTRL Selectables")
				{
					static bool selection[5] = { false, false, false, false, false };
					for (int n = 0; n < 5; n++)
					{
						char buf[32];
						sprintf_s(buf, "Object %d", n);
						if (ImGui::Selectable(buf, selection[n]))
						{
							if (!ImGui::GetIO().KeyCtrl)
								memset(selection, 0, sizeof(selection));
							selection[n] ^= 1;
						}
					}
					IMGUI_TREE_END
				}


				// 행 
				//	IMGUI_DEMO_MARKER("Widgets/Selectables/In columns");
				// 그리드 
				//	IMGUI_DEMO_MARKER("Widgets/Selectables/Grid");
				// Alignment
				//	IMGUI_DEMO_MARKER("Widgets/Selectables/Alignment");


				IMGUI_TREE_END
			}


			IMGUI_TREE_BEGIN("Input Box")
			{
				IMGUI_TREE_BEGIN("Text Filters")
				{
					struct TextFilters
					{
						// Return 0 (pass) if the character is 'i' or 'm' or 'g' or 'u' or 'i'
						static int FilterImGuiLetters(ImGuiInputTextCallbackData* data)
						{
							if (data->EventChar < 256 && strchr("imgui", (char)data->EventChar))
								return 0;
							return 1;
						}
					};
					// 기본
					static char buf1[64] = ""; ImGui::InputText("default", buf1, 64);
					// 숫자만
					static char buf2[64] = ""; ImGui::InputText("decimal", buf2, 64, ImGuiInputTextFlags_CharsDecimal);
					// 대문자
					static char buf4[64] = ""; ImGui::InputText("uppercase", buf4, 64, ImGuiInputTextFlags_CharsUppercase);
					// NoBlank
					static char buf5[64] = ""; ImGui::InputText("no blank", buf5, 64, ImGuiInputTextFlags_CharsNoBlank);
					// 함수로 특정 문자만 받기
					static char buf6[64] = ""; ImGui::InputText("\"imgui\" letters", buf6, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
					IMGUI_TREE_END
				}

				IMGUI_TREE_END
			}

			// 탭창
			IMGUI_TREE_BEGIN("Tab Sheet")
			{
				ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
				if (ImGui::BeginTabBar("TestTab", tab_bar_flags))
				{
					if (ImGui::BeginTabItem("AAA"))
					{
						ImGui::Text("AAA Tab Open");
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("BBB"))
					{
						ImGui::Text("BBB Tab Open");
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("CCC"))
					{
						ImGui::Text("CCC Tab Open");
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
				ImGui::Separator();
				IMGUI_TREE_END

			}
			
			// 진행바
			IMGUI_TREE_BEGIN("PrograssBar")
			{

				static float progress = 0.0f, progress_dir = 1.0f;

				progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
				if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
				if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
				

				// Typically we would use ImVec2(-1.0f,0.0f) or ImVec2(-FLT_MIN,0.0f) to use all available width,
				// or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
				ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				ImGui::Text("Progress Bar");

				float progress_saturated = IM_CLAMP(progress, 0.0f, 1.0f);
				char buf[32];
				sprintf_s(buf, "%d/%d", (int)(progress_saturated * 1753), 1753);
				ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);

				IMGUI_TREE_END
			}

			// Color Picker
			IMGUI_TREE_BEGIN("Color Picker")
			{
				static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

				ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_AlphaPreview ;
				ImGui::ColorEdit4("MyColor##1", (float*)&color, misc_flags);
				ImGui::ColorEdit4("MyColor##2", (float*)&color, ImGuiColorEditFlags_Float| misc_flags);

				ImGui::Text("Color button with Picker:"); ImGui::SameLine();
				ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

				IMGUI_TREE_END
			}


#pragma region ItemBar Status


			// 각 UI의 상태 디버깅용
			IMGUI_TREE_BEGIN("ItemBar Status For Watching")
			{
				// Select an item type
				const char* item_names[] =
				{
					"Text", "Button", "Button (w/ repeat)", "Checkbox", "SliderFloat", "InputText", "InputTextMultiline", "InputFloat",
					"InputFloat3", "ColorEdit4", "Selectable", "MenuItem", "TreeNode", "TreeNode (w/ double-click)", "Combo", "ListBox"
				};
				static int item_type = 4;
				static bool item_disabled = false;
				ImGui::Combo("Item Type", &item_type, item_names, IM_ARRAYSIZE(item_names), IM_ARRAYSIZE(item_names));
				ImGui::SameLine();
				HelpMarker("Testing how various types of items are interacting with the IsItemXXX functions. Note that the bool return value of most ImGui function is generally equivalent to calling ImGui::IsItemHovered().");
				ImGui::Checkbox("Item Disabled", &item_disabled);

				// Submit selected item item so we can query their status in the code following it.
				bool ret = false;
				static bool b = false;
				static float col4f[4] = { 1.0f, 0.5, 0.0f, 1.0f };
				static char str[16] = {};

				if (item_disabled)
					ImGui::BeginDisabled(true);

				if (item_type == 0) { ImGui::Text("ITEM: Text"); }                                              // Testing text items with no identifier/interaction
				if (item_type == 1) { ret = ImGui::Button("ITEM: Button"); }                                    // Testing button
				if (item_type == 2) { ImGui::PushButtonRepeat(true); ret = ImGui::Button("ITEM: Button"); ImGui::PopButtonRepeat(); } // Testing button (with repeater)
				if (item_type == 3) { ret = ImGui::Checkbox("ITEM: Checkbox", &b); }                            // Testing checkbox
				if (item_type == 4) { ret = ImGui::SliderFloat("ITEM: SliderFloat", &col4f[0], 0.0f, 1.0f); }   // Testing basic item
				if (item_type == 5) { ret = ImGui::InputText("ITEM: InputText", &str[0], IM_ARRAYSIZE(str)); }  // Testing input text (which handles tabbing)
				if (item_type == 6) { ret = ImGui::InputTextMultiline("ITEM: InputTextMultiline", &str[0], IM_ARRAYSIZE(str)); } // Testing input text (which uses a child window)
				if (item_type == 7) { ret = ImGui::InputFloat("ITEM: InputFloat", col4f, 1.0f); }               // Testing +/- buttons on scalar input
				if (item_type == 8) { ret = ImGui::InputFloat3("ITEM: InputFloat3", col4f); }                   // Testing multi-component items (IsItemXXX flags are reported merged)
				if (item_type == 9) { ret = ImGui::ColorEdit4("ITEM: ColorEdit4", col4f); }                     // Testing multi-component items (IsItemXXX flags are reported merged)
				if (item_type == 10) { ret = ImGui::Selectable("ITEM: Selectable"); }                            // Testing selectable item
				if (item_type == 11) { ret = ImGui::MenuItem("ITEM: MenuItem"); }                                // Testing menu item (they use ImGuiButtonFlags_PressedOnRelease button policy)
				if (item_type == 12) { ret = ImGui::TreeNode("ITEM: TreeNode"); if (ret) ImGui::TreePop(); }     // Testing tree node
				if (item_type == 13) { ret = ImGui::TreeNodeEx("ITEM: TreeNode w/ ImGuiTreeNodeFlags_OpenOnDoubleClick", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_NoTreePushOnOpen); } // Testing tree node with ImGuiButtonFlags_PressedOnDoubleClick button policy.
				if (item_type == 14) { const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi" }; static int current = 1; ret = ImGui::Combo("ITEM: Combo", &current, items, IM_ARRAYSIZE(items)); }
				if (item_type == 15) { const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi" }; static int current = 1; ret = ImGui::ListBox("ITEM: ListBox", &current, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items)); }

				// Display the values of IsItemHovered() and other common item state functions.
				// Note that the ImGuiHoveredFlags_XXX flags can be combined.
				// Because BulletText is an item itself and that would affect the output of IsItemXXX functions,
				// we query every state in a single call to avoid storing them and to simplify the code.
				ImGui::BulletText(
					"Return value = %d\n"
					"IsItemFocused() = %d\n"
					"IsItemHovered() = %d\n"
					"IsItemHovered(_AllowWhenBlockedByPopup) = %d\n"
					"IsItemHovered(_AllowWhenBlockedByActiveItem) = %d\n"
					"IsItemHovered(_AllowWhenOverlapped) = %d\n"
					"IsItemHovered(_AllowWhenDisabled) = %d\n"
					"IsItemHovered(_RectOnly) = %d\n"
					"IsItemActive() = %d\n"
					"IsItemEdited() = %d\n"
					"IsItemActivated() = %d\n"
					"IsItemDeactivated() = %d\n"
					"IsItemDeactivatedAfterEdit() = %d\n"
					"IsItemVisible() = %d\n"
					"IsItemClicked() = %d\n"
					"IsItemToggledOpen() = %d\n"
					"GetItemRectMin() = (%.1f, %.1f)\n"
					"GetItemRectMax() = (%.1f, %.1f)\n"
					"GetItemRectSize() = (%.1f, %.1f)",
					ret,
					ImGui::IsItemFocused(),
					ImGui::IsItemHovered(),
					ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup),
					ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem),
					ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped),
					ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled),
					ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly),
					ImGui::IsItemActive(),
					ImGui::IsItemEdited(),
					ImGui::IsItemActivated(),
					ImGui::IsItemDeactivated(),
					ImGui::IsItemDeactivatedAfterEdit(),
					ImGui::IsItemVisible(),
					ImGui::IsItemClicked(),
					ImGui::IsItemToggledOpen(),
					ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y,
					ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y,
					ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y
				);

				if (item_disabled)
					ImGui::EndDisabled();

				char buf[1] = "";
				ImGui::InputText("unused", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_ReadOnly);
				ImGui::SameLine();
				HelpMarker("This widget is only here to be able to tab-out of the widgets above and see e.g. Deactivated() status.");

				IMGUI_TREE_END
			}
#pragma endregion


			IMGUI_TREE_END
		}
		ImGui::End();
	}
}

void CImgui_MyDemo::AppSimpleOverlay(bool* p_open)
{
	if (*p_open == false)
		return;

	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (corner != -1)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Example: Simple overlay", p_open, window_flags))
	{
		ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
		ImGui::Separator();
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			if (p_open && ImGui::MenuItem("Close")) *p_open = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}



CImgui_MyDemo * CImgui_MyDemo::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_MyDemo*	pInstance = NEW CImgui_MyDemo(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_MyDemo::Free()
{
//	__super::Free();

}
