#include "stdafx.h"
#include "FIleIO/GameObject_Creater.h"
#include "FIleIO/ObjectIO.h"
#include "GameObject/GameObject_2D.h"
#include "GameObject/GameObject_3D_Static.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_MyTerrain.h"

CGameObject_Creater::CGameObject_Creater()
{
	//	mObjectIoManager = nullptr;
	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;
}

HRESULT CGameObject_Creater::NativeConstruct(ID3D11Device * d, ID3D11DeviceContext * c)
{
	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
	{
		m_pDevice = d;
		m_pDeviceContext = c;
		Safe_AddRef(m_pDevice);
		Safe_AddRef(m_pDeviceContext);
	}
//	LoaderDatFile_For_PrototypeObject();

	return S_OK;
}

list<string>* CGameObject_Creater::Get_MapObject_Type(E_OBJECT_TYPE type)
{
	list<string>* listtype = NEW list<string>;

	if (mMap_GameObject2File_Proto.empty())
		return listtype;

	for (auto& obj : mMap_GameObject2File_Proto)
	{
		E_OBJECT_TYPE id = (E_OBJECT_TYPE)obj.second->Get_ObjectTypeID();
		if (id == type)
		{
			wstring wstr = obj.first;
			string str = CHelperClass::Convert_Wstr2str(wstr);
			listtype->push_front(str);
		}
	}
	return listtype;
}

HRESULT CGameObject_Creater::LoaderDatFile_For_PrototypeObject()
{
	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
		return E_FAIL;

	CObjectIO* ObjectIoManager = GetSingle(CGameManager)->Get_ObjectIOManager();

	// 매번 초기화
	if (mMap_GameObject2File_Proto.empty() == false)
	{
		for (auto& obj : mMap_GameObject2File_Proto)
		{
			Safe_Release(obj.second);
		}
		mMap_GameObject2File_Proto.clear();
	}

	// 모든 dat 파일 재 로드
	GetSingle(CGameManager)->Set_ReListPath(CGameManager::PATHTYPE_DATA);

	const list<MYFILEPATH*>* listDatpath = GetSingle(CGameManager)->Get_PathList(CGameManager::PATHTYPE_DATA);

	// Dat 파일 정보로 원형 생성
	for (auto& path : *listDatpath)
	{
		// 파일을 불러와서 타입별로 원형객체 추가
		ObjectIoManager->LoadObject_Create(STR_FILEPATH_RESOURCE_DAT_L, path->FileName);
	}

	// 모든 DESC 파일 재 로드
	GetSingle(CGameManager)->Set_ReListPath(CGameManager::PATHTYPE_DESC);

	const list<MYFILEPATH*>* listDESCpath = GetSingle(CGameManager)->Get_PathList(CGameManager::PATHTYPE_DESC);

	for (auto& path : *listDESCpath)
	{
		// 파일을 불러와서 타입별로 원형객체 추가
		ObjectIoManager->Load_DESC(DESC_DATA_TERRAIN, STR_FILEPATH_RESOURCE_DAT_L, path->FileName,path->Extension);
	}

	return S_OK;
}

CGameObject_Base* CGameObject_Creater::CreateEmptyObject(const E_TAYGAMEOBJECT type)
{
	// 깡통 오브젝트로 빈오브젝트 생성
	// 레이어 추가 X
	CGameObject_Base* EmptyObject = (CGameObject_Base*)GetSingle(CGameInstance)->Create_GameObject(TAGOBJ(type));
	return EmptyObject;
}

HRESULT CGameObject_Creater::PushObject(CGameObject_Base** obj, _uint levelInex, wstring layertag)
{
	FAILED_CHECK(GetSingle(CGameInstance)->Push_Object(levelInex, layertag.c_str(), *obj));

	return S_OK;
}

CGameObject_Base * CGameObject_Creater::CreateAndPush(const E_TAYGAMEOBJECT type, _uint levelInex, wstring layertag)
{
	CGameObject_Base* EmptyObject = (CGameObject_Base*)GetSingle(CGameInstance)->Create_GameObject(TAGOBJ(type));
	NULL_CHECK_BREAK(EmptyObject);
	GetSingle(CGameInstance)->Push_Object(levelInex, layertag.c_str(), EmptyObject);
	return EmptyObject;
}

//HRESULT CGameObject_Creater::Create_ObjectProto_Type(const E_OBJECT_TYPE type, const char * data, wstring protoname)
//{
//	_uint offset = 0;
//	CGameInstance* GameInstance = GetSingle(CGameInstance);
//
//	CGameObject_Base* obj = nullptr;
//
//	switch (type)
//	{
//	case OBJECT_TYPE_2D:
//	{
//		offset = sizeof(UI_DESC);
//
//		UI_DESC UIDesc;
//		memcpy(&UIDesc, data, sizeof(UI_DESC));
//		TEXTURE_UI_DESC TexDesc;
//		memcpy(&TexDesc, data + offset, sizeof(TEXTURE_UI_DESC));
//
//		// 깡통오브젝트에 DESC정보를 클론 맵에 저장한다.
//		obj = CreateEmptyObject(GAMEOBJECT_2D);
//		CGameObject_2D* obj2D = static_cast<CGameObject_2D*>(obj);
//		obj2D->Set_LoadUIDesc(UIDesc);
//		obj2D->Set_LoadTexDesc(TexDesc);
//	}
//	break;
//
//	case OBJECT_TYPE_3D_STATIC:
//	{
//		MODEL_STATIC_DESC ModelDESC;
//		memcpy(&ModelDESC, data, sizeof(MODEL_STATIC_DESC));
//
//		// 깡통오브젝트에 DESC정보를 클론 맵에 저장한다.
//		obj = CreateEmptyObject(GAMEOBJECT_3D_STATIC);
//		CGameObject_3D_Static* obj3D = static_cast<CGameObject_3D_Static*>(obj);
//		obj3D->Set_LoadModelDESC(ModelDESC);
//	}
//	break;
//
//	case OBJECT_TYPE_3D_DYNAMIC:
//	{
//		MODEL_DYNAMIC_DESC ModelDESC;
//		memcpy(&ModelDESC, data, sizeof(MODEL_DYNAMIC_DESC));
//
//		// 깡통오브젝트에 DESC정보를 클론 맵에 저장한다.
//		obj = CreateEmptyObject(GAMEOBJECT_3D_STATIC);
//		CGameObject_3D_Dynamic* obj3D = static_cast<CGameObject_3D_Dynamic*>(obj);
//		obj3D->Set_LoadModelDynamicDESC(ModelDESC);
//	}
//	break;
//	case OBJECT_TYPE_TERRAIN:
//	{
//		//MODEL_STATIC_DESC ModelDESC;
//		//memcpy(&ModelDESC, data, sizeof(MODEL_STATIC_DESC));
//
//		// 깡통오브젝트에 DESC정보를 클론 맵에 저장한다.
//		obj = CreateEmptyObject(GAMEOBJECT_TERRAIN);
//		CGameObject_MyTerrain* obj3D = static_cast<CGameObject_MyTerrain*>(obj);
//	}
//	break;
//
//	case OBJECT_TYPE_END:
//		return E_FAIL;
//		break;
//	default:
//		return E_FAIL;
//		break;
//	}
//
//	mMap_GameObject2File_Proto.emplace(protoname, obj);
//	return S_OK;
//}

CGameObject_Base* CGameObject_Creater::Create_ObjectClone_Prefab(_uint levelindex, wstring cloneName, wstring layertag)
{
	// 복사된 오므젝트 레이어에 추가
	CGameObject_Base* findobject = Find_MapObject(cloneName);
	if (findobject)
	{
		CGameObject_Base* newObj = findobject->Clone(nullptr);
		GetSingle(CGameInstance)->Push_Object(levelindex, layertag.c_str(), newObj);
		return newObj;
	}
	return nullptr;
}

CGameObject_Base * CGameObject_Creater::Create_ObjectClone_Prefab_NoPush(wstring cloneName)
{
	CGameObject_Base* findobject = Find_MapObject(cloneName);
	return findobject;
}

HRESULT CGameObject_Creater::Add_MapObject(wstring keyname, CGameObject_Base* obj)
{
	mMap_GameObject2File_Proto.emplace(keyname, obj);
	return S_OK;
}

CGameObject_Base * CGameObject_Creater::Find_MapObject(wstring key)
{
	auto iter = mMap_GameObject2File_Proto.find(key);
	if (iter != mMap_GameObject2File_Proto.end())
		return iter->second;
	return nullptr;
}

CGameObject_Creater * CGameObject_Creater::Create(ID3D11Device * d, ID3D11DeviceContext * c)
{
	CGameObject_Creater*	pInstance = NEW CGameObject_Creater();

	if (FAILED(pInstance->NativeConstruct(d, c)))
	{
		MSGBOX("Failed to Creating CGameObject_Creater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Creater::Free()
{
	//	Safe_Release(mObjectIoManager);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	if (mMap_GameObject2File_Proto.empty() == false)
	{
		for (auto& obj : mMap_GameObject2File_Proto)
		{
			Safe_Release(obj.second);
		}
		mMap_GameObject2File_Proto.clear();
	}
}