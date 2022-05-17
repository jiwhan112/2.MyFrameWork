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
	HRESULT Setup_Terrain(CGameObject_MyTerrain* terrain);

	// 레벨 나갈떄마다
	void Release_DaungonData();


protected:
	HRESULT Init_Tile();
	HRESULT Set_Neigbor_Tile();


protected:
	// 상하좌우 검사 해서 큐브 타일 설정
	HRESULT Init_TileSet(); // 전체 타일을 검사
	HRESULT Update_TileSet(); // 해당 타일만 검사
	HRESULT Update_TerrainTile();



private:
//	void Update_PickPos(_float3 pickPos);
//	void Update_TileVec(int x, int z);

protected:
	_uint mSizeX=0;
	_uint mSizeZ=0;

	// 생성된 Terrain 저장
	CGameObject_MyTerrain*					mDaungonTerrain = nullptr;

	// 타일객체 벡터 저장
	vector<CGameObject_3D_Tile*>*			mVecTiles = nullptr;


public:
	static CDaungon_Manager* Create();
	virtual void Free() override;
};

END