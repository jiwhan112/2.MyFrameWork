#pragma once

#include "Base.h"

BEGIN(Client)
class CGameObject_MyTerrain;
class CGameObject_3D_Tile;

// 슈퍼클래스
// 게임에 들어가는 전반적인 처리 구현
// 던전맵 / 지형맵 / 
// 플레이어 처리(마우스)
// 게임내에 모든 객체 관리등 

struct TASKBASE;

class CDungeon_Manager final :
	public CBase
{
public:
	enum E_GAMEMODE
	{
		GAMEMODE_DUNGEON,
		GAMEMODE_WORLD,
		GAMEMODE_END
	};
	enum E_CAMERAMODE
	{
		CAMERAMODE_DUNGEON,
		CAMERAMODE_WORLD,
		CAMERAMODE_INTRO,
		CAMERAMODE_BOSS,
		CAMERAMODE_END,
	};


protected:
	explicit CDungeon_Manager();
	virtual ~CDungeon_Manager() = default;


public:
	virtual HRESULT NativeConstruct(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	_int Tick_Game(_double timeDelta);

public: //GET SET
	class CDungeon_Objects* Get_DungeonObjects() const { return mDungeon_Objects; }
	E_GAMEMODE Get_CurrentGameMode()const
	{
		return meCurrentGameMode;
	}

public:
	HRESULT NativeConstruct_Level(E_LEVEL level);

	// 레벨 나갈떄마다
	void Release_DaungonData();

public: // For. Camera
	HRESULT Set_CameraMove(E_CAMERAMODE mode);

public: // For. Tile
	CGameObject_3D_Tile* FInd_TIleForIndex(_int TileIndex) const;
	HRESULT RemoveTile(CGameObject_3D_Tile* pTIle);
	HRESULT Setup_TileState(_int tileIndex = -1);

public: // For. object
//	HRESULT Get_Layer();


public:	// For. Task
	// 피킹시 각 타일에서 자신을 할일로 등록한다.
	// 인터페이스 함수
	HRESULT Add_Task_Tile(_uint index);
	HRESULT Add_Task_Gold(_uint index);
	HRESULT Add_Task_WorldMove(_float3 WorldPos);
	HRESULT Add_Task_Player_WorldAttack(CGameObject_Base* target);

	HRESULT Check_Task();

private:
	// 테스크에 따른 유닛 할당
	_bool Task_Trigger(TASKBASE* task);
	_bool Task_Mine(TASKBASE* task);
	_bool Task_Player_Move_World(TASKBASE* task);
	_bool Task_Player_Attack_World(TASKBASE* task);



private:
	// 던전에 들어가는 오브젝트들 생성
	class CDungeon_Objects*		mDungeon_Objects = nullptr;
	// 던전의 할일 목록
	class CDungeon_Task*		mDungeon_TaskMgr = nullptr;

	E_GAMEMODE					meCurrentGameMode;
	class CCamera_Client*		mGameCamera= nullptr;

	ID3D11Device*				mDevice = nullptr;
	ID3D11DeviceContext*		mDeviceContext = nullptr;

public: // DEBUG
	_float						mDebugValue = 5;

public: // SETTING
	const _float3 mDungeonDoorGoalPos = _float3(22.4f, 0.010f, 23.5f);
	const _float3 mDungeonDoorStartPos = _float3(22, 0.010f, 28.5f);
	const _float3 mWorldDoorStartPos = _float3(59.3f, 8.72f, 4.15f);
	const _float3 mWorldDoorGoalPos= _float3(52.24f, 8.72f, 11.3f);




public:
	static CDungeon_Manager* Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	virtual void Free() override;
};

END