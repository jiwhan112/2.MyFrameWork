#pragma once

#include "Base.h"

BEGIN(Client)
class CGameObject_MyTerrain;
class CGameObject_3D_Tile;

// 던전 지형 관리 오브젝트
class CDaungon_Manager final :
	public CBase
{

protected:
	explicit CDaungon_Manager();
	virtual ~CDaungon_Manager() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();

	virtual _int Tick(_double TimeDelta);



public: // DESC
	// 타일 생성
	HRESULT Setup_Terrain(CGameObject_MyTerrain* terrain,E_LEVEL level);

	// 레벨 나갈떄마다
	void Release_DaungonData();

	// 타일 찾기
	CGameObject_3D_Tile* FInd_TIleForIndex(_int TileIndex) const;
	// 타일 지우기
	HRESULT RemoveTile(CGameObject_3D_Tile* pTIle);
	// 벽이 부서지면 갈 수 있게 
	HRESULT Update_NaviMesh_STOPSetting(_uint TileIndex);

private:
	HRESULT Init_Tile(E_LEVEL level);
	HRESULT Init_NaviMesh_STOPSetting();
	HRESULT Set_Neigbor_Tile();
	HRESULT Update_TileState(_int TileIndex = -1);

	// 상하좌우 검사 해서 큐브 타일 설정
	HRESULT Init_TileSet(); // 전체 타일 초기화



protected:
	_uint mSizeX=0;
	_uint mSizeZ=0;

	// 생성된 Terrain 저장
	CGameObject_MyTerrain*					mDaungonTerrain = nullptr;

	// 타일객체 벡터 저장
	list<CGameObject_3D_Tile*>*			mListVecTiles = nullptr;


public:
	static CDaungon_Manager* Create();
	virtual void Free() override;
};

END