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

	DECLARE_SINGLETON(CGameObject_Creater);

private:
	explicit	CGameObject_Creater();
	virtual		~CGameObject_Creater() = default;

public:
	HRESULT NativeConstruct();

public:
	HRESULT Set_Device(ID3D11Device* d, ID3D11DeviceContext* c);

	const map<wstring, CGameObject*>& Get_Map_GameObjectClones() const
	{
		return mMap_GameObjectClones;
	}


	// ��� �׽�Ʈ������ public ��Ŵ
public:
	// dat���Ϸ� ����� �����͸� �ε��ؼ� ���밴ü�� ������ �Է��� Ŭ�� ��ü ����
	HRESULT LoaderDatFile_For_PrototypeObject();

	// �ҷ��� �����ͷ� Ŭ�� ������Ʈ �����ϰ� �ʿ� ����
	HRESULT Create_ObjectProto_Type(const E_OBJECT_TYPE type, const char* data, wstring protoname);

	// �ʿ� ����� �����ͷ� ������ Ŭ�а�ü �����غ�
	HRESULT Create_ObjectClone_Prefab(_uint levelindex, wstring cloneName, wstring layertag);

	
private:
	CGameObject* Find_MapObject(wstring key);

private:	
	// ������ �ε�
	class CObjectIO* mObjectIO = nullptr;

	// ������ ������� ������ ���� ������Ʈ�� Ŭ�� ������ �����Ѵ�.
	map<wstring, CGameObject*> mMap_GameObjectClones;

	
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;

};
END