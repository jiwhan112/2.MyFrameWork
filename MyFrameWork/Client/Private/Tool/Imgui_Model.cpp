#include "stdafx.h"
#include "Tool/Imgui_Model.h"
#include "GameObject/GameObject_2D.h"
#include "FIleIO/ObjectIO.h"
#include "FIleIO/GameObject_Creater.h"


CImgui_Model::CImgui_Model(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_Model::NativeConstruct()
{


	mObjectIoManager = GetSingle(CGameManager)->Get_ObjectIOManager();
	Safe_AddRef(mObjectIoManager);

	mCreaterManager = GetSingle(CGameManager)->Get_CreaterManager();
	Safe_AddRef(mCreaterManager);

	return S_OK;
}

HRESULT CImgui_Model::Update(_double time)
{
	FAILED_CHECK(Render_UI());

	return S_OK;
}

HRESULT CImgui_Model::Render_UI()
{

	//if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_TEST)))
	//{
	//	if (ImGui::CollapsingHeader("PATH"))
	//	{
	//		PATHMODE();	
	//	}

	//	//static bool bAppOveraly = false;
	//	//ImGui::Checkbox("Check", &bAppOveraly);

	//	ImGui::Checkbox("ObjectListWindow", &mIsObjectList);

	//	if (mIsObjectList)
	//	{
	//		if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_OBJECT)))
	//		{
	//			FAILED_CHECK(Update_ObjectList());
	//			ImGui::End();
	//		}
	//	}

	//	ImGui::Checkbox("UISettingWindow", &mIsDataSetting);

	//	if (mIsDataSetting)
	//	{
	//		if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_UI)))
	//		{
	//			UIMODE();
	//			ImGui::End();
	//		}
	//	}

	//	ImGui::End();
	//}

	return S_OK;
}

void CImgui_Model::FBXMODE()
{
	// 선택된 FBX 오브젝트 수정
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
	Safe_Delete(mFBXPathData);


	Safe_Release(mObjectIoManager);
	Safe_Release(mCreaterManager);

}