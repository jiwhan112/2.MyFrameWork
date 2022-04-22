#include "stdafx.h"
#include "Tool/Imgui_UI.h"
#include "GameObject/GameObject_2D.h"
#include "FIleIO/ObjectIO.h"

/* #TODO: IMGUI ����

	���� �ҷ��� ���� ������Ʈ ��� ��� // ��
	������Ʈ Ŭ�� ���� �׽�Ʈ // ��

	������Ʈ ���ù� ���õ� ������Ʈ �����ϱ� // ��	
	���� �ҷ����� �׽�Ʈ // ��

	UI �����ϴ� �� ���� ������ //
	Input ������� ���� // 

	�θ��ڽ� ������Ʈ ����� // 
	������ ���� // 
*/

/*
	UI�������� UI ���� ���븸 ó��
	��������
	��������


*/

/*
	#TODO: ���������

*/

CImgui_UI::CImgui_UI(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_UI::NativeConstruct()
{
	mCurrentUIObject = nullptr;
	mObjectSaverClass = CObjectIO::Create();

	return S_OK;
}

HRESULT CImgui_UI::Update(_double time)
{
	FAILED_CHECK(Render_UI());

	return S_OK;
}

void CImgui_UI::Set_UIObject(CGameObject_2D * obj)
{
	Safe_Release(mCurrentUIObject);
	mCurrentUIObject = obj;
	Safe_AddRef(mCurrentUIObject);
}

HRESULT CImgui_UI::Render_UI()
{

	if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_TEST)))
	{
		if (ImGui::CollapsingHeader("PATH"))
		{
			PATHMODE();	
		}

		//static bool bAppOveraly = false;
		//ImGui::Checkbox("Check", &bAppOveraly);

		ImGui::Checkbox("ObjectListWindow", &mIsObjectList);

		if (mIsObjectList)
		{
			if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_OBJECT)))
			{
				FAILED_CHECK(Update_ObjectList());
				ImGui::End();
			}
		}

		ImGui::Checkbox("UISettingWindow", &mIsDataSetting);

		if (mIsDataSetting)
		{
			if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_UI)))
			{
				UIMODE();
				ImGui::End();
			}
		}

		ImGui::End();
	}

	return S_OK;
}

void CImgui_UI::UIMODE()
{
	// ���õ� UI ������Ʈ ����
	FAILED_CHECK_NONERETURN(Edit_ProtoObjectList());
	FAILED_CHECK_NONERETURN(Edit_UIObject());
	FAILED_CHECK_NONERETURN(Edit_Texture());


}

void CImgui_UI::PATHMODE()
{
	// �ؽ�Ʈ ����
	IMGUI_TREE_BEGIN("PathToSaver_txt")
	{

		if (ImGui::Button("Path_For_Sprite"))
		{

			Button_PathTxtSave(STR_FILEPATH_RESOURCE_SPRITE_L, STR_FILEPATH_RESOURCE_PATH_L, L"SpritePath.txt");
		}
		if (ImGui::Button("Path_For_Dat"))
		{
			// Resoure�� �ִ� ��� ���ҽ� ������ TXT�� ����
			Button_PathTxtSave(STR_FILEPATH_RESOURCE_DAT_L, STR_FILEPATH_RESOURCE_PATH_L, L"DatPath.txt");
		}
		if (ImGui::Button("Path_For_3D"))
		{
			// Resoure�� �ִ� ��� ���ҽ� ������ TXT�� ����
			Button_PathTxtSave(STR_FILEPATH_RESOURCE_3DMODEL_L, STR_FILEPATH_RESOURCE_PATH_L, L"3DPath.txt");
		}


		IMGUI_TREE_END
	}
	

	// ������Ʈ ����
	IMGUI_TREE_BEGIN("ObjectSaver")
	{
		if (mCurrentUIObject != nullptr)
		{
			static char ObjectName[128] = "";
			ImGui::InputTextWithHint("hint Text", "enter Obj Name", ObjectName, IM_ARRAYSIZE(ObjectName));
			if (ImGui::Button("SaveTest"))
			{
				// dat ���� ��ο� ����				
				string namechar = ObjectName;
				wstring namechar2;
				namechar2.assign(namechar.begin(), namechar.end());
				mObjectSaverClass->SaverObject(OBJECT_TYPE_2D, STR_FILEPATH_RESOURCE_DAT_L, namechar2 + L".dat", mCurrentUIObject);

			}
		}
		IMGUI_TREE_END
	}


	// �׽�Ʈ ���� ��ư Ŭ�� ������Ʈ ����
	IMGUI_TREE_BEGIN("For_TestButton")
	{
		if (ImGui::Button("Load_And_CloneData"))
		{
			// ���Ϸ� ����� ������ ������ �ҷ�����
			GetSingle(CGameObject_Creater)->LoaderDatFile_For_PrototypeObject();	
		}

		IMGUI_TREE_BEGIN("ProtoObject")
		{

			// ������Ʈ Ʈ���� ���� ������ ������Ʈ �����ֱ�
			auto CreateCloneObject = GetSingle(CGameObject_Creater)->Get_Map_GameObject2File_Proto();

			static int selectObjectIndex = -1;

			_uint cnt = 0;
			static wstring selectObjectStr = L"";
			for (auto& pObj : CreateCloneObject)
			{
				string cnvertString = "";
				cnt++;
				cnvertString.assign(pObj.first.begin(), pObj.first.end());

				if (ImGui::Selectable(cnvertString.c_str(), selectObjectIndex == cnt))
				{
					selectObjectIndex = cnt;
					selectObjectStr = pObj.first;
				}

			}

			// ���� ������Ʈ Ŭ��
			if (ImGui::Button("Create_Clone"))
			{
				// ���� ������Ʈ Ŭ��
				_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
				CGameObject* createobj =  GetSingle(CGameObject_Creater)->Create_ObjectClone_Prefab(idx, selectObjectStr, TAGLAY(LAY_BACKGROUND));
				if (mCurrentUIObject == nullptr)
				{
					mCurrentUIObject = static_cast<CGameObject_2D*>(createobj);
					Safe_AddRef(mCurrentUIObject);
				}

			}

			// �� ������Ʈ Ŭ��
			if (ImGui::Button("Create_Empty"))
			{
				// ���� ������Ʈ Ŭ��
				_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
				CGameObject* createobj = GetSingle(CGameObject_Creater)->CreateEmptyObject(GAMEOBJECT_2D);
				UIDESC desc;
				static_cast<CGameObject_2D*>(createobj)->Set_LoadUIDesc(desc);
				GetSingle(CGameInstance)->Push_Object(idx, TAGLAY(LAY_BACKGROUND), createobj);

				if (mCurrentUIObject == nullptr)
				{
					mCurrentUIObject = static_cast<CGameObject_2D*>(createobj);
					Safe_AddRef(mCurrentUIObject);
				}
			}

			IMGUI_TREE_END
		}
		IMGUI_TREE_END
	}
}

void CImgui_UI::Button_PathTxtSave(wstring path, wstring txtpath, wstring txtname)
{
	// �ؽ�ó �н����δ�

	// Ž��
	GetSingle(CGameInstance)->FolderFinder(path);

	// Ž���� ����Ʈ TXT ����
	GetSingle(CGameInstance)->SaveVectorToDat(txtpath + L"\\" + txtname);
}

void CImgui_UI::Button_TextureLoader()
{

}

HRESULT CImgui_UI::Edit_ProtoObjectList()
{
	//IMGUI_TREE_BEGIN("ProtoObject_List")
	//{
	//	// ������Ʈ ����Ʈ

	//	IMGUI_TREE_END
	//}
	return S_OK;
}

HRESULT CImgui_UI::Edit_UIObject()
{
	if (mCurrentUIObject == nullptr)
		return S_FALSE;

	// UIDesc ������ �������ָ� �˾Ƽ� ������Ʈ ������Ʈ �ÿ� �����ȴ�.
	UIDESC myDesc = mCurrentUIObject->Get_UIDesc();

	IMGUI_TREE_BEGIN("Position")
	{
		ImGui::DragInt("PosX", &myDesc.mPosX, 1.0f,0, g_iWinCX, "PosX = %d");
		ImGui::DragInt("PosY", &myDesc.mPosY, 1.0f, 0, g_iWinCY, "PosY= %d");
		IMGUI_TREE_END
	}

	IMGUI_TREE_BEGIN("Size")
	{
		ImGui::DragInt("sizeX", &myDesc.mSizeX,1, 1, g_iWinCX, "sizex = %d");
		ImGui::DragInt("sizeY", &myDesc.mSizeY,1, 1, g_iWinCY, "sizey = %d");
		IMGUI_TREE_END
	}

	IMGUI_TREE_BEGIN("Pivot")
	{
		ImGui::DragFloat2("PivotXY", (float*)&myDesc.mPivot,0.01f, 0.0f, 1.0f);
		IMGUI_TREE_END
	}
	IMGUI_TREE_BEGIN("Depth")
	{
		ImGui::DragInt("DepthINT", (int*)&myDesc.mDepth, 1, 0, 50);
		IMGUI_TREE_END
	}
	mCurrentUIObject->Set_LoadUIDesc(myDesc);
	return S_OK;
}

HRESULT CImgui_UI::Edit_Texture()
{
	// �ؽ�ó ���� ȭ��
	IMGUI_TREE_BEGIN("Textures")
	{
		static int selectedTex = -1;
		if (mListTextureKey == nullptr)
			mListTextureKey = (mCurrentUIObject->Get_TextureMap()->Get_MapKeyList());

		_uint cnt = 0;
		string selectTeture = "";
		static ImGuiTextFilter filter;
		filter.Draw();

		for (auto iter = mListTextureKey->begin(); iter != mListTextureKey->end(); ++cnt, iter++)
		{
			if (filter.PassFilter(iter->c_str()))
			{
				if (ImGui::Selectable(iter->c_str(), selectedTex == cnt))
				{
					selectedTex = cnt;
					selectTeture = *iter;

					//	mUIObject->Get_TextureMap()->Set_TextureMap(selectTeture);

					TEXTUREDESC tex;
					strcpy_s(tex.mTextureKey_Diffuse, selectTeture.c_str());
					mCurrentUIObject->Set_LoadTexDesc(tex);
				}
			}

			
		}

		IMGUI_TREE_END
	}
	return S_OK;
}

HRESULT CImgui_UI::Update_ObjectList()
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
			Update_ChildObject_ListBox(obj, &cnt,&selectIndex);
		}
		ImGui::EndListBox();
	}
	

	//if (ImGui::Button("Create New Object"))
	//{
	//	
	//}
	//ImGui::SameLine(50);
	if (ImGui::Button("Delect Object"))
	{
		mCurrentUIObject->Set_Dead();
		Safe_Release(mCurrentUIObject);
	}

	

	return S_OK;
}

void CImgui_UI::Update_ChildObject_ListBox(CGameObject * parent, _uint* cnt,_int* selectindex)
{
	E_OBJECT_TYPE objtype = (E_OBJECT_TYPE)parent->Get_ObjectTypeID();

	char buf[128] = "";
	sprintf_s(buf, "OBJ_%s_%d", TAGOBJTYPE(objtype),*cnt);

	if (ImGui::Selectable(buf, *selectindex == *cnt))
	{
		*selectindex = *cnt;
		Set_UIObject(static_cast<CGameObject_2D*>(parent));
	}
	(*cnt)++;

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


CImgui_UI * CImgui_UI::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_UI*	pInstance = NEW CImgui_UI(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_UI::Free()
{
	__super::Free();
	Safe_Release(mCurrentUIObject);
	Safe_Release(mObjectSaverClass);
}