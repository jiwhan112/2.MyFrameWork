#include "stdafx.h"
#include "Tool/Imgui_UI.h"
#include "GameObject/GameObject_2D.h"
#include "FIleIO/ObjectIO.h"
#include "FIleIO/GameObject_Creater.h"



CImgui_UI::CImgui_UI(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_UI::NativeConstruct()
{
	mCurrentUIObject = nullptr;
	return S_OK;
}

HRESULT CImgui_UI::Update(_double time)
{
	CGameObject_Base* SelectObject = (CGameObject_Base*)GetSingle(CGameManager)->Get_ImGuiManager()->Get_SelectObject();
	if (SelectObject != nullptr)
	{
		E_OBJECT_TYPE type = SelectObject->Get_ObjectTypeID_Client();
		if (type == OBJECT_TYPE_2D)
		{
			mCurrentUIObject = static_cast<CGameObject_2D*>(SelectObject);
		}
			
	}
	else
		mCurrentUIObject = nullptr;

	FAILED_CHECK(Render_UI());


	return S_OK;
}

HRESULT CImgui_UI::Render_UI()
{
	if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_MAIN)))
	{

		if (ImGui::CollapsingHeader(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_UI,"Saver")))
		{
			UI_CREATEMODE();


			ImGui::Checkbox("UISettingWindow", &mIsDataSetting);

			if (mIsDataSetting)
			{
				if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_UI)))
				{
					UI_SETTINGMODE();
					ImGui::End();
				}
			}
		
		}

		ImGui::End();
	}

	return S_OK;
}

void CImgui_UI::UI_SETTINGMODE()
{
	if (mCurrentUIObject == nullptr)
		return;

	// 선택된 UI 오브젝트 수정
	FAILED_CHECK_NONERETURN(Edit_UIObject());
	FAILED_CHECK_NONERETURN(Edit_Texture());
}

void CImgui_UI::UI_CREATEMODE()
{
	// 저장
	CObjectIO* Object_IO_Manager = GetSingle(CGameManager)->Get_ObjectIOManager();

	// 생성
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	// UI 저장
	IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_UI,"ObjectSaver"))
	{
		if (mCurrentUIObject != nullptr)
		{
			static char ObjectName[128] = "";
			ImGui::InputTextWithHint("hint Text", "enter Obj Name", ObjectName, IM_ARRAYSIZE(ObjectName));
			if (ImGui::Button("SaveTest"))
			{
				// dat 파일 경로에 저장				
				string str = ObjectName;
				wstring wstr = CHelperClass::Convert_str2wstr(str);
				Object_IO_Manager->SaverObject(OBJECT_TYPE_2D, STR_FILEPATH_RESOURCE_DAT_L, wstr + L".dat", mCurrentUIObject);

			}
		}
		IMGUI_TREE_END
	}

	IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_UI, "CreateObject"))
	{
		// 빈 오브젝트 클론
		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_UI,"Create_Empty")))
		{
			// UI 타입별로 빈 오브젝트 생성
			_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
			CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_2D);
			UIDESC emptyDesc;
			static_cast<CGameObject_2D*>(createobj)->Set_LoadUIDesc(emptyDesc);

			// 이미 만들어진 오브젝트 추가
			GetSingle(CGameInstance)->Push_Object(levelindex, TAGLAY(LAY_UI), createobj);
		}

		// 이미 만들어진 오브젝트 클론

		if (mProtoUIList == nullptr)
			mProtoUIList = Create_Manager->Get_MapObject_Type(OBJECT_TYPE_2D);

		static int selectObjectIndex = -1;
		_uint cnt = 0;
		static wstring selectObjectStr = L"";

		for (auto& protoString : *mProtoUIList)
		{
			cnt++;

			if (ImGui::Selectable(protoString.c_str(), selectObjectIndex == cnt))
			{
				selectObjectIndex = cnt;
				selectObjectStr = CHelperClass::Convert_str2wstr(protoString);
			}
		}

		// 선택 원형 오브젝트 클론
		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_UI, "Create_ProtoObject")))
		{
			_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
			Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectStr, TAGLAY(LAY_UI));
		}


		IMGUI_TREE_END
	}

	
}




HRESULT CImgui_UI::Edit_UIObject()
{
	// UI 세팅 화면


	// UIDesc 정보만 변경해주면 알아서 오브젝트 업데이트 시에 수정된다.
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
	// 텍스처 선택 화면
	if (mSpritepathList == nullptr)
	{
		const list<MYFILEPATH*>* spritepathList = GetSingle(CGameManager)->Get_PathList(CGameManager::PATHTYPE_SPRITE);
		if (spritepathList == nullptr)
			return S_FALSE;

		mSpritepathList = NEW list<string>;

		wstring wstr;
		for (auto& path : *spritepathList)
		{
			wstr = path->FileName;
			string str = CHelperClass::Convert_Wstr2str(wstr);
			mSpritepathList->push_front(str);
		}

	}

	if (mSpritepathList->empty())
		return E_FAIL;

	IMGUI_TREE_BEGIN("Textures")
	{
		static int selectedTex = -1;

		_uint cnt = 0;
		string selectTeture = "";
		static ImGuiTextFilter filter;
		filter.Draw();

		for (auto iter = mSpritepathList->begin(); iter != mSpritepathList->end(); ++cnt, iter++)
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
	Safe_Delete(mSpritepathList);
	Safe_Delete(mProtoUIList);

}