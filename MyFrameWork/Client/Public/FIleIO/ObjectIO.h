#pragma once

#include "Base.h"

BEGIN(Client)

// 클라이언트의 DESC 정보 저장 불러오기 클래스
// 바이너리로 DESC파일 저장 및 불러오기 수행

// 불러온 데이터 전역 저장
class CGameObject_Base;

class CObjectIO final
	:public CBase
{
private:
	explicit	CObjectIO() = default;
	explicit	CObjectIO(const CObjectIO& rhs) = default;
	virtual		~CObjectIO() = default;

public:
	HRESULT NativeConstruct();

public:
	// 오브젝트 타입별로 저장 수행
	HRESULT SaverObject(E_OBJECT_TYPE type, wstring FolderPath, wstring filename, CGameObject_Base* obj);


	// 데이터를 불러와서 생성기에 넘김
	HRESULT LoadObject_Create(wstring FolderPath, wstring filename);

	// Creater 맵에 원형 저장
	bool Create_CreateMap_ProtoType(HANDLE& hFile, wstring keyname);
	
	// 오브젝트를 desc타입 별로 나눠서 저장한다.
	HRESULT				Save_DESC(E_DESC_DATA descid, wstring FolderPath, wstring filename, void* desc);
	HRESULT				Load_DESC(E_DESC_DATA descid, wstring FolderPath, wstring filename, wstring Extension);
	wstring				Get_EXEDescName(E_DESC_DATA descid);

public: // Terrain Desc
	HRESULT					Add_TerrainDesc(wstring key, TERRAIN_DESC* value);
	TERRAIN_DESC*			Find_TerrainDesc(wstring key);

private:
	// DESC정보 전역 저장
	map<wstring, TERRAIN_DESC*> mMap_TerrainDesc;

public:
	static CObjectIO* Create();
	virtual void Free() override;
};

END