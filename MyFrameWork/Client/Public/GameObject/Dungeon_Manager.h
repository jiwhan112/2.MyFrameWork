#pragma once

#include "Base.h"

BEGIN(Client)
class CGameObject_MyTerrain;
class CGameObject_3D_Tile;

// ����Ŭ����
// ���ӿ� ���� �������� ó�� ����
// ������ / ������ / 
// �÷��̾� ó��(���콺)
// ���ӳ��� ��� ��ü ������ 

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
	// �������� �ʱ�ȭ
	HRESULT Init_GameLevel(E_LEVEL level);

	// ���� ����������
	void Release_DaungonData();

public: // Public Tile
	// Ÿ�� ����
	HRESULT Setup_Terrain(CGameObject_MyTerrain* terrain,E_LEVEL level);
	// Ÿ�� ã��
	CGameObject_3D_Tile* FInd_TIleForIndex(_int TileIndex) const;
	// Ÿ�� �����
	HRESULT RemoveTile(CGameObject_3D_Tile* pTIle);
	// ���� �μ����� �� �� �ְ� 
	HRESULT Update_NaviMesh_STOPSetting(_uint TileIndex);

private: // Private Tile
	HRESULT Init_Tile(E_LEVEL level);
	HRESULT Init_NaviMesh_STOPSetting();
	HRESULT Set_Neigbor_Tile();
	HRESULT Update_TileState(_int TileIndex = -1);
	// �����¿� �˻� �ؼ� ť�� Ÿ�� ����
	HRESULT Init_TileSet(); // ��ü Ÿ�� �ʱ�ȭ


	// ������ ���� ������Ʈ�� ����
	class CDungeon_Objects*		mGameAllObjects;


protected: // value Tile
	_uint mSizeX=0;
	_uint mSizeZ=0;
	// ������ Terrain ����
	CGameObject_MyTerrain*					mDaungonTerrain = nullptr;
	// Ÿ�ϰ�ü ���� ����
	list<CGameObject_3D_Tile*>*			mListVecTiles = nullptr;

protected:


public:
	static CDungeon_Manager* Create();
	virtual void Free() override;
};

END