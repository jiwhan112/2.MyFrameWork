#include "stdafx.h"
#include "Tool/Imgui_Terrain.h"

#include "Camera_Client.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "GameObject/GameObject_3D_Static.h"
#include "GameObject/GameObject_3D_Dynamic.h"

// #TODO: 맵툴 제작
// 맵 버텍스 수정
// 맵 저장 불러오기
// 맵 오브젝트 배치
// 큐브 셀 깔기



CImgui_Terrain::CImgui_Terrain(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_Terrain::NativeConstruct()
{
	mCurrent_TerrainObject = nullptr;

	mCameraClient = nullptr;
	meCreateTERRAIN_Layer = LAY_TERRAIN;
	meCreateOBJ_Layer = LAY_OBJECT;

	return S_OK;
}

HRESULT CImgui_Terrain::Update(_double time)
{
	if (mCameraClient == nullptr)
	{
		mCameraClient = static_cast<CCamera_Client*>(GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_CAMERA)));
		Safe_AddRef(mCameraClient);
	}

	CGameObject_Base* SelectObject = (CGameObject_Base*)GetSingle(CGameManager)->Get_ImGuiManager()->Get_SelectObject();

	if (SelectObject != nullptr)
	{
		E_OBJECT_TYPE type = SelectObject->Get_ObjectTypeID_Client();

	//	mCameraClient->Set_CameraMode(CCamera_Client::CAMERA_MODE_TARGET, SelectObject);

		if (type == OBJECT_TYPE_TERRAIN)
		{
			mCurrent_TerrainObject = static_cast<CGameObject_MyTerrain*>(SelectObject);
			meToolMode = CImgui_Terrain::E_TOOLMODE_TERRAIN_MAP;

		}
		else
			SelectObject = nullptr;
	}
	else
	{
		if (mCameraClient)
		{
			mCameraClient->ReleaseTarget();
		}

		SelectObject = nullptr;
		mCurrent_TerrainObject = nullptr;
		meToolMode = CImgui_Terrain::E_TOOLMODE_TERRAIN_END;
	}

	FAILED_CHECK(Render_UI());
	SelectObject = nullptr;

	return S_OK;
}

HRESULT CImgui_Terrain::Render_UI()
{

	if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_MAIN)))
	{

		if (ImGui::CollapsingHeader(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Terrain")))
		{
			// 빈 3D 오브젝트 생성
			RENDER_CREATEEMPTY();

			// 파일 불러오기
			RENDER_CREATE_PROTO();

			// 각 모드에 맞는 세팅
			if (meToolMode ==  CImgui_Terrain::E_TOOLMODE_TERRAIN_MAP)
			{
				// 타입에 따라 저장
				RENDER_MAP_MODE();
			}

			if (meToolMode == CImgui_Terrain::E_TOOLMODE_TERRAIN_OBJ)
			{
				RENDER_MAP_OBJ_MODE();
			}

			ImGui::Checkbox("TerrainSetting", &mIsTerrainSetting);

			if (mIsTerrainSetting)
			{
				if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_TERRAIN)))
				{
					WINDOW_TERRAIN();
					ImGui::End();
				}
			}
		}
		ImGui::End();
	}

	return S_OK;
}

void CImgui_Terrain::RENDER_CREATEEMPTY()
{

	// 빈 오브젝트 클론
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Create_Terrain")))
	{

		_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
		CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_TERRAIN);

		// 이미 만들어진 오브젝트 추가
		GetSingle(CGameInstance)->Push_Object(levelindex, TAGLAY(meCreateTERRAIN_Layer), createobj);
	}


}

void CImgui_Terrain::RENDER_CREATE_PROTO()
{
}

void CImgui_Terrain::RENDER_MAP_MODE()
{
}

void CImgui_Terrain::RENDER_MAP_OBJ_MODE()
{
}


void CImgui_Terrain::WINDOW_TERRAIN()
{
	if (mCurrent_TerrainObject )
	{
		// 선택된 TERRAIN 오브젝트 수정
		FAILED_CHECK_NONERETURN(Edit_TERRAIN());
	}

}

HRESULT CImgui_Terrain::Edit_TERRAIN()
{
	return S_OK;
}




CImgui_Terrain * CImgui_Terrain::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_Terrain*	pInstance = NEW CImgui_Terrain(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_Terrain::Free()
{
	__super::Free();
	Safe_Release(mCameraClient);
	Safe_Release(mCurrent_TerrainObject);

}

