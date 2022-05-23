#pragma once

#include "Base.h"

BEGIN(Client)

class	CGameObject_MyTerrain;
class	CGameObject_3D_Tile;
class	CGameObject_3D_Dynamic;


// ���� ������Ʈ ���� ����
// �ʱ�ȭ Ÿ�� ������ �� ����� �ű�
class CDungeon_Objects final :
	public CBase
{

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


	HRESULT Setup_Terrain_Tool(CGameObject_MyTerrain* toolmap);
	const list<_uint>* Get_ListRemoveTile() const { return  mListRemoveTile; }

	HRESULT Setup_DungeonTerrain_Data(TERRAIN_DESC* toolmap);
	HRESULT Setup_DungeonWorld_Data(TERRAIN_DESC* toolmap);
	HRESULT ResetTile_Tool(TERRAIN_DESC * data);


private: // ����
	// Ÿ�� ����
	HRESULT Setup_Terrain();
	HRESULT Create_Tiles(E_LEVEL level);
	HRESULT Setup_Neigbor_Tile();
	HRESULT Setup_Collision_Navi2Tile();
	HRESULT Update_Collision_Navi2Tile(_uint index);

private:
	HRESULT Release_Objects();

private: // value Tile
	_uint								mSizeX = 0;
	_uint								mSizeZ = 0;
	// ������ Terrain ����
	CGameObject_MyTerrain*				mDungeonMap = nullptr;
	// Ÿ�ϰ�ü ���� ����
	list<CGameObject_3D_Tile*>*			mListVecTiles = nullptr;

	list<_uint>*						mListRemoveTile = nullptr;


private:
	CGameObject_3D_Dynamic*				mTestUnit = nullptr;


private:
	// ������Ʈ
	E_LEVEL								mCurrentLevel = LEVEL_END;
	ID3D11Device*						mDevice = nullptr;
	ID3D11DeviceContext*				mDeviceContext = nullptr;


	// �� ������ �ʿ��� ������ �޴´�.
	TERRAIN_DESC*						mDesc_DungeonMap =nullptr;
	TERRAIN_DESC*						mDesc_WorldMap = nullptr;



public:
	static CDungeon_Objects* Create(ID3D11Device* device, ID3D11DeviceContext* context, E_LEVEL level);
	virtual void Free() override;
};

END