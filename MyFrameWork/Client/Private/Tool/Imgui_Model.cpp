#include "stdafx.h"
#include "Tool/Imgui_Model.h"
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

	return S_OK;
}

HRESULT CImgui_Model::Update(_double time)
{
	CGameObject* SelectObject = GetSingle(CGameManager)->Get_ImGuiManager()->Get_SelectObject();

	if (SelectObject != nullptr)
	{
		mCurrentModelObject = static_cast<CGameObject_3D_Static*>(SelectObject);
	}
	else
		mCurrentModelObject = nullptr;
	

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

	// ���õ� FBX ������Ʈ ����
	FAILED_CHECK_NONERETURN(Edit_FBX());
	FAILED_CHECK_NONERETURN(Edit_Texture());

}

void CImgui_Model::FBX_CREATEMODE()
{
	// ����
	CObjectIO* Object_IO_Manager = GetSingle(CGameManager)->Get_ObjectIOManager();

	// ����
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	// MODEL ����
	//IMGUI_TREE_BEGIN("ObjectSaver")
	//{
	//	if (mCurrentModelObject != nullptr)
	//	{
	//		static char ObjectName[128] = "";
	//		ImGui::InputTextWithHint("hint Text", "enter Obj Name", ObjectName, IM_ARRAYSIZE(ObjectName));
	//		if (ImGui::Button("SaveTest"))
	//		{
	//			// dat ���� ��ο� ����				
	//			string namechar = ObjectName;
	//			wstring namechar2;
	//			namechar2.assign(namechar.begin(), namechar.end());
	//			Object_IO_Manager->SaverObject(OBJECT_TYPE_3D_STATIC, STR_FILEPATH_RESOURCE_DAT_L, namechar2 + L".dat", mCurrentModelObject);

	//		}
	//	}
	//	IMGUI_TREE_END
	//}

	IMGUI_TREE_BEGIN("Create Empty Object")
	{
		// �� ������Ʈ Ŭ��
		if (ImGui::Button("Create_Empty_GAMEOBJECT3D_STATIC"))
		{
			_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
			CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_STATIC);
			//	MODEL_STATIC_DESC emptyDesc;
			//	static_cast<CGameObject_3D_Static*>(createobj)->Set_LoadModelDESC(emptyDesc);

			// �̹� ������� ������Ʈ �߰�
			GetSingle(CGameInstance)->Push_Object(levelindex, TAGLAY(LAY_BACKGROUND), createobj);
		}
		IMGUI_TREE_END
	}

	//if (ImGui::Button("Load_UIdata_Proto"))
	//{
	//	// Dat ���Ͽ� ����� ������Ʈ �Ҿ����
	//	// Create_Manager Map�� ���� ����
	//	Create_Manager->LoaderDatFile_For_PrototypeObject();
	//}

	//IMGUI_TREE_BEGIN("ProtoObject")
	//{
	//	auto CreateCloneObject = Create_Manager->Get_Map_GameObject2File_Proto();

	//	static int selectObjectIndex = -1;

	//	_uint cnt = 0;
	//	static wstring selectObjectStr = L"";
	//	for (auto& pObj : *CreateCloneObject)
	//	{
	//		string cnvertString = "";
	//		cnt++;
	//		cnvertString.assign(pObj.first.begin(), pObj.first.end());

	//		if (ImGui::Selectable(cnvertString.c_str(), selectObjectIndex == cnt))
	//		{
	//			selectObjectIndex = cnt;
	//			selectObjectStr = pObj.first;
	//		}
	//	}

	//	// ���� ���� ������Ʈ Ŭ��
	//	if (ImGui::Button("Create_Clone"))
	//	{
	//		_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
	//		CGameObject* createobj = Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectStr, TAGLAY(LAY_BACKGROUND));
	//		if (mCurrentUIObject == nullptr)
	//		{
	//			mCurrentUIObject = static_cast<CGameObject_2D*>(createobj);
	//			Safe_AddRef(mCurrentUIObject);
	//		}
	//	}
	//	IMGUI_TREE_END
	//}
}

HRESULT CImgui_Model::Edit_FBX()
{
	return S_OK;
}

HRESULT CImgui_Model::Edit_Texture()
{
	// FBX ���Ͽ� ���� �� �ٲٱ�
	// ���� �ҷ����� 

	// �ؽ�ó ���� ȭ��
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

}