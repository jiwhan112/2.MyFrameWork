#pragma once

#include "GameObject_3D_Static.h"

BEGIN(Client)

// 타일 오브젝트
class CGameObject_3D_Tile  final:
	public CGameObject_3D_Static
{
public:
	// 업데이트 모드
	enum E_UPDATETYPE
	{
		// 기본
		E_UPDATETYPE_NONE,
		E_UPDATETYPE_PICK, // 피킹시 일꾼에 전달
		E_UPDATETYPE_END
	};
	
	enum E_TILETYPE
	{
		TILETYPE_TOP,
		TILETYPE_ONEWALL,
		TILETYPE_TOWWALL,
		TILETYPE_END
	};

	enum E_NEIGHBOR_TILE
	{
		NEIGHBOR_TILE_LEFT,
		NEIGHBOR_TILE_TOP,
		NEIGHBOR_TILE_RIGHT,
		NEIGHBOR_TILE_BOTTOM,
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
	//
	//virtual HRESULT Render();
	HRESULT Set_LoadNewFBX(E_TILETYPE type);
	


protected:
//	HRESULT Create_Model();
//	virtual HRESULT Set_Component() override;


private:
	// 이웃하는 타일
	// CGameObject_3D_Tiles* mTileObject[NEIGHBOR_TILE_END] = { nullptr, };

	// 타일 FBX 이름
	string mTileNames[TILETYPE_END];



public:
	static CGameObject_3D_Tile* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Tile* Clone(void* pArg);
	virtual void Free() override;
};

END

