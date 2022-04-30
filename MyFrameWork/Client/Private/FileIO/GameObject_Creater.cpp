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
	// LoaderDatFile_For_PrototypeObject();

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

	// �Ź� �ʱ�ȭ
	if (mMap_GameObject2File_Proto.empty() == false)
	{
		for (auto& obj : mMap_GameObject2File_Proto)
		{
			Safe_Release(obj.second);
		}
		mMap_GameObject2File_Proto.clear();
	}

	// ��� dat ���� �� �ε�
	GetSingle(CGameManager)->Set_ReListPath(CGameManager::PATHTYPE_DATA);

	const list<MYFILEPATH*>* listDatpath = GetSingle(CGameManager)->Get_PathList(CGameManager::PATHTYPE_DATA);

	// Dat ���� ������ ���� ����
	for (auto& path :*listDatpath)
	{
		E_OBJECT_TYPE type = OBJECT_TYPE_END;
		char* buf = nullptr;

		// buf���� �����Ͱ� �Ѿ�´�. / type������ ������Ʈ ��������
		ObjectIoManager->LoadObject(STR_FILEPATH_RESOURCE_DAT_L, path->FileName, &buf,&type);

		// �����߰�
		FAILED_CHECK(Create_ObjectProto_Type(type, buf, path->FileName));
		Safe_Delete_Array(buf);
	}

	return S_OK;
}

CGameObject_Base* CGameObject_Creater::CreateEmptyObject(const E_TAYGAMEOBJECT type)
{
	// ���� ������Ʈ�� �������Ʈ ����
	// ���̾� �߰� X
	CGameObject_Base* EmptyObject = (CGameObject_Base*)GetSingle(CGameInstance)->Create_GameObject(TAGOBJ(type));
	return EmptyObject;
}

HRESULT CGameObject_Creater::Create_ObjectProto_Type(const E_OBJECT_TYPE type, const char * data, wstring protoname)
{
	_uint offset = 0;
	CGameInstance* GameInstance = GetSingle(CGameInstance);

	CGameObject_Base* obj = nullptr;

	switch (type)
	{
	case OBJECT_TYPE_2D:
	{
		offset = sizeof(UI_DESC);

		UI_DESC UIDesc;
		memcpy(&UIDesc, data, sizeof(UI_DESC));
		TEXTURE_DESC TexDesc;
		memcpy(&TexDesc, data + offset, sizeof(TEXTURE_DESC));

		// ���������Ʈ�� DESC������ Ŭ�� �ʿ� �����Ѵ�.
		obj = CreateEmptyObject(GAMEOBJECT_2D);
		CGameObject_2D* obj2D = static_cast<CGameObject_2D*>(obj);
		obj2D->Set_LoadUIDesc(UIDesc);
		obj2D->Set_LoadTexDesc(TexDesc);
	}
	break;

	case OBJECT_TYPE_3D_STATIC:
	{
		MODEL_STATIC_DESC ModelDESC;
		memcpy(&ModelDESC, data, sizeof(MODEL_STATIC_DESC));

		// ���������Ʈ�� DESC������ Ŭ�� �ʿ� �����Ѵ�.
		obj = CreateEmptyObject(GAMEOBJECT_3D_STATIC);
		CGameObject_3D_Static* obj3D = static_cast<CGameObject_3D_Static*>(obj);
		obj3D->Set_LoadModelDESC(ModelDESC);
	}
		break;

	case OBJECT_TYPE_3D_DYNAMIC:
	{
		MODEL_DYNAMIC_DESC ModelDESC;
		memcpy(&ModelDESC, data, sizeof(MODEL_DYNAMIC_DESC));

		// ���������Ʈ�� DESC������ Ŭ�� �ʿ� �����Ѵ�.
		obj = CreateEmptyObject(GAMEOBJECT_3D_STATIC);
		CGameObject_3D_Dynamic* obj3D = static_cast<CGameObject_3D_Dynamic*>(obj);
		obj3D->Set_LoadModelDynamicDESC(ModelDESC);
	}
	break;
	case OBJECT_TYPE_TERRAIN:
	{
		//MODEL_STATIC_DESC ModelDESC;
		//memcpy(&ModelDESC, data, sizeof(MODEL_STATIC_DESC));

		// ���������Ʈ�� DESC������ Ŭ�� �ʿ� �����Ѵ�.
		obj = CreateEmptyObject(GAMEOBJECT_TERRAIN);
		CGameObject_MyTerrain* obj3D = static_cast<CGameObject_MyTerrain*>(obj);
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

CGameObject_Base* CGameObject_Creater::Create_ObjectClone_Prefab(_uint levelindex, wstring cloneName, wstring layertag)
{
	// ����� ������Ʈ ���̾ �߰�
	CGameObject_Base* findobject = Find_MapObject(cloneName);
	if (findobject)
	{
		CGameObject_Base* newObj = findobject->Clone(nullptr);
		GetSingle(CGameInstance)->Push_Object(levelindex, layertag.c_str(), newObj);
		return newObj;

	}
	return nullptr;
}

CGameObject_Base * CGameObject_Creater::Find_MapObject(wstring key)
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
