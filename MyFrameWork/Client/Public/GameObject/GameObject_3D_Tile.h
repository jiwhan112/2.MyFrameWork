#pragma once

#include "GameObject_3D_Static.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

// 타일 오브젝트
class CGameObject_3D_Tile  final:
	public CGameObject_3D_Static
{
public:
	// 업데이트 모드
	enum E_TICKTYPE
	{
		// 기본
		E_TICKE_NONE,
		E_TICKE_PICK, // 피킹
		E_TICKE_TASK, // 피킹시 일꾼에 전달
		E_TICKE_END
	};
	
	enum E_TILETYPE
	{
		TILETYPE_TOP,
		TILETYPE_WALL,
		TILETYPE_CONER,
		TILETYPE_FLOOR,
		TILETYPE_BEHIND,
		TILETYPE_DIAG,
		TILETYPE_END
	};

	// 보이는 면만 깐다.
	enum E_TILESTATE
	{
		TILESTATE_TOP,
		TILESTATE_WALL_LEFT,
		TILESTATE_WALL_TOP,
		TILESTATE_WALL_RIGHT,
		TILESTATE_WALL_BOTTOM,
		TILESTATE_CONER_LB,
		TILESTATE_CONER_LT,
		TILESTATE_CONER_RT,
		TILESTATE_CONER_RB,
		TILESTATE_FLOOR,

		TILESTATE_BEHIND_LEFT,
		TILESTATE_BEHIND_TOP,
		TILESTATE_BEHIND_RIGHT,
		TILESTATE_BEHIND_BOTTOM,

		TILESTATE_DIAGTEST_LEFT,
		TILESTATE_DIAGTEST_TOP,
		TILESTATE_DIAGTEST_RIGHT,
		TILESTATE_DIAGTEST_BOTTOM,


		TILESTATE_END, 
	};

	enum E_NEIGHBOR_TILE
	{
		NEIGHBOR_TILE_LEFT,
		NEIGHBOR_TILE_TOP,
		NEIGHBOR_TILE_RIGHT,
		NEIGHBOR_TILE_BOTTOM,
		// 왼쪽 오른쪽 위
		NEIGHBOR_TILE_LT,
		NEIGHBOR_TILE_RT,
		NEIGHBOR_TILE_RB,
		NEIGHBOR_TILE_LB,
		NEIGHBOR_TILE_END
	};

	enum E_TILE_DIGTYPE
	{
		TILE_DIGTYPE_WALL,
		TILE_DIGTYPE_GOLD,
		TILE_DIGTYPE_END,

	};



protected:
	explicit CGameObject_3D_Tile(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Tile(const CGameObject_3D_Tile& rhs);
	virtual ~CGameObject_3D_Tile() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	//virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render()override;
	virtual HRESULT CollisionFunc(_float3 PickPosition, _float dist) override;

	void Add_TileTask_this();

	HRESULT Set_LoadNewFBX(E_TILETYPE type);
	void Set_TileIndex(_int index) { mIndex = index; }
	_int Get_TileIndex()const { return mIndex; }

	void Set_TileTickState(E_TICKTYPE e) { meTileTickState = e; }
	void Set_TileIndex(E_NEIGHBOR_TILE e, _int index) { mNeighborIndex[e] = index; }
	bool Set_EmptySearchNeighbor(int searchIndex);
	void Set_ColliderPosition();

	_bool Get_IsBlocked()const;
	_float3 Get_AbleTilePos(_float offset);

	E_TILE_DIGTYPE Get_TileDigType()const { return meTileDigType; }
	void Set_TileDigType(E_TILE_DIGTYPE e){ meTileDigType = e; }

	HRESULT RemoveThisTile();
//	HRESULT Set_LoadModel_Deco_DESC(E_TILETYPE tileIndex);
public:

	// 타일의 이웃정보에 따라 자신의 상태 변경
	HRESULT Update_NeighborTile();
	HRESULT Update_Tile_CrossCheck();
	

protected:
	void Update_TILESTATE();
	void Update_Debug_TILESTATE();


protected:
	// 타일 인덱스
	_int mIndex = -1;

	// 이웃하는 타일 인덱스
	_int mNeighborIndex[NEIGHBOR_TILE_END] = { -1,-1,-1,-1,-1,-1,-1,-1 };
	
	// 타일 FBX 이름	
	string mTileNames[TILETYPE_END];

	E_TILESTATE	meTileState = TILESTATE_TOP;
	E_TICKTYPE meTileTickState = E_TICKE_NONE;
	E_TILE_DIGTYPE meTileDigType = TILE_DIGTYPE_WALL;

//	CModel* mComModel_deco = nullptr;



public:
	static CGameObject_3D_Tile* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Tile* Clone(void* pArg);
	virtual void Free() override;
};

END

