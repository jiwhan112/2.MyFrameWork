#include "stdafx.h"
#include "GameObject/Dungeon_Manager.h"
#include "GameObject/Dungeon_Objects.h"
#include "GameObject/Dungeon_Task.h"
#include "GameObject/Client_Object.h"
#include "Camera_Client.h"
#include "AI/AI_Sequnce.h"



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
	mWorldCheckTimer = 0;
	return S_OK;
}

_int CDungeon_Manager::Tick(_double TimeDelta)
{
	Tick_Game(TimeDelta);
	return UPDATENONE;
}

_int CDungeon_Manager::LateTick(_double TimeDelta)
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

	
	return UPDATENONE;
}

_int CDungeon_Manager::Tick_Game(_double timeDelta)
{
	FAILED_CHECK_NONERETURN(Check_Task());

	// 1초마다 체크 월드에서 체크 할 것 체크
	mWorldCheckTimer += timeDelta;
	if (mWorldCheckTimer > 1)
	{
		FAILED_CHECK_NONERETURN(Check_World());
	}


	if (meCurrentGameMode == CDungeon_Manager::GAMEMODE_DUNGEON)
	{

		// Dungeon Global
	}
	else
	{
		// World Global
		if (KEYDOWN(DIK_R))
		{
			_float3 worldPos = GetSingle(CGameManager)->Get_PickPos();
			FAILED_CHECK_NONERETURN(Add_Task_WorldMove(worldPos));
		}

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

		mDungeon_TaskMgr = CDungeon_Task::Create();
		NULL_CHECK_HR(mDungeon_TaskMgr);

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
	Safe_Release(mDungeon_TaskMgr);

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
	return mDungeon_Objects->Find_TileForIndex(TileIndex);
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

HRESULT CDungeon_Manager::Add_Task_Tile(_uint index)
{
	return mDungeon_TaskMgr->Add_Task_Tile_Rock(index);
}

HRESULT CDungeon_Manager::Add_Task_Gold(_uint index)
{
	return 	mDungeon_TaskMgr->Add_Task_Tile_Gold(index);
}

HRESULT CDungeon_Manager::Add_Task_WorldMove(_float3 WorldPos)
{
	return
		mDungeon_TaskMgr->Add_Task_PlayerMoveWorld(WorldPos);
}
HRESULT CDungeon_Manager::Add_Task_Player_WorldAttack(CGameObject_Base* target)
{
	return 	mDungeon_TaskMgr->Add_Task_PlayerAttackWorld(target);
}

HRESULT CDungeon_Manager::Check_Task()
{
	TASKBASE* getTask = nullptr;
	if (mDungeon_TaskMgr)
		getTask = mDungeon_TaskMgr->Get_BackTask();
	if (getTask)
		Task_Trigger(getTask);
	return S_OK;
}

HRESULT CDungeon_Manager::Check_World()
{
	// 월드 체크 요소

	return S_OK;
}

_bool CDungeon_Manager::Task_Trigger(TASKBASE* task)
{
	// 테스크를 유닛에게 전달한다.

	switch (task->mTaskID)
	{
	case CDungeon_Task::TASK_TILE:
		Task_Mine(task);
		break;
	case CDungeon_Task::TASK_GOLD:
		Task_Mine(task);
		break;
	case CDungeon_Task::TASK_PLAYER_MOVE_WORLD:
		Task_Player_Move_World(task);
		break;
	case CDungeon_Task::TASK_PLAYER_ATTACK_WORLD:
		Task_Player_Attack_World(task);
		break;

	case CDungeon_Task::TASK_ENEMY_MOVE_WORLD_DEALY:
		Task_Player_Move_World(task);
		break;
	case CDungeon_Task::TASK_END:
		break;
	default:
		break;
	}
	return true;
}

_bool CDungeon_Manager::Task_Mine(TASKBASE* task)
{
	// 테스크를 유닛에게 전달한다.
	
	// 1. 유닛과 타일을 찾음
	auto unitlist = mDungeon_Objects->Get_ListObjecID(OBJECT_TYPE_3D_DYNAMIC_MINE);
	_uint tileindex =  static_cast<TASKTILE*>(task)->mTileIndex;
	CGameObject_3D_Tile* tile = mDungeon_Objects->Find_TileForIndex(tileindex);

	_float maxdis = INT8_MAX;
	CGameObject_Mine *SearchMine = nullptr;

	// 2. 조건 찾기
	for (auto& unit : unitlist)
	{
		// 타일과 가까운 유닛 찾기
		_float distance = _float3::Distance(unit->Get_WorldPostition(), tile->Get_WorldPostition());
		if (distance < maxdis)
		{
			maxdis = distance;
			SearchMine = (CGameObject_Mine*)unit;
		}
	}

	if (SearchMine == nullptr)
		return false;
	

	// 3. 유닛에게 정보전달
	if(task->mTaskID == CDungeon_Task::TASK_TILE)
		SearchMine->Set_Dig_Tile(tile);
	
	else if(task->mTaskID == CDungeon_Task::TASK_GOLD)
		SearchMine->Set_Dig_Gold(tile);


	Safe_Delete(task);
	return true;
}

_bool CDungeon_Manager::Task_Player_Move_World(TASKBASE * task)
{
	// 테스크를 유닛에게 전달한다.

	// 1. 월드 유닛 / 월드 타일 인덱스 검색
	auto unitlist = mDungeon_Objects->Get_UnitList_World(); 
	if (unitlist == nullptr)
		return false;
	if (unitlist->empty())
		return false;

	_float3 worldPos = static_cast<TASKMAP*>(task)->mWorldMapPickPos;
	_uint  GoalTileIndex = mDungeon_Objects->Get_WorldMap()->Get_TileIndex(worldPos);

	// 2. 가운데 유닛에게만 이동명령


	// 유닛하나에 정보를 주고 전체 따라가게 한다.
	// Flocking 사용
	// 중심 객체 구하기

	_float3 centerPos = _float3();
	CGameObject_3D_Dynamic* OrderObject = nullptr;
	_int PlayerCnt = 0;
	for (auto& unit : *unitlist)
	{
		if (unit->Get_UnitType() == E_UNITTYPE::UNIT_PLAYER)
		{
			centerPos += unit->Get_WorldPostition();
			OrderObject = unit;
			PlayerCnt++;
		}
		
	}

	if (PlayerCnt == 0)
		return false;

	centerPos /= PlayerCnt;

	_float MinDistance = INT_MAX;
	for (auto& unit : *unitlist)
	{
		if (unit->Get_UnitType() == E_UNITTYPE::UNIT_PLAYER)
		{
			_float dis = _float3::Distance(unit->Get_WorldPostition(), centerPos);
			if (dis < MinDistance)
			{
				OrderObject = unit;
				MinDistance = dis;
			}
		}
	}

	// 3. 같은 경로 따라가게 하기
	FAILED_CHECK_NONERETURN(OrderObject->Select_WorldPostition(worldPos));

	// 4. 따라가게 하기
	// TestCode
	for (auto& unit : *unitlist)
	{
		if (unit->Get_UnitType() == E_UNITTYPE::UNIT_PLAYER)
		{
			FAILED_CHECK_NONERETURN(unit->Select_WorldPostition(worldPos));

		}
	}
	Safe_Delete(task);
	return true;
}

_bool CDungeon_Manager::Task_Player_Attack_World(TASKBASE * task)
{
	// 1. 월드 유닛 / 월드 타일 인덱스 검색
	auto unitlist = mDungeon_Objects->Get_UnitList_World();
	CGameObject_3D_Dynamic* worldTarget = (CGameObject_3D_Dynamic*)(static_cast<TASK_VOID*>(task)->pArg);

	// 2. 유닛에게 정보 전달 
	for (auto& unit : *unitlist)
	{
		E_UNITTYPE objtype =  unit->Get_UnitType();

		switch (objtype)
		{
		case UNIT_PLAYER:
			unit->Select_WorldAttack(worldTarget);
			break;
		case UNIT_ENEMY:
			unit->Select_WorldAttack(worldTarget);
			break;
		default:
			break;
		}
	
	}

	Safe_Delete(task);
	return true;
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


