#pragma once
#include "Base.h"

BEGIN(Client)
class CGameObject_Base;

// ���������Ʈ�� �ܺ����������� �°� �����Ѵ�.
// �θ� �ڽİ��谡 �ִٸ� �ش� ���·� ���� �ѱ��.
// ���� ������Ʈ �������� �����Ѵ�.

class CGameObject_Creater final
	:public CBase
{
private:
	explicit	CGameObject_Creater();
	virtual		~CGameObject_Creater() = default;

public:
	HRESULT NativeConstruct(ID3D11Device* d, ID3D11DeviceContext* c);

public:

	const map<wstring, CGameObject_Base*>* Get_Map_GameObject2File_Proto() const
	{
		return &mMap_GameObject2File_Proto;
	}

	// Ÿ�Ժ��� Ű ����Ʈ ��ȯ
	list<string>* Get_MapObject_Type(E_OBJECT_TYPE type);


public:
	// dat���Ϸ� ����� �����͸� �ε��ؼ� ���밴ü�� ������ �Է��� Ŭ�� ��ü ����
	HRESULT LoaderDatFile_For_PrototypeObject();

	// �������Ʈ ���� ObjectManager �� �������
	CGameObject_Base* CreateEmptyObject(const E_TAYGAMEOBJECT type);
	HRESULT PushObject(CGameObject_Base** obj,_uint levelInex,wstring layertag);

	// �ҷ��� �����ͷ� Ŭ�� ������Ʈ �����ϰ� �ʿ� ����
//	HRESULT Create_ObjectProto_Type(const E_OBJECT_TYPE type, const char* data, wstring protoname);

	// �ʿ� ����� �����ͷ� ������ Ŭ�а�ü �����غ�
	CGameObject_Base* Create_ObjectClone_Prefab(_uint levelindex, wstring cloneName, wstring layertag);
	CGameObject_Base* Create_ObjectClone_Prefab_NoPush(wstring cloneName);

	//	HRESULT Create_ObjectClone_Prefab_AllData(_uint levelindex, wstring layertag);

	HRESULT  Add_MapObject(wstring keyname, CGameObject_Base* obj);
	HRESULT  Add_MapTerrainDesc(wstring keyname, TERRAIN_DESC* terraindesc);


public: // Ư�� ������Ʈ ����
//	CGameObject_Base* Create_SocketObject();
	TERRAIN_DESC*	Find_TerrainData(wstring key);

private:
	CGameObject_Base* Find_MapObject(wstring key);

private:
	// ������Ʈ ������� ������ ���� ������Ʈ�� Ŭ�� ������ �����Ѵ�.
	map<wstring, CGameObject_Base*> mMap_GameObject2File_Proto;

	// DESC ������� �� ������ ������� �����Ѵ�.
	// Ȯ���� ������� �ؼ�
	// map<wstring, void*>*			mMap_DescData;

	// DESC �������� �۾��� �ɷ��� �ϴ� �� ������ ���� �ʿ������Ѵ�.
	map<wstring, TERRAIN_DESC*>				mMap_TerrainDESC;

	ID3D11Device*							m_pDevice = nullptr;
	ID3D11DeviceContext*					m_pDeviceContext = nullptr;

public:
	static CGameObject_Creater* Create(ID3D11Device* d, ID3D11DeviceContext* c);
	virtual void Free() override;
};
END