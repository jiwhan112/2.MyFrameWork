#pragma once

#include "Base.h"

BEGIN(Client)
class CGameObject_MyTerrain;
class CGameObject_3D_Tile;

// ����Ŭ����
// ���ӿ� ���� �������� ó�� ����
// ������ / ������ / 
// �÷��̾� ó��(���콺)
// ���ӳ��� ��� ��ü ������ 

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

	// ���� ����������
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
	// ��ŷ�� �� Ÿ�Ͽ��� �ڽ��� ���Ϸ� ����Ѵ�.
	// �������̽� �Լ�
	HRESULT Add_Task_Tile(_uint index);
	HRESULT Add_Task_Gold(_uint index);
	HRESULT Check_Task();

private:
	// �׽�ũ�� ���� ���� �Ҵ�
	_bool Task_Trigger(class MyTask* task);
	_bool Task_Mine(class MyTask* task);



private:
	// ������ ���� ������Ʈ�� ����
	class CDungeon_Objects*		mDungeon_Objects = nullptr;
	// ������ ���� ���
	class CDungeon_Task*		mDungeon_TaskMgr = nullptr;

	E_GAMEMODE					meCurrentGameMode;
	class CCamera_Client*		mGameCamera= nullptr;

	ID3D11Device*				mDevice = nullptr;
	ID3D11DeviceContext*		mDeviceContext = nullptr;

public: // DEBUG
	_float						mDebugValue = 5;

public:
	static CDungeon_Manager* Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	virtual void Free() override;
};

END