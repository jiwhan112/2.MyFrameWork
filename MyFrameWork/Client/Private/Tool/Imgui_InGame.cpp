#include "stdafx.h"
#include "Tool/Imgui_InGame.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "Camera_Client.h"

CImgui_InGame::CImgui_InGame(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_InGame::NativeConstruct()
{
//	meCreateTERRAIN_Layer = LAY_TERRAIN;
//	meCreateOBJ_Layer = LAY_OBJECT;

	return S_OK;
}

HRESULT CImgui_InGame::Update(_double time)
{
	FAILED_CHECK(Render_UI());

	return S_OK;
}

HRESULT CImgui_InGame::Render_UI()
{
	if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_MAIN)))
	{
		static bool bInGame = false;

		ImGui::Checkbox("INGAME", &bInGame);

		if (bInGame)
		{
			if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_INGAME)))
			{
				if (ImGui::CollapsingHeader("TerrainSetting"))
				{
					FAILED_CHECK(Edit_TERRAIN());
				}
				ImGui::End();
			}
		}

		ImGui::End();
	}

	return S_OK;
}

HRESULT CImgui_InGame::Edit_TERRAIN()
{
	// 지형처리 하기

	if (mCameraClient == nullptr)
	{
	//	CGameObject_Base* CameraObject = (CGameObject_Base*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_CAMERA));
	//	mCameraClient = static_cast<CCamera_Client*>(CameraObject);
	}
	if (mTerrainObject == nullptr)
	{
		CGameObject_Base* TerrainObejct = (CGameObject_Base*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN));
		mTerrainObject = static_cast<CGameObject_MyTerrain*>(TerrainObejct);

	}

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_INGAME, "Change_Terrain")))
	{
		E_TERRAINSIZE size =  mTerrainObject->Get_TerrainDESC().meTerrainSize;
		int newsize = (int)size;
		newsize++;
		newsize %= (int)TERRAINSIZE_128;
		mTerrainObject->Set_TerrainMode((E_TERRAINSIZE)newsize);

	}

	return S_OK;
}

CImgui_InGame * CImgui_InGame::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_InGame*	pInstance = NEW CImgui_InGame(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_InGame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_InGame::Free()
{
	__super::Free();
	Safe_Release(mTerrainObject);
	Safe_Release(mCameraClient);
}