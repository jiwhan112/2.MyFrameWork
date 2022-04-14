#include "stdafx.h"
#include "FIleIO/GameObject_Creater.h"
#include "FIleIO/ObjectIO.h"
#include "GameObject/GameObject_2D.h"


IMPLEMENT_SINGLETON(CGameObject_Creater);


CGameObject_Creater::CGameObject_Creater()
{
	mObjectIO = nullptr;
	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;
}

HRESULT CGameObject_Creater::NativeConstruct()
{
	// LoaderDatFile_For_PrototypeObject();
	return S_OK;
}

HRESULT CGameObject_Creater::Set_Device(ID3D11Device * d, ID3D11DeviceContext * c)
{
	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
	{
		m_pDevice = d;
		m_pDeviceContext = c;
		Safe_AddRef(m_pDevice);
		Safe_AddRef(m_pDeviceContext);
	}
	return S_OK;
}

HRESULT CGameObject_Creater::LoaderDatFile_For_PrototypeObject()
{
	if (mObjectIO == nullptr)
		mObjectIO = CObjectIO::Create();
	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
		return E_FAIL;
	if (mMap_GameObjectClones.empty() == false)
	{
		for (auto& obj : mMap_GameObjectClones)
		{
			Safe_Release(obj.second);
		}
		mMap_GameObjectClones.clear();
	}

	// 모든 dat 파일 로드
	list<MYFILEPATH*> listpngpath = GetSingle(CGameInstance)->Load_ExtensionList(STR_FILEPATH_RESOURCE_DATPATHTXT_L, "dat");


	for (auto& path :listpngpath)
	{
		E_OBJECT_TYPE type = OBJECT_TYPE_END;
		char* buf = nullptr;

		// buf에서 데이터가 넘어온다. / type정보로 오브젝트 원형제작
		mObjectIO->LoadObject(STR_FILEPATH_RESOURCE_DAT_L, path->FileName, &buf,&type);

		FAILED_CHECK(Create_ObjectProto_Type(type, buf, path->FileName));

		Safe_Delete_Array(buf);
		Safe_Delete(path);
	}
	listpngpath.clear();


	// 오브젝트에 따라 원형 객체 생성

	// 파일이름으로 원형오브젝트 이름저장

	return S_OK;
}

HRESULT CGameObject_Creater::Create_ObjectProto_Type(const E_OBJECT_TYPE type, const char * data, wstring protoname)
{
	_uint offset = 0;
	CGameInstance* GameInstance = GetSingle(CGameInstance);

	CGameObject* obj = nullptr;

	switch (type)
	{
	case OBJECT_TYPE_2D:
	{
		offset = sizeof(UIDESC);

		UIDESC UIDesc;
		memcpy(&UIDesc, data, sizeof(UIDESC));
		TEXTUREDESC TexDesc;
		memcpy(&TexDesc, data + offset, sizeof(TEXTUREDESC));

		// 깡통오브젝트에 DESC정보를 클론 맵에 저장한다.
		obj = GameInstance->Create_GameObject(TAGOBJ(GAMEOBJECT_2D));
		CGameObject_2D* obj2D = static_cast<CGameObject_2D*>(obj);
		obj2D->Set_LoadUIDesc(UIDesc);
		obj2D->Set_LoadTexDesc(TexDesc);
	}
	break;
	case OBJECT_TYPE_END:
		return E_FAIL;
		break;
	default:
		return E_FAIL;
		break;

	}
	mMap_GameObjectClones.emplace(protoname, obj);
	
	return S_OK;
}

HRESULT CGameObject_Creater::Create_ObjectClone_Prefab(_uint levelindex, wstring cloneName, wstring layertag)
{

	// 복사된 오므젝트 레이어에 추가
	CGameObject* findobject = Find_MapObject(cloneName);
	if (findobject)
	{
		CGameObject* newObj = findobject->Clone(nullptr);
		GetSingle(CGameInstance)->Push_Object(levelindex, layertag.c_str(), newObj);
	}
	return S_OK;
}

CGameObject * CGameObject_Creater::Find_MapObject(wstring key)
{
	auto iter =  mMap_GameObjectClones.find(key);
	if (iter != mMap_GameObjectClones.end())
		return iter->second;
	return nullptr;
}

void CGameObject_Creater::Free()
{

	Safe_Release(mObjectIO);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	if (mMap_GameObjectClones.empty() == false)
	{
		for (auto& obj : mMap_GameObjectClones)
		{
			Safe_Release(obj.second);
		}
		mMap_GameObjectClones.clear();
	}
}
