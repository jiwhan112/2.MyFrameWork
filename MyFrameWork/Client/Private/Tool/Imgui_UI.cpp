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
	CGameObject* SelectObject = GetSingle(CGameManager)->Get_ImGuiManager()->Get_SelectObject();
	
	if (SelectObject != nullptr)
	{
		mCurrentUIObject = static_cast<CGameObject_2D*>(SelectObject);
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
		if (ImGui::CollapsingHeader("UI_SAVER"))
		{

			ImGui::Checkbox("UISettingWindow", &mIsDataSetting);

			if (mIsDataSetting)
			{
				if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_UI)))
				{
					UIMODE();
					ImGui::End();
				}
			}
			TESTMODE();
		}

		ImGui::End();
	}

	return S_OK;
}

void CImgui_UI::UIMODE()
{
	// 선택된 UI 오브젝트 수정
	FAILED_CHECK_NONERETURN(Edit_ProtoObjectList());
	FAILED_CHECK_NONERETURN(Edit_UIObject());
	FAILED_CHECK_NONERETURN(Edit_Texture());
}

void CImgui_UI::TESTMODE()
{
	// 저장
	CObjectIO* Object_IO_Manager = GetSingle(CGameManager)->Get_ObjectIOManager();
	// 생성
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	// UI 저장
	IMGUI_TREE_BEGIN("ObjectSaver")
	{
		if (mCurrentUIObject != nullptr)
		{
			static char ObjectName[128] = "";
			ImGui::InputTextWithHint("hint Text", "enter Obj Name", ObjectName, IM_ARRAYSIZE(ObjectName));
			if (ImGui::Button("SaveTest"))
			{
				// dat 파일 경로에 저장				
				string namechar = ObjectName;
				wstring namechar2;
				namechar2.assign(namechar.begin(), namechar.end());
				Object_IO_Manager->SaverObject(OBJECT_TYPE_2D, STR_FILEPATH_RESOURCE_DAT_L, namechar2 + L".dat", mCurrentUIObject);

			}
		}
		IMGUI_TREE_END
	}


	// 테스트 중인 버튼 클론 오브젝트 생성
	IMGUI_TREE_BEGIN("For_TestButton")
	{
		if (ImGui::Button("Load_And_CloneData"))
		{
			// 파일로 저장된 프로토 데이터 불러오기
			Create_Manager->LoaderDatFile_For_PrototypeObject();	
		}

		IMGUI_TREE_BEGIN("ProtoObject")
		{

			// 오브젝트 트리로 현재 생성된 오브젝트 보여주기
			auto CreateCloneObject =Create_Manager->Get_Map_GameObject2File_Proto();

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

			// 선택 오브젝트 클론
			if (ImGui::Button("Create_Clone"))
			{
				// 선택 오브젝트 클론
				_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
				CGameObject* createobj = Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectStr, TAGLAY(LAY_BACKGROUND));
				if (mCurrentUIObject == nullptr)
				{
					mCurrentUIObject = static_cast<CGameObject_2D*>(createobj);
					Safe_AddRef(mCurrentUIObject);
				}

			}

			// 빈 오브젝트 클론
			if (ImGui::Button("Create_Empty"))
			{
				// 선택 오브젝트 클론
				_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
				CGameObject* createobj =Create_Manager->CreateEmptyObject(GAMEOBJECT_2D);
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




HRESULT CImgui_UI::Edit_UIObject()
{
	// UI 세팅 화면

	if (mCurrentUIObject == nullptr)
		return S_FALSE;

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

}