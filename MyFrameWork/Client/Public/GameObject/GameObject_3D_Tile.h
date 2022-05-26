#pragma once

#include "GameObject_3D_Static.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

// Ÿ�� ������Ʈ
class CGameObject_3D_Tile  final:
	public CGameObject_3D_Static
{
public:
	// ������Ʈ ���
	enum E_UPDATETYPE
	{
		// �⺻
		E_UPDATETYPE_NONE,
		E_UPDATETYPE_PICK, // ��ŷ�� �ϲۿ� ����
		E_UPDATETYPE_END
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

	// ���̴� �鸸 ���.
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
		// ���� ������ ��
		NEIGHBOR_TILE_LT,
		NEIGHBOR_TILE_RT,
		NEIGHBOR_TILE_END
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


	HRESULT Set_LoadNewFBX(E_TILETYPE type);
	void Set_NeighborTileIndex(_int index) { mIndex = index; }
	_int Get_TileIndex()const { return mIndex; }

	void Set_NeighborTileIndex(E_NEIGHBOR_TILE e, _int index) { mNeighborIndex[e] = index; }
	bool Set_EmptySearchNeighbor(int searchIndex);
	void Set_ColliderPosition();

//	HRESULT Set_LoadModel_Deco_DESC(E_TILETYPE tileIndex);
public:

	// Ÿ���� �̿������� ���� �ڽ��� ���� ����
	HRESULT Update_NeighborTile();

protected:
	void Update_TILESTATE();
	void Update_Debug_TILESTATE();


protected:
	// Ÿ�� �ε���
	_int mIndex = -1;

	// �̿��ϴ� Ÿ�� �ε���
	_int mNeighborIndex[NEIGHBOR_TILE_END] = { -1,-1,-1,-1,-1,-1 };
	
	// Ÿ�� FBX �̸�	
	string mTileNames[TILETYPE_END];

	E_TILESTATE	meTileState = TILESTATE_TOP;

//	CModel* mComModel_deco = nullptr;



public:
	static CGameObject_3D_Tile* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Tile* Clone(void* pArg);
	virtual void Free() override;
};

END

