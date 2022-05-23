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
	virtual HRESULT NativeConstruct(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);

public: //GET SET
	class CDungeon_Objects* Get_DungeonObjects() const { return mDungeon_Objects; }

public:
	HRESULT NativeConstruct_Level(E_LEVEL level);

	// 레벨 나갈떄마다
	void Release_DaungonData();

public: // For. Tile
	CGameObject_3D_Tile* FInd_TIleForIndex(_int TileIndex) const;
	HRESULT RemoveTile(CGameObject_3D_Tile* pTIle);
	HRESULT Setup_TileState(_int tileIndex = -1);


private:
	// 던전에 들어가는 오브젝트들 생성
	class CDungeon_Objects*		mDungeon_Objects = nullptr;

	ID3D11Device*			mDevice = nullptr;
	ID3D11DeviceContext*	mDeviceContext = nullptr;

public:
	static CDungeon_Manager* Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	virtual void Free() override;
};

END