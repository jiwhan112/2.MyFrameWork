#include "stdafx.h"
#include "Tool/Imgui_InGame.h"
#include "GameObject/Client_Object.h"
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
	mGameMode = GetSingle(CGameManager)->Get_DaungonManager()->Get_CurrentGameMode();

	mSelectObject =
		(CGameObject_Base*)GetSingle(CGameManager)->Get_ImGuiManager()->Get_SelectObject();


	if (mTerrainDungeon == nullptr)
	{
		mTerrainDungeon = GetSingle(CGameManager)->Get_LevelObject_DUNGEONMAP();
		Safe_AddRef(mTerrainDungeon);
	}
	if (mTerrainWorld == nullptr)
	{
		mTerrainWorld = GetSingle(CGameManager)->Get_LevelObject_DUNGEONMAP();
		Safe_AddRef(mTerrainWorld);


	}

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
					FAILED_CHECK(Edit_InGame());
					FAILED_CHECK(Edit_Unit());
					FAILED_CHECK(Edit_Camera());
				}
				ImGui::End();
			}
		}

		ImGui::End();
	}

	return S_OK;
}

HRESULT CImgui_InGame::Edit_InGame()
{
	if (mTerrainWorld == nullptr || mTerrainDungeon == nullptr)
		return S_FALSE;

	_float3 pickWorld = GetSingle(CGameManager)->Get_PickPos();
	int TileIndex  = 0;

	if (mGameMode == CDungeon_Manager::E_GAMEMODE::GAMEMODE_DUNGEON)
	{
		TileIndex =mTerrainDungeon->Get_TileIndex(pickWorld);
		ImGui::DragInt("TILEINDEX:",&TileIndex);
	}
	else if (mGameMode == CDungeon_Manager::E_GAMEMODE::GAMEMODE_WORLD)
	{
		TileIndex =mTerrainWorld->Get_TileIndex(pickWorld);
		ImGui::DragInt("TILEINDEX:", &TileIndex);
	}

	if (ImGui::Button("Create_Dungeon_MINE"))
	{
		_float3 SpawnPos = mTerrainDungeon->Get_TileWorld(mSpawnIndexDAUNGEON);
		GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects()->
			Create_Unit(E_TAYGAMEOBJECT::GAMEOBJECT_3D_DYNAMIC_MINE  ,SpawnPos);

	}
	ImGui::SameLine();
	if (ImGui::Button("Create_Dungeon_Orc"))
	{
		_float3 SpawnPos = mTerrainDungeon->Get_TileWorld(mSpawnIndexDAUNGEON);
		GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects()->
			Create_Unit(E_TAYGAMEOBJECT::GAMEOBJECT_3D_DYNAMIC_ORC, SpawnPos);
	}
	ImGui::SameLine();

	if (ImGui::Button("Create_Dungeon_Gobline"))
	{
		_float3 SpawnPos = mTerrainDungeon->Get_TileWorld(mSpawnIndexDAUNGEON);
		GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects()->
			Create_Unit(E_TAYGAMEOBJECT::GAMEOBJECT_3D_DYNAMIC_GOBLIN, SpawnPos);
	}

	//if (ImGui::Button("Create_WorldObject"))
	//{
	//	_float3 SpawnPos =  mTerrainWorld->Get_TileWorld(mSpawnIndexWORLD_UNIT);
	//	GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects()->Create_Unit(SpawnPos);
	//}

	return S_OK;
}

HRESULT CImgui_InGame::Edit_Unit()
{
	// 현재 선택 유닛에 따라서 정보변경
	if (mSelectObject == nullptr)
		return S_OK;

	E_OBJECT_TYPE id = (E_OBJECT_TYPE)mSelectObject->Get_ObjectTypeID();

	if (id == OBJECT_TYPE_3D_DYNAMIC)
	{
		CGameObject_3D_Dynamic* dynamicunit = ((CGameObject_3D_Dynamic*)mSelectObject);

		ImGui::Text(dynamicunit->Get_ComBehavior()->Get_StrCurrentLeafName());
	}
	
	return S_OK;
}

HRESULT CImgui_InGame::Edit_Camera()
{
	// 카메라 
	mCameraClient = (CCamera_Client*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_CAMERA));
	if (mCameraClient == nullptr)
		return S_OK;
	// 카메라 위치 설정
	/*IMGUI_TREE_BEGIN("CameraMove_LIST")
	{
		static _float3 newFLoat;
		ImGui::DragFloat3("SaveFloat3", (float*)&newFLoat, 0.1f, -100, 500);

		if (ImGui::Button("PushFloat"))
		{
			mCameraClient->mListMove_Dungeon.push_back(newFLoat);
		}
		if (ImGui::Button("PushFloat_Current"))
		{
			_float3 curPos = mCameraClient->Get_ComTransform()->GetState(CTransform::STATE_POSITION);
			mCameraClient->mListMove_Dungeon.push_back(curPos);
		}

		if (ImGui::Button("ClearList"))
		{
			mCameraClient->mListMove_Dungeon.clear();
		}

		if (ImGui::Button("Reverse"))
		{
			mCameraClient->mListMove_Dungeon.reverse();
		}

		IMGUI_LISTBOX_BEGIN("Visible")
		{
			for (auto pos : mCameraClient->mListMove_Dungeon)
			{
				char buf[64] = "";
				sprintf_s(buf, sizeof(buf), "%.3f, %.3f, %.3f", pos.x, pos.y, pos.z);
				ImGui::Text(buf);
			}

			IMGUI_LISTBOX_END
		}
		IMGUI_TREE_END
	}*/

	IMGUI_TREE_BEGIN("CameraMove_Bezior")
	{
		//if (ImGui::Button("CurrentPosSetCenterPos"))
		//{
		//	_float3 curPos = mCameraClient->Get_ComTransform()->GetState(CTransform::STATE_POSITION);
		//	mCameraClient->mDungeon_CenterPos = curPos;
		//}

		//char buf[64] = "";
		//_float3 pos = mCameraClient->mDungeon_CenterPos;
		//sprintf_s(buf, sizeof(buf), "%.3f, %.3f, %.3f", pos.x, pos.y, pos.z);
		//ImGui::Text(buf);


		//if (ImGui::Button("CAMERA_DUNGEON"))
		//{
		//	GetSingle(CGameManager)->Get_DaungonManager()->Set_CameraMove(CDungeon_Manager::CAMERAMODE_DUNGEON);
		//}

		//if (ImGui::Button("CAMERA_WORLD"))
		//{
		//	GetSingle(CGameManager)->Get_DaungonManager()->Set_CameraMove(CDungeon_Manager::CAMERAMODE_DUNGEON);
		//}

		IMGUI_TREE_END
	}


}

const char * CImgui_InGame::ToTextFloat3(_float3 pos)
{
	char buf[256] = "";
	sprintf_s(buf, "(%f,%f,%f)", pos.x, pos.y, pos.z);
	   
	return buf;
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
	mSelectObject = nullptr;
	mCameraClient = nullptr;
	Safe_Release(mTerrainDungeon);
	Safe_Release(mTerrainWorld);
}