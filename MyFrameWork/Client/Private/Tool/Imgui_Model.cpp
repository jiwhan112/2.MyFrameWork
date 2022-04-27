#include "stdafx.h"
#include "Tool/Imgui_Model.h"
#include "Camera_Client.h"
#include "GameObject/GameObject_3D_Static.h"
#include "FIleIO/ObjectIO.h"
#include "FIleIO/GameObject_Creater.h"


CImgui_Model::CImgui_Model(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_Model::NativeConstruct()
{
	mCurrentModelObject = nullptr;

	mCameraClient = nullptr;

	return S_OK;
}

HRESULT CImgui_Model::Update(_double time)
{
	if (mCameraClient == nullptr)
	{
		mCameraClient = static_cast<CCamera_Client*>(GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_CAMERA)));
		Safe_AddRef(mCameraClient);
	}

	CGameObject* SelectObject = GetSingle(CGameManager)->Get_ImGuiManager()->Get_SelectObject();

	if (SelectObject != nullptr)
	{
		mCurrentModelObject = static_cast<CGameObject_3D_Static*>(SelectObject);
		if (mCameraClient)
			mCameraClient->Set_CameraMode(CCamera_Client::CAMERA_MODE_TARGET, mCurrentModelObject);


		// 3D 툴 카메라 세팅


	}
	else
	{
		mCurrentModelObject = nullptr;
	}

	FAILED_CHECK(Render_UI());

	return S_OK;
}

HRESULT CImgui_Model::Render_UI()
{
	if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_MAIN)))
	{
		if (ImGui::CollapsingHeader("MODEL_SAVER"))
		{
			FBX_CREATEMODE();

			ImGui::Checkbox("ModelSetting", &mIsModelSetting);

			if (mIsModelSetting)
			{
				if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_FBX)))
				{
					FBX_SETTINGMODE();
					ImGui::End();
				}
			}
		}

		ImGui::End();
	}
	

	return S_OK;
}

void CImgui_Model::FBX_SETTINGMODE()
{
	if (mCurrentModelObject == nullptr)
		return;

	// 선택된 FBX 오브젝트 수정
	FAILED_CHECK_NONERETURN(Edit_FBX());
	FAILED_CHECK_NONERETURN(Edit_Texture());

}

void CImgui_Model::FBX_CREATEMODE()
{
	// 저장
	CObjectIO* Object_IO_Manager = GetSingle(CGameManager)->Get_ObjectIOManager();

	// 생성
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	// MODEL 저장
	IMGUI_TREE_BEGIN("ObjectSaver")
	{
		if (mCurrentModelObject != nullptr)
		{
			static char ObjectName[128] = "";
			ImGui::InputTextWithHint("hint Text", "enter Obj Name", ObjectName, IM_ARRAYSIZE(ObjectName));
			if (ImGui::Button("SaveTest"))
			{
				// dat 파일 경로에 저장	
				string str = ObjectName;
				wstring wstr = CHelperClass::Convert_str2wstr(str);
				Object_IO_Manager->SaverObject(OBJECT_TYPE_3D_STATIC, STR_FILEPATH_RESOURCE_DAT_L, wstr + L".dat", mCurrentModelObject);
			}
		}
		IMGUI_TREE_END
	}

	IMGUI_TREE_BEGIN("Create Empty Object")
	{
		// 빈 오브젝트 클론
		if (ImGui::Button("Create_Empty_GAMEOBJECT3D_STATIC"))
		{
			_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
			CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_STATIC);
			//	MODEL_STATIC_DESC emptyDesc;
			//	static_cast<CGameObject_3D_Static*>(createobj)->Set_LoadModelDESC(emptyDesc);

			// 이미 만들어진 오브젝트 추가
			GetSingle(CGameInstance)->Push_Object(levelindex, TAGLAY(LAY_BACKGROUND), createobj);
		}
		IMGUI_TREE_END
	}
	
	IMGUI_TREE_BEGIN("ProtoObject")
	{
		if (mProtoModelList == nullptr)
			mProtoModelList = Create_Manager->Get_MapObject_Type(OBJECT_TYPE_3D_STATIC);

		static int selectObjectIndex = -1;
		_uint cnt = 0;
		static wstring selectObjectStr = L"";
		for (auto& protoString : *mProtoModelList)
		{
			cnt++;

			if (ImGui::Selectable(protoString.c_str(), selectObjectIndex == cnt))
			{
				selectObjectIndex = cnt;
				selectObjectStr = CHelperClass::Convert_str2wstr(protoString);
			}
		}

		// 선택 원형 오브젝트 클론
		if (ImGui::Button("Create_Clone"))
		{
			_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
			Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectStr, TAGLAY(LAY_OBJECT));
		}

		IMGUI_TREE_END
	}
}

HRESULT CImgui_Model::Edit_FBX()
{
	return S_OK;
}

HRESULT CImgui_Model::Edit_Texture()
{
	// FBX 파일에 따라 모델 바꾸기
	// 저장 불러오기 

	// 텍스처 선택 화면
	if (mFBXpathList == nullptr)
	{
		const list<MYFILEPATH*>* FBXPathList = GetSingle(CGameManager)->Get_PathList(CGameManager::PATHTYPE_FBX);
		if (FBXPathList == nullptr)
			return S_FALSE;

		mFBXpathList = NEW list<string>();

		wstring wstr;
		for (auto& path : *FBXPathList)
		{
			wstr = path->FileName;
			string str = CHelperClass::Convert_Wstr2str(wstr);
			mFBXpathList->push_front(str);
		}

	}

	if (mFBXpathList->empty())
		return E_FAIL;

	IMGUI_TREE_BEGIN("FbxFiles")
	{
		static int selectedTex = -1;

		_uint cnt = 0;
		string selectFBX = "";
		static ImGuiTextFilter filter;
		filter.Draw();

		for (auto iter = mFBXpathList->begin(); iter != mFBXpathList->end(); ++cnt, iter++)
		{
			if (filter.PassFilter(iter->c_str()))
			{
				if (ImGui::Selectable(iter->c_str(), selectedTex == cnt))
				{
					selectedTex = cnt;
					selectFBX = *iter;
					MODEL_STATIC_DESC fbx;
					
					strcpy_s(fbx.mModelName, selectFBX.c_str());
					mCurrentModelObject->Set_LoadModelDESC(fbx);
				}
			}
		}

		IMGUI_TREE_END
	}
	return S_OK;


	return S_OK;
}




CImgui_Model * CImgui_Model::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_Model*	pInstance = NEW CImgui_Model(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_Model");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_Model::Free()
{
	__super::Free();
	Safe_Release(mCameraClient);
	Safe_Delete(mFBXpathList);
	Safe_Delete(mProtoModelList);
	


}