#include "stdafx.h"
#include "Tool/Imgui_CommonUI.h"
#include "GameObject.h"
#include "Layer.h"

CImgui_CommonUI::CImgui_CommonUI(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_CommonUI::NativeConstruct()
{
	PATHMODE_ALLLOAD();
	dClock = 0;
	mFrameCount = 0;
	mCurrentFrame = 0;

	mSelectObject = nullptr;
	return S_OK;
}

HRESULT CImgui_CommonUI::Update(_double time)
{
	FAILED_CHECK(Render_UI());
	FrameUI_Overaly(time);

	return S_OK;
}

HRESULT CImgui_CommonUI::Render_UI()
{
	if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_MAIN)))
	{
		// Path
		if (ImGui::CollapsingHeader("PATH"))
		{
			PATHMODE();
		}

		// ObjList
		if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_OBJECTLIST)))
		{
			FAILED_CHECK(Update_ObjectList());
			ImGui::End();
		}

		{
			// DebugText
			_float3 pos =  GetSingle(CGameManager)->Get_PickPos();
			ImGui::DragFloat3("MousePick",(float*)&pos);
		}


		// Frame

		ImGui::End();
	}

	return S_OK;
}

void CImgui_CommonUI::PATHMODE()
{
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	// 경로 저장용

	// 텍스트 저장
	IMGUI_TREE_BEGIN("PathToSaver_txt")
	{
		// PathFinder
		if (ImGui::Button("PATH_ALL"))
		{
			PATHMODE_ALLLOAD();
		}

		//	if (ImGui::Button("Path_For_Sprite"))
		//	{
		//		Button_PathTxtSave(STR_FILEPATH_RESOURCE_SPRITE_L, STR_FILEPATH_RESOURCE_PATH_L, STR_FILENAME_SPRITE_TXT);
		//	}
		//	if (ImGui::Button("Path_For_Dat"))
		//	{
		//		// Resoure에 있는 모든 리소스 데이터 TXT로 저장
		//		Button_PathTxtSave(STR_FILEPATH_RESOURCE_DAT_L, STR_FILEPATH_RESOURCE_PATH_L, STR_FILENAME_DATA_TXT);
		//	}
		//	if (ImGui::Button("Path_For_3D"))
		//	{
		//		// Resoure에 있는 모든 리소스 데이터 TXT로 저장
		//		Button_PathTxtSave(STR_FILEPATH_RESOURCE_3DMODEL_L, STR_FILEPATH_RESOURCE_PATH_L, STR_FILENAME_3D_TXT);
		//	}
		//
		//	if (ImGui::Button("Path_For_3D_STATIC"))
		//	{
		//		// Resoure에 있는 모든 리소스 데이터 TXT로 저장
		//		Button_PathTxtSave(STR_FILEPATH_RESOURCE_3DPATHTXT_STATIC_L, STR_FILEPATH_RESOURCE_PATH_L, STR_FILENAME_3DSTATIC_TXT);
		//	}
		//	if (ImGui::Button("Path_For_3D_DYNAMIC"))
		//	{
		//		// Resoure에 있는 모든 리소스 데이터 TXT로 저장
		//		Button_PathTxtSave(STR_FILEPATH_RESOURCE_3DPATHTXT_DYNAMIC_L, STR_FILEPATH_RESOURCE_PATH_L, STR_FILENAME_3DDYNAMIC_TXT);
		//	}
		IMGUI_TREE_END
	}

	// Dat 파일로 원본 생성해서 CreateManager맵에 넣어놓기
	IMGUI_TREE_BEGIN("Load_Data_Proto")
	{
		if (ImGui::Button("Load_AllDatFile"))
		{
			Create_Manager->LoaderDatFile_For_PrototypeObject();
		}
		IMGUI_TREE_END
	}
}

void CImgui_CommonUI::PATHMODE_ALLLOAD()
{
	// 경로 / 저장위치 / 파일이름
	Button_PathTxtSave(STR_FILEPATH_RESOURCE_SPRITE_L, STR_FILEPATH_RESOURCE_PATH_L, STR_FILENAME_SPRITE_TXT);
	Button_PathTxtSave(STR_FILEPATH_RESOURCE_DAT_L, STR_FILEPATH_RESOURCE_PATH_L, STR_FILENAME_DATA_TXT);
	Button_PathTxtSave(STR_FILEPATH_RESOURCE_3DMODEL_L, STR_FILEPATH_RESOURCE_PATH_L, STR_FILENAME_3DSPRITE_TXT, L"png");
	Button_PathTxtSave(STR_FILEPATH_RESOURCE_3DMODEL_STATIC_L, STR_FILEPATH_RESOURCE_PATH_L, STR_FILENAME_3DSTATIC_TXT, L"fbx");
	Button_PathTxtSave(STR_FILEPATH_RESOURCE_3DMODEL_DYNAMIC_L, STR_FILEPATH_RESOURCE_PATH_L, STR_FILENAME_3DDYNAMIC_TXT, L"fbx");
}

void CImgui_CommonUI::Button_PathTxtSave(wstring path, wstring txtpath, wstring txtname, wstring exeName)
{
	// 탐색
	GetSingle(CGameInstance)->FolderFinder(path);

	// 탐색한 리스트 TXT 저장
	GetSingle(CGameInstance)->SaveVectorToDat(txtpath + L"\\" + txtname, exeName);
}

HRESULT CImgui_CommonUI::Update_ObjectList()
{
	//_uint CurrentLevelIndex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
	//auto objmap = GetSingle(CGameInstance)->Get_All_GameObjectLayerMap(CurrentLevelIndex);

	//// 각 레이어 별로 생성

	//if (objmap == nullptr)
	//	return S_OK;

	//if (ImGui::BeginListBox("ObjectListBox"))
	//{
	//	static _int selectIndex = -1;
	//	_uint cnt = 0;

	//	for (auto& pair : *objmap)
	//	{
	//		wstring wstr = pair.first;
	//		string str = CHelperClass::Convert_Wstr2str(wstr);
	//		auto objectList = pair.second->Get_GameObjectList();
	//		if (objectList == nullptr)
	//			continue;

	//		for (auto& obj : *objectList)
	//		{
	//			if (obj == nullptr)
	//				continue;
	//			Update_ListBox(obj, str, cnt, &selectIndex);
	//			cnt++;
	//		}
	//	}
	//	ImGui::EndListBox();
	//}

	Update_ObjectList_Layer();


	//if (ImGui::Button("Delect Object"))
	//{
	//	if (mSelectObject)
	//	{
	//		mSelectObject->Set_Dead();
	//		Safe_Release(mSelectObject);
	//		mSelectObject = nullptr;
	//	}
	//}
	//if (ImGui::Button("Delect All"))
	//{
	//	for (auto& pair : *objmap)
	//	{
	//		if (pair.first == TAGLAY(LAY_CAMERA))
	//			continue;
	//		else
	//		{
	//			auto objectList = pair.second->Get_GameObjectList();

	//			for (auto& obj : *objectList)
	//			{
	//				obj->Set_Dead();
	//			}
	//		}
	//	}	

	//}


	/*if (ImGui::BeginListBox("ObjectListBox"))
	{
		static _int selectIndex = -1;
		_uint cnt = 0;

		for (auto& obj : *objList)
		{
			if (obj == nullptr)
				continue;
			Update_ListBox(obj, cnt, &selectIndex);
			cnt++;
		}
		ImGui::EndListBox();
	}

	*/
	return S_OK;
}

HRESULT CImgui_CommonUI::Update_ObjectList_Layer()
{

	_uint CurrentLevelIndex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
	auto objmap = GetSingle(CGameInstance)->Get_All_GameObjectLayerMap(CurrentLevelIndex);

	if (objmap == nullptr)
		return S_OK;

	IMGUI_LISTBOX_BEGIN("ObjectBox")
	{
		for (auto& pair : *objmap)
		{
			wstring laywstr = pair.first;
			string laystr = CHelperClass::Convert_Wstr2str(laywstr);
			auto objectList = pair.second->Get_GameObjectList();
			if (objectList == nullptr)
				continue;


			IMGUI_TREE_BEGIN(laystr.c_str())
			{
				int cnt = 0;
				for (auto& obj : *objectList)
				{
					if (obj == nullptr)
						continue;

					E_OBJECT_TYPE objtype = (E_OBJECT_TYPE)obj->Get_ObjectTypeID();

					char buf[128] = "";
					sprintf_s(buf, "OBJ_%s_%d", TAGOBJTYPE(objtype), cnt);
					if (ImGui::Selectable(buf, mSelectIndex == cnt))
					{
						mSelectIndex = cnt;
						Set_SelectObject(obj);
					}
					cnt++;
				}
				IMGUI_TREE_END
			}

		}

		IMGUI_LISTBOX_END
	}
	return S_OK;
}

HRESULT CImgui_CommonUI::Set_SelectObject(CGameObject * obj)
{
	Safe_Release(mSelectObject);
	mSelectObject = obj;
	Safe_AddRef(mSelectObject);
	return S_OK;
}

void CImgui_CommonUI::Update_ListBox(CGameObject * obj, _uint cnt, _int* selectindex)
{
	E_OBJECT_TYPE objtype = (E_OBJECT_TYPE)obj->Get_ObjectTypeID();

	char buf[128] = "";
	sprintf_s(buf, "OBJ_%s_%d", TAGOBJTYPE(objtype), cnt);

	if (ImGui::Selectable(buf, *selectindex == cnt))
	{
		*selectindex = cnt;
		Set_SelectObject(obj);
	}

	//auto childlist = parent->Get_Children();
	//if (childlist == nullptr)
	//	return;

	//char buf2[128] = "";
	//sprintf_s(buf2, "Child%d", parent->Get_Depth());

	//for (auto& obj : *childlist)
	//{
	//	IMGUI_TREE_BEGIN(buf2)
	//	{
	//		Update_ChildObject_ListBox(obj, cnt, selectindex);
	//		IMGUI_TREE_END
	//	}
	//}
}

void CImgui_CommonUI::Update_ListBox(CGameObject * obj, string layerstr, _uint cnt, _int* selectindex)
{
	E_OBJECT_TYPE objtype = (E_OBJECT_TYPE)obj->Get_ObjectTypeID();

	char buf[128] = "";
	sprintf_s(buf, "%s_%s_%d", layerstr.c_str(), TAGOBJTYPE(objtype), cnt);

	if (ImGui::Selectable(buf, *selectindex == cnt))
	{
		*selectindex = cnt;
		Set_SelectObject(obj);
	}
}

//void CImgui_CommonUI::FrameUI(_double time)
//{
//	dClock += time;
//	mFrameCount++;
//
//
//	if (dClock >= 1)
//	{
//		mCurrentFrame = mFrameCount;
//		dClock = 0;
//		mFrameCount = 0;
//	}
//
//	ImGui::Begin("Frame");
//	ImGui::Text("Frame: %d", mCurrentFrame);
//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f
//		/ ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//	ImGui::End();
//}

void CImgui_CommonUI::FrameUI_Overaly(_double time)
{
	static int corner = 0;
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
	bool t = true;
	if (ImGui::Begin("Example: Simple overlay", &t, window_flags))
	{
		dClock += time;
		mFrameCount++;

		if (dClock >= 1)
		{
			mCurrentFrame = mFrameCount;
			dClock = 0;
			mFrameCount = 0;
		}

		ImGui::Text("Frame: %d", mCurrentFrame);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f
			/ ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();
}

CImgui_CommonUI * CImgui_CommonUI::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_CommonUI*	pInstance = NEW CImgui_CommonUI(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_CommonUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_CommonUI::Free()
{
	Safe_Release(mDevice);
	Safe_Release(mDeviceContext);

	Safe_Release(mSelectObject);
}