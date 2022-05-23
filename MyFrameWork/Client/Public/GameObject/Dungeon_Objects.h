#pragma once

#include "Base.h"

BEGIN(Client)

class	CGameObject_MyTerrain;
class	CGameObject_3D_Tile;
class	CGameObject_3D_Dynamic;


// 게임 오브젝트 생성 저장
// 초기화 타일 생성등 다 여기로 옮김
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


	HRESULT Setup_Terrain_Tool(CGameObject_MyTerrain* toolmap);
	const list<_uint>* Get_ListRemoveTile() const { return  mListRemoveTile; }

	HRESULT Setup_DungeonTerrain_Data(TERRAIN_DESC* toolmap);
	HRESULT Setup_DungeonWorld_Data(TERRAIN_DESC* toolmap);
	HRESULT ResetTile_Tool(TERRAIN_DESC * data);


private: // 지형
	// 타일 세팅
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
	// 생성된 Terrain 저장
	CGameObject_MyTerrain*				mDungeonMap = nullptr;
	// 타일객체 벡터 저장
	list<CGameObject_3D_Tile*>*			mListVecTiles = nullptr;

	list<_uint>*						mListRemoveTile = nullptr;


private:
	CGameObject_3D_Dynamic*				mTestUnit = nullptr;


private:
	// 오브젝트
	E_LEVEL								mCurrentLevel = LEVEL_END;
	ID3D11Device*						mDevice = nullptr;
	ID3D11DeviceContext*				mDeviceContext = nullptr;


	// 맵 생성에 필요한 정보를 받는다.
	TERRAIN_DESC*						mDesc_DungeonMap =nullptr;
	TERRAIN_DESC*						mDesc_WorldMap = nullptr;



public:
	static CDungeon_Objects* Create(ID3D11Device* device, ID3D11DeviceContext* context, E_LEVEL level);
	virtual void Free() override;
};

END