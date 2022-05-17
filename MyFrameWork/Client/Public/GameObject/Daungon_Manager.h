#pragma once

#include "Base.h"

BEGIN(Client)
class CGameObject_MyTerrain;
class CGameObject_3D_Tile;

// ���� ���� ���� ������Ʈ
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
	// Ÿ�� ����
	HRESULT Setup_Terrain(CGameObject_MyTerrain* terrain);

	// ���� ����������
	void Release_DaungonData();


protected:
	HRESULT Init_Tile();
	HRESULT Set_Neigbor_Tile();


protected:
	// �����¿� �˻� �ؼ� ť�� Ÿ�� ����
	HRESULT Init_TileSet(); // ��ü Ÿ���� �˻�
	HRESULT Update_TileSet(); // �ش� Ÿ�ϸ� �˻�
	HRESULT Update_TerrainTile();



private:
//	void Update_PickPos(_float3 pickPos);
//	void Update_TileVec(int x, int z);

protected:
	_uint mSizeX=0;
	_uint mSizeZ=0;

	// ������ Terrain ����
	CGameObject_MyTerrain*					mDaungonTerrain = nullptr;

	// Ÿ�ϰ�ü ���� ����
	vector<CGameObject_3D_Tile*>*			mVecTiles = nullptr;


public:
	static CDaungon_Manager* Create();
	virtual void Free() override;
};

END