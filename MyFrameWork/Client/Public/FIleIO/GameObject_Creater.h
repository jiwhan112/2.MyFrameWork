#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

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

	const map<wstring, CGameObject*>* Get_Map_GameObject2File_Proto() const
	{
		return &mMap_GameObject2File_Proto;
	}

	// Ÿ�Ժ��� Ű ����Ʈ ��ȯ
	list<string>* Get_MapObject_Type(E_OBJECT_TYPE type);


	// ��� �׽�Ʈ������ public ��Ŵ
public:
	// dat���Ϸ� ����� �����͸� �ε��ؼ� ���밴ü�� ������ �Է��� Ŭ�� ��ü ����
	HRESULT LoaderDatFile_For_PrototypeObject();

	// �������Ʈ ���� ObjectManager �� �������
	CGameObject* CreateEmptyObject(const E_TAYGAMEOBJECT type);

	// �ҷ��� �����ͷ� Ŭ�� ������Ʈ �����ϰ� �ʿ� ����
	HRESULT Create_ObjectProto_Type(const E_OBJECT_TYPE type, const char* data, wstring protoname);

	// �ʿ� ����� �����ͷ� ������ Ŭ�а�ü �����غ�
	CGameObject* Create_ObjectClone_Prefab(_uint levelindex, wstring cloneName, wstring layertag);

//	HRESULT Create_ObjectClone_Prefab_AllData(_uint levelindex, wstring layertag);

	

	
private:
	CGameObject* Find_MapObject(wstring key);


private:	
	// ������ �ε�
	class CObjectIO* mObjectIoManager = nullptr;

	// ������ ������� ������ ���� ������Ʈ�� Ŭ�� ������ �����Ѵ�.
	map<wstring, CGameObject*> mMap_GameObject2File_Proto;

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

public:
	static CGameObject_Creater* Create(ID3D11Device* d, ID3D11DeviceContext* c);
	virtual void Free() override;

};
END