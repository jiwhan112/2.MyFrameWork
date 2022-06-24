#pragma once

#include "Base.h"

BEGIN(Client)

class	CGameObject_MyTerrain;
class	CGameObject_3D_Tile;
class	CGameObject_3D_Static;
class	CGameObject_3D_Dynamic;
class	CGameObject_Environment;


// 게임 오브젝트 생성 저장
// 초기화 타일 생성등 다 여기로 옮김
class CDungeon_Objects final :
	public CBase
{
public:
	enum E_SHADERPASS_INTERACTIVE
	{
		// 각 던전 / 월드에 있는 문
	E_SHADERPASS_INTERACTIVE_HEART01=4, // 발광
	E_SHADERPASS_INTERACTIVE_HEART02, // Hit
	E_SHADERPASS_INTERACTIVE_DOOR, // Default
	};


	enum E_STATICOBJECT
	{
		// 각 던전 / 월드에 있는 문
		STATICOBJECT_DUNGEONDOOR,
		STATICOBJECT_WORLDDOOR,
		STATICOBJECT_HEART,
		STATICOBJECT_END
	};

protected:
	explicit CDungeon_Objects();
	virtual ~CDungeon_Objects() = default;

public:


private:
	HRESULT NativeConstruct_Prototype(ID3D11Device* device, ID3D11DeviceContext* context, E_LEVEL level);
	HRESULT Ready_IMGUI();

	// 세팅
	HRESULT Ready_Light();
	HRESULT Ready_UI();
	HRESULT Ready_Camera();
	HRESULT Ready_BackGround();
	HRESULT Ready_GameObjects();
	HRESULT Ready_StaticObject();


public: // 지형
	// 타일 찾기
	CGameObject_3D_Tile* Find_TileForIndex(_int TileIndex) const;
	// 타일 지우기
	HRESULT RemoveTile(CGameObject_3D_Tile* pTIle);
	// 타일 업데이트
	HRESULT Setup_TileState(_int tileIndex = -1);

	HRESULT Setup_Terrain_Daungeon(CGameObject_MyTerrain* toolmap = nullptr);
	HRESULT Setup_Terrain_World(CGameObject_MyTerrain* toolmap = nullptr);
	HRESULT ResetTile_Tool(TERRAIN_DESC * data);

	const list<CGameObject_3D_Tile*>*	Get_TileList() const { return mListVecTiles; }

	CGameObject_MyTerrain*				Get_DungeonMap() { return mDungeonMap; }
	CGameObject_MyTerrain*				Get_WorldMap() { return mWorldMap; }

public: // 유닛
	HRESULT								Create_Unit(E_TAYGAMEOBJECT id, _float3 PositionXZ);
	list<CGameObject_Base*>				Get_ListObjecID(E_OBJECT_TYPE id);
	list<CGameObject_3D_Dynamic*>		Get_ListUnitID(E_UNITTYPE id);
	CGameObject_3D_Static*				Get_ModelName(string name, CGameObject_3D_Static* samename = nullptr);
	CGameObject_3D_Static*				Get_InterActiveModel(E_STATICOBJECT e)
	{
		return mStatic_InterActive_Objects[e];
	}

	list<CGameObject_3D_Dynamic*>*			Get_UnitList_Dungeon();
	list<CGameObject_3D_Dynamic*>*			Get_UnitList_World();

	void Add_EnemyDieCount() { 
		if (mIsCreateBoss)
			return;
		mEnemyDieCount++;
		if (mEnemyDieCount >= 3)
		{
			Create_Boss();
			mIsCreateBoss = true;
		}
	
	}

private: // 지형
	HRESULT Create_Tiles(E_LEVEL level);
	HRESULT Setup_Neigbor_Tile();
	HRESULT Setup_Collision_Navi2Tile();
	HRESULT Setup_Collision_Navi2Object();
	HRESULT Update_Collision_Navi2Tile(_uint index);

private:
	HRESULT Release_Objects();

private:
	// 레벨 세팅
	//HRESULT Init_GameScene_Dungeon();
	//HRESULT Init_GameScene_World();
	//HRESULT Init_ToolScene_Tool();


private: // value Tile
	_uint								mSizeX = 0;
	_uint								mSizeZ = 0;
	// 생성된 Terrain 저장
	CGameObject_MyTerrain*				mDungeonMap = nullptr;
	// 생성된 Terrain 저장
	CGameObject_MyTerrain*				mWorldMap = nullptr;
	// 타일객체 벡터 저장
	list<CGameObject_3D_Tile*>*			mListVecTiles = nullptr;

	list<CGameObject_3D_Dynamic*>*		mListUnitDanumgeonMap = nullptr;
	list<CGameObject_3D_Dynamic*>*		mListUnitWorldMap= nullptr;

private: // 상호작용하는 Static 오브젝트
	CGameObject_3D_Static*				mStatic_InterActive_Objects[E_STATICOBJECT::STATICOBJECT_END];

private: // 상호작용 트리거 볼륨


private:
	static void Create_Mine();
	static void Create_Orc();
	static void Create_Goblin();
	static void Create_Enemy();
	static void Create_Boss();
	static void Move_Dungeon();


private:
	// 오브젝트
	E_LEVEL								mCurrentLevel = LEVEL_END;
	ID3D11Device*						mDevice = nullptr;
	ID3D11DeviceContext*				mDeviceContext = nullptr;

	_int								mEnemyDieCount = 0;
	bool								mIsCreateBoss = false;



public:
	static CDungeon_Objects* Create(ID3D11Device* device, ID3D11DeviceContext* context, E_LEVEL level);
	virtual void Free() override;
};

END