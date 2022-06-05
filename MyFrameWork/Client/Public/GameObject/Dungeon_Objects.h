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
	//enum E_LEVEL_GAMEOPTION
	//{
	//	LEVEL_GAMEOPTION_GAME,
	//	LEVEL_GAMEOPTION_GAME_DUNGEON,
	//	LEVEL_GAMEOPTION_GAME_WORLD,
	//	LEVEL_GAMEOPTION_TOOL,
	//	LEVEL_GAMEOPTION_END,
	//};

protected:
	explicit CDungeon_Objects();
	virtual ~CDungeon_Objects() = default;

public:


private:
	HRESULT NativeConstruct_Prototype(ID3D11Device* device, ID3D11DeviceContext* context, E_LEVEL level);
	HRESULT Ready_IMGUI();

	// 세팅
	HRESULT Ready_Light();
	HRESULT Ready_Camera();
	HRESULT Ready_BackGround();
	HRESULT Ready_GameObjects();

public: // 지형
	// 타일 찾기
	CGameObject_3D_Tile* FInd_TIleForIndex(_int TileIndex) const;
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
//	HRESULT Get_Unit_Map(E_TAYGAMEOBJECT id, _float3 PositionXZ);


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


private:
	CGameObject_3D_Dynamic*				mTestUnit = nullptr;


private:
	// 오브젝트
	E_LEVEL								mCurrentLevel = LEVEL_END;
	ID3D11Device*						mDevice = nullptr;
	ID3D11DeviceContext*				mDeviceContext = nullptr;

public:
	static CDungeon_Objects* Create(ID3D11Device* device, ID3D11DeviceContext* context, E_LEVEL level);
	virtual void Free() override;
};

END