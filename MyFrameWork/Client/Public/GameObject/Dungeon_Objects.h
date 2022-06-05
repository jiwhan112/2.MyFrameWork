#pragma once

#include "Base.h"

BEGIN(Client)

class	CGameObject_MyTerrain;
class	CGameObject_3D_Tile;
class	CGameObject_3D_Static;
class	CGameObject_3D_Dynamic;
class	CGameObject_Environment;


// ���� ������Ʈ ���� ����
// �ʱ�ȭ Ÿ�� ������ �� ����� �ű�
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

	// ����
	HRESULT Ready_Light();
	HRESULT Ready_Camera();
	HRESULT Ready_BackGround();
	HRESULT Ready_GameObjects();

public: // ����
	// Ÿ�� ã��
	CGameObject_3D_Tile* FInd_TIleForIndex(_int TileIndex) const;
	// Ÿ�� �����
	HRESULT RemoveTile(CGameObject_3D_Tile* pTIle);
	// Ÿ�� ������Ʈ
	HRESULT Setup_TileState(_int tileIndex = -1);

	HRESULT Setup_Terrain_Daungeon(CGameObject_MyTerrain* toolmap = nullptr);
	HRESULT Setup_Terrain_World(CGameObject_MyTerrain* toolmap = nullptr);
	HRESULT ResetTile_Tool(TERRAIN_DESC * data);

	const list<CGameObject_3D_Tile*>*	Get_TileList() const { return mListVecTiles; }

	CGameObject_MyTerrain*				Get_DungeonMap() { return mDungeonMap; }
	CGameObject_MyTerrain*				Get_WorldMap() { return mWorldMap; }

public: // ����
	HRESULT								Create_Unit(E_TAYGAMEOBJECT id, _float3 PositionXZ);
	list<CGameObject_Base*>				Get_ListObjecID(E_OBJECT_TYPE id);
//	HRESULT Get_Unit_Map(E_TAYGAMEOBJECT id, _float3 PositionXZ);


private: // ����
	HRESULT Create_Tiles(E_LEVEL level);
	HRESULT Setup_Neigbor_Tile();
	HRESULT Setup_Collision_Navi2Tile();
	HRESULT Setup_Collision_Navi2Object();
	HRESULT Update_Collision_Navi2Tile(_uint index);

private:
	HRESULT Release_Objects();

private:
	// ���� ����
	//HRESULT Init_GameScene_Dungeon();
	//HRESULT Init_GameScene_World();
	//HRESULT Init_ToolScene_Tool();


private: // value Tile
	_uint								mSizeX = 0;
	_uint								mSizeZ = 0;
	// ������ Terrain ����
	CGameObject_MyTerrain*				mDungeonMap = nullptr;
	// ������ Terrain ����
	CGameObject_MyTerrain*				mWorldMap = nullptr;
	// Ÿ�ϰ�ü ���� ����
	list<CGameObject_3D_Tile*>*			mListVecTiles = nullptr;


private:
	CGameObject_3D_Dynamic*				mTestUnit = nullptr;


private:
	// ������Ʈ
	E_LEVEL								mCurrentLevel = LEVEL_END;
	ID3D11Device*						mDevice = nullptr;
	ID3D11DeviceContext*				mDeviceContext = nullptr;

public:
	static CDungeon_Objects* Create(ID3D11Device* device, ID3D11DeviceContext* context, E_LEVEL level);
	virtual void Free() override;
};

END