#include "stdafx.h"
#include "Tool/Imgui_CommonUI.h"
#include "GameObject.h"

CImgui_CommonUI::CImgui_CommonUI(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_CommonUI::NativeConstruct()
{
	mSelectObject = nullptr;
	return S_OK;
}

HRESULT CImgui_CommonUI::Update(_double time)
{
	FAILED_CHECK(Render_UI());

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

		// Frame


		ImGui::End();
	}

	return S_OK;
}

void CImgui_CommonUI::PATHMODE()
{
	// 경로 저장용

	// 텍스트 저장
	IMGUI_TREE_BEGIN("PathToSaver_txt")
	{
		// PathFinder
		if (ImGui::Button("Path_For_Sprite"))
		{
			Button_PathTxtSave(STR_FILEPATH_RESOURCE_SPRITE_L, STR_FILEPATH_RESOURCE_PATH_L, L"SpritePath.txt");
		}
		if (ImGui::Button("Path_For_Dat"))
		{
			// Resoure에 있는 모든 리소스 데이터 TXT로 저장
			Button_PathTxtSave(STR_FILEPATH_RESOURCE_DAT_L, STR_FILEPATH_RESOURCE_PATH_L, L"DatPath.txt");
		}
		if (ImGui::Button("Path_For_3D"))
		{
			// Resoure에 있는 모든 리소스 데이터 TXT로 저장
			Button_PathTxtSave(STR_FILEPATH_RESOURCE_3DMODEL_L, STR_FILEPATH_RESOURCE_PATH_L, L"3DPath.txt");
		}
		IMGUI_TREE_END
	}
}


void CImgui_CommonUI::Button_PathTxtSave(wstring path, wstring txtpath, wstring txtname)
{
	// 탐색
	GetSingle(CGameInstance)->FolderFinder(path);

	// 탐색한 리스트 TXT 저장
	GetSingle(CGameInstance)->SaveVectorToDat(txtpath + L"\\" + txtname);
}

HRESULT CImgui_CommonUI::Update_ObjectList()
{
	_uint CurrentLevelIndex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
	const list<CGameObject*>*  objList = GetSingle(CGameInstance)->Get_GameObjectLayerList(CurrentLevelIndex, TAGLAY(LAY_BACKGROUND));

	if (objList == nullptr)
		return S_OK;

	if (ImGui::BeginListBox("ObjectListBox"))
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

	if (ImGui::Button("Delect Object"))
	{
		mSelectObject->Set_Dead();
		Safe_Release(mSelectObject);
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
