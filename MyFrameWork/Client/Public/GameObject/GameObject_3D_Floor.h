#pragma once

#include "GameObject_3D_Static.h"

BEGIN(Engine)
class CVIRect;
END

BEGIN(Client)

// �ٴ� / �ǹ� ������Ʈ ���� ���

class CGameObject_3D_Floor final:
	public CGameObject_3D_Static
{
public:
	// ������Ʈ ���
	enum E_TICKTYPE
	{
		// �⺻
		TICKTYPE_NONE,		// ����
		TICKTYPE_PRE,		// �����ȵȻ���
		TICKTYPE_PICK,		// ���� ����
		TICKTYPE_END,
	};
	
	enum E_FLOORTYPE
	{
		FLOOR_FLOOR, // ��Ź Ÿ��
		FLOOR_BUILD_ORC,
		FLOOR_BUILD_GOBLIN,
		FLOOR_END,

	};

	//enum E_NEIGHBOR_TILE
	//{
	//	NEIGHBOR_TILE_LEFT,
	//	NEIGHBOR_TILE_TOP,
	//	NEIGHBOR_TILE_RIGHT,
	//	NEIGHBOR_TILE_BOTTOM,

	//};

protected:
	explicit CGameObject_3D_Floor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Floor(const CGameObject_3D_Floor& rhs);
	virtual ~CGameObject_3D_Floor() = default;

	// ������Ʈ ����
	virtual HRESULT Set_Component()override;
	virtual HRESULT Set_ConstantTable_Model()override; // �� ����


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	//virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render()override;

	HRESULT Set_LoadNewFBX(E_FLOORTYPE type);

	

public:
	//// Ÿ���� �̿������� ���� �ڽ��� ���� ����
	//HRESULT Update_NeighborTile();
	//HRESULT Update_Tile_CrossCheck();
	
	const char* FloorFbxName = "tileset_GreyRock_FLOOR.fbx";
	const char* BuildFbxName_Hideout_Goblin = "room_Hideout_Interact_Goblin.fbx";
	const char* BuildFbxName_Hideout_Orc	= "room_Hideout_Interact_Orc.fbx";

	const char* TexName1 = "WATER.dds";
	const char* TexName2 = "WATER.dds";


protected:
	E_TICKTYPE		meTickType = TICKTYPE_NONE;
	E_FLOORTYPE		meFloorType = FLOOR_END;


public:
	static CGameObject_3D_Floor* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Floor* Clone(void* pArg);
	virtual void Free() override;
};

END

