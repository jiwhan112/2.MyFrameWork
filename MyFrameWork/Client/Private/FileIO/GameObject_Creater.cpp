#include "stdafx.h"
#include "FIleIO/GameObject_Creater.h"
#include "FIleIO/ObjectIO.h"
#include "GameObject/GameObject_2D.h"


CGameObject_Creater::CGameObject_Creater()
{
	mObjectIoManager = nullptr;
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
	// LoaderDatFile_For_PrototypeObject();

	return S_OK;
}

HRESULT CGameObject_Creater::LoaderDatFile_For_PrototypeObject()
{
	if (mObjectIoManager == nullptr)
	{
		mObjectIoManager = GetSingle(CGameManager)->Get_ObjectIOManager();
		Safe_AddRef(mObjectIoManager);
	}

	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
		return E_FAIL;

	if (mMap_GameObject2File_Proto.empty() == false)
	{
		for (auto& obj : mMap_GameObject2File_Proto)
		{
			Safe_Release(obj.second);
		}
		mMap_GameObject2File_Proto.clear();
	}

	// 모든 dat 파일 로드
	GetSingle(CGameManager)->Set_ReListPath(CGameManager::PATHTYPE_DATA);

	const list<MYFILEPATH*>* listDatpath = GetSingle(CGameManager)->Get_PathList(CGameManager::PATHTYPE_DATA);

	// Dat 파일 정보로 원형 생성
	for (auto& path :*listDatpath)
	{
		E_OBJECT_TYPE type = OBJECT_TYPE_END;
		char* buf = nullptr;

		// buf에서 데이터가 넘어온다. / type정보로 오브젝트 원형제작
		mObjectIoManager->LoadObject(STR_FILEPATH_RESOURCE_DAT_L, path->FileName, &buf,&type);

		// 원형추가
		FAILED_CHECK(Create_ObjectProto_Type(type, buf, path->FileName));
		Safe_Delete_Array(buf);
	}

	return S_OK;
}

CGameObject* CGameObject_Creater::CreateEmptyObject(const E_TAYGAMEOBJECT type)
{
	// 깡통 오브젝트로 빈오브젝트 생성
	// 레이어 추가 X
	CGameObject* EmptyObject = GetSingle(CGameInstance)->Create_GameObject(TAGOBJ(type));
	return EmptyObject;
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
		obj = CreateEmptyObject(GAMEOBJECT_2D);
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

	mMap_GameObject2File_Proto.emplace(protoname, obj);
	
	return S_OK;
}

CGameObject* CGameObject_Creater::Create_ObjectClone_Prefab(_uint levelindex, wstring cloneName, wstring layertag)
{
	// 복사된 오므젝트 레이어에 추가
	CGameObject* findobject = Find_MapObject(cloneName);
	if (findobject)
	{
		CGameObject* newObj = findobject->Clone(nullptr);
		GetSingle(CGameInstance)->Push_Object(levelindex, layertag.c_str(), newObj);
		return newObj;

	}
	return nullptr;
}

CGameObject * CGameObject_Creater::Find_MapObject(wstring key)
{
	auto iter =  mMap_GameObject2File_Proto.find(key);
	if (iter != mMap_GameObject2File_Proto.end())
		return iter->second;
	return nullptr;
}

CGameObject_Creater * CGameObject_Creater::Create(ID3D11Device * d, ID3D11DeviceContext * c)
{
	CGameObject_Creater*	pInstance = NEW CGameObject_Creater();

	if (FAILED(pInstance->NativeConstruct(d,c)))
	{
		MSGBOX("Failed to Creating CGameObject_Creater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Creater::Free()
{

	Safe_Release(mObjectIoManager);
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
