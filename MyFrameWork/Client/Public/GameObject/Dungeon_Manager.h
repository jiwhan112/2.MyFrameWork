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
	virtual HRESULT NativeConstruct(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);

public: //GET SET
	class CDungeon_Objects* Get_DungeonObjects() const { return mDungeon_Objects; }

public:
	HRESULT NativeConstruct_Level(E_LEVEL level);

	// ���� ����������
	void Release_DaungonData();

public: // For. Tile
	CGameObject_3D_Tile* FInd_TIleForIndex(_int TileIndex) const;
	HRESULT RemoveTile(CGameObject_3D_Tile* pTIle);
	HRESULT Setup_TileState(_int tileIndex = -1);


private:
	// ������ ���� ������Ʈ�� ����
	class CDungeon_Objects*		mDungeon_Objects = nullptr;

	ID3D11Device*			mDevice = nullptr;
	ID3D11DeviceContext*	mDeviceContext = nullptr;

public:
	static CDungeon_Manager* Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	virtual void Free() override;
};

END