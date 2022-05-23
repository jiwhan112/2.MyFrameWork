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

class CDungeon_Manager final :
	public CBase
{

protected:
	explicit CDungeon_Manager();
	virtual ~CDungeon_Manager() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);

public:
	// 레벨마다 초기화
	HRESULT Init_GameLevel(E_LEVEL level);

	// 레벨 나갈떄마다
	void Release_DaungonData();

public: // Public Tile
	// 타일 생성
	HRESULT Setup_Terrain(CGameObject_MyTerrain* terrain,E_LEVEL level);
	// 타일 찾기
	CGameObject_3D_Tile* FInd_TIleForIndex(_int TileIndex) const;
	// 타일 지우기
	HRESULT RemoveTile(CGameObject_3D_Tile* pTIle);
	// 벽이 부서지면 갈 수 있게 
	HRESULT Update_NaviMesh_STOPSetting(_uint TileIndex);

private: // Private Tile
	HRESULT Init_Tile(E_LEVEL level);
	HRESULT Init_NaviMesh_STOPSetting();
	HRESULT Set_Neigbor_Tile();
	HRESULT Update_TileState(_int TileIndex = -1);
	// 상하좌우 검사 해서 큐브 타일 설정
	HRESULT Init_TileSet(); // 전체 타일 초기화


	// 던전에 들어가는 오브젝트들 생성
	class CDungeon_Objects*		mGameAllObjects;


protected: // value Tile
	_uint mSizeX=0;
	_uint mSizeZ=0;
	// 생성된 Terrain 저장
	CGameObject_MyTerrain*					mDaungonTerrain = nullptr;
	// 타일객체 벡터 저장
	list<CGameObject_3D_Tile*>*			mListVecTiles = nullptr;

protected:


public:
	static CDungeon_Manager* Create();
	virtual void Free() override;
};

END