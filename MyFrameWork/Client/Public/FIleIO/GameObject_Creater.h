#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

// 깡통오브젝트를 외부파일정보에 맞게 생성한다.
// 부모 자식관계가 있다면 해당 형태로 만들어서 넘긴다.
// 원형 오브젝트 생성기라고 정의한다.

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

	// 타입별로 키 리스트 반환
	list<string>* Get_MapObject_Type(E_OBJECT_TYPE type);


	// 기능 테스트용으로 public 시킴
public:
	// dat파일로 저장된 데이터를 로드해서 깡통객체에 정보를 입력해 클론 객체 생성
	HRESULT LoaderDatFile_For_PrototypeObject();

	// 빈오브젝트 생성 ObjectManager 와 같은기능
	CGameObject* CreateEmptyObject(const E_TAYGAMEOBJECT type);

	// 불러온 데이터로 클론 오브젝트 생성하고 맵에 저장
	HRESULT Create_ObjectProto_Type(const E_OBJECT_TYPE type, const char* data, wstring protoname);

	// 맵에 저장된 데이터로 씬에서 클론객체 생성해봄
	CGameObject* Create_ObjectClone_Prefab(_uint levelindex, wstring cloneName, wstring layertag);

//	HRESULT Create_ObjectClone_Prefab_AllData(_uint levelindex, wstring layertag);

	

	
private:
	CGameObject* Find_MapObject(wstring key);


private:	
	// 데이터 로드
	class CObjectIO* mObjectIoManager = nullptr;

	// 데이터 기반으로 생성된 깡통 오브젝트의 클론 정보를 보관한다.
	map<wstring, CGameObject*> mMap_GameObject2File_Proto;

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

public:
	static CGameObject_Creater* Create(ID3D11Device* d, ID3D11DeviceContext* c);
	virtual void Free() override;

};
END