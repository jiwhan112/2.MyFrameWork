#include "stdafx.h"
#include "GameObject/Dungeon_Manager.h"
#include "GameObject/Dungeon_Objects.h"
#include "GameObject/Client_Object.h"
#include "Camera_Client.h"

CDungeon_Manager::CDungeon_Manager()
{
	mDungeon_Objects = nullptr;
	mDevice = nullptr;
	mDeviceContext = nullptr;

}

HRESULT CDungeon_Manager::NativeConstruct(ID3D11Device* device,ID3D11DeviceContext* context)
{
	mDevice = device;
	mDeviceContext = context;

	Safe_AddRef(mDevice);
	Safe_AddRef(mDeviceContext);

	return S_OK;
}

_int CDungeon_Manager::Tick(_double TimeDelta)
{
	if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_K)& DIS_Down)
	{
		// 모드 변경 연출
		if (meCurrentGameMode == CDungeon_Manager::GAMEMODE_WORLD)
		{
			Set_CameraMove(CDungeon_Manager::CAMERAMODE_DUNGEON);
		}
		else
		{
			Set_CameraMove(CDungeon_Manager::CAMERAMODE_WORLD);
		}
	}

	Tick_Game(TimeDelta);

	return UPDATENONE;
}

_int CDungeon_Manager::LateTick(_double TimeDelta)
{
	return UPDATENONE;
}

_int CDungeon_Manager::Tick_Game(_double timeDelta)
{
	if (meCurrentGameMode == CDungeon_Manager::GAMEMODE_DUNGEON)
	{
	//	mDungeon_Objects->Get_DungeonMap()->Set_RenderOff();
	}
	else
	{
	//	mDungeon_Objects->Get_WorldMap()->Set_RenderOff();

	}

	return UPDATENONE;
}

HRESULT CDungeon_Manager::NativeConstruct_Level(E_LEVEL level)
{
	if (mDungeon_Objects == nullptr)
	{
		mDungeon_Objects = CDungeon_Objects::Create(mDevice, mDeviceContext, level);
		NULL_CHECK_HR(mDungeon_Objects);
		meCurrentGameMode = CDungeon_Manager::GAMEMODE_END;
	}

	meCurrentGameMode = CDungeon_Manager::GAMEMODE_DUNGEON;
	// 시점 설정
	mGameCamera =  (CCamera_Client*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_CAMERA));
	Safe_AddRef(mGameCamera);

	mGameCamera->Get_ComTransform()->Set_State(CTransform::STATE_POSITION,_float3(13,3,13));
	Set_CameraMove(E_CAMERAMODE::CAMERAMODE_DUNGEON);
	return S_OK;
}

void CDungeon_Manager::Release_DaungonData()
{
	Safe_Release(mDungeon_Objects);
}

HRESULT CDungeon_Manager::Set_CameraMove(E_CAMERAMODE eMode)
{
	// 월드 < - > 던전 카메라 이동 플레그
	// 카메라 연출
	switch (eMode)
	{
	case Client::CDungeon_Manager::CAMERAMODE_DUNGEON:
		meCurrentGameMode = GAMEMODE_DUNGEON;
		mGameCamera->Set_CameraMode(CCamera_Client::CAMERA_MODE_MOVE, CCamera_Client::CAMERA_MOVEPOS_STATE_D, (CGameObject*)mDungeon_Objects->Get_DungeonMap());
		break;
	
	case Client::CDungeon_Manager::CAMERAMODE_WORLD:
		meCurrentGameMode = GAMEMODE_WORLD;
		mGameCamera->Set_CameraMode(CCamera_Client::CAMERA_MODE_MOVE, CCamera_Client::CAMERA_MOVEPOS_STATE_W, (CGameObject*)mDungeon_Objects->Get_WorldMap());
		break;

	case Client::CDungeon_Manager::CAMERAMODE_INTRO:
		meCurrentGameMode = GAMEMODE_DUNGEON;
		mGameCamera->Set_CameraMode(CCamera_Client::CAMERA_MODE_MOVE, CCamera_Client::CAMERA_MOVEPOS_STATE_INTRO, (CGameObject*)mDungeon_Objects->Get_DungeonMap());
		break;

	case Client::CDungeon_Manager::CAMERAMODE_BOSS:
		meCurrentGameMode = GAMEMODE_WORLD;
		mGameCamera->Set_CameraMode(CCamera_Client::CAMERA_MODE_MOVE, CCamera_Client::CAMERA_MOVEPOS_STATE_BOSS, (CGameObject*)mDungeon_Objects->Get_WorldMap());

		break;
	case Client::CDungeon_Manager::CAMERAMODE_END:
		break;
	}	

	if (meCurrentGameMode == CDungeon_Manager::GAMEMODE_DUNGEON)
	{
		// 오브젝트 Visble
		mDungeon_Objects->Get_DungeonMap()->Set_isVisible(true);
		mDungeon_Objects->Get_WorldMap()->Set_isVisible(false);

		GetSingle(CGameManager)->Set_VisibleTag(TAGLAY(LAY_OBJECT_D), true);
		GetSingle(CGameManager)->Set_VisibleTag(TAGLAY(LAY_CUBETILE), true);
		GetSingle(CGameManager)->Set_VisibleTag(TAGLAY(LAY_OBJECT_W), false);
	}
	else
	{
		// 오브젝트 Visble
		mDungeon_Objects->Get_DungeonMap()->Set_isVisible(false);
		mDungeon_Objects->Get_WorldMap()->Set_isVisible(true);

		GetSingle(CGameManager)->Set_VisibleTag(TAGLAY(LAY_OBJECT_D), false);
		GetSingle(CGameManager)->Set_VisibleTag(TAGLAY(LAY_CUBETILE), false);
		GetSingle(CGameManager)->Set_VisibleTag(TAGLAY(LAY_OBJECT_W), true);

	}
	return S_OK;
}

CGameObject_3D_Tile * CDungeon_Manager::FInd_TIleForIndex(_int TileIndex) const
{
	NULL_CHECK_BREAK(mDungeon_Objects);
	return mDungeon_Objects->FInd_TIleForIndex(TileIndex);
}

HRESULT CDungeon_Manager::RemoveTile(CGameObject_3D_Tile * pTIle)
{
	NULL_CHECK_BREAK(mDungeon_Objects);
	return mDungeon_Objects->RemoveTile(pTIle);
}

HRESULT CDungeon_Manager::Setup_TileState(_int tileIndex)
{
	NULL_CHECK_BREAK(mDungeon_Objects);
	return mDungeon_Objects->Setup_TileState(tileIndex);
}

HRESULT CDungeon_Manager::Add_Task_Tile()
{
	return E_NOTIMPL;
}

HRESULT CDungeon_Manager::Add_Task_Gold()
{
	return E_NOTIMPL;
}

CDungeon_Manager * CDungeon_Manager::Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	CDungeon_Manager* pInstance = NEW CDungeon_Manager();

	if (FAILED(pInstance->NativeConstruct(device,deviceContext)))
	{
		MSGBOX("Failed to Created CDungeon_Manager");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDungeon_Manager::Free()
{
	Release_DaungonData();
	Safe_Release(mDevice);
	Safe_Release(mDeviceContext);
	Safe_Release(mGameCamera);


}


