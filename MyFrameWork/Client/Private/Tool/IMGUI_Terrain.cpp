#include "stdafx.h"
#include "Tool/Imgui_Terrain.h"

#include "Camera_Client.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "GameObject/GameObject_3D_Static.h"
#include "GameObject/GameObject_3D_Dynamic.h"

// #TODO: ���� ����
// �� ���ؽ� ����
// �� ���� �ҷ�����
// �� ������Ʈ ��ġ
// ť�� �� ���



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
			// �� 3D ������Ʈ ����
			RENDER_CREATEEMPTY();

			// ���� �ҷ�����
			RENDER_CREATE_PROTO();

			// �� ��忡 �´� ����
			if (meToolMode ==  CImgui_Terrain::E_TOOLMODE_TERRAIN_MAP)
			{
				// Ÿ�Կ� ���� ����
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

	// �� ������Ʈ Ŭ��
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Create_Terrain")))
	{

		_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
		CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_TERRAIN);

		// �̹� ������� ������Ʈ �߰�
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
		// ���õ� TERRAIN ������Ʈ ����
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

