#include "stdafx.h"
#include "FIleIO/ObjectIO.h"
#include "GameObject/GameObject_2D.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_3D_Static.h"
#include "GameObject/GameObject_3D_Static2.h"
#include "GameObject/GameObject_MyTerrain.h"

HRESULT CObjectIO::NativeConstruct()
{
	return S_OK;
}

HRESULT CObjectIO::SaverObject(E_OBJECT_TYPE type, wstring FolderPath, wstring filename, CGameObject_Base* obj)
{
	// #SAVE 오브젝트 저장

	_ulong			dwByte = 0;
	wstring datPath = FolderPath + L"\\" + filename + L".dat";

	HANDLE			hFile = CreateFile(datPath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	WriteFile(hFile, &(obj->Get_ObjectTypeID()), sizeof(E_OBJECT_TYPE), &dwByte, nullptr);

	// 1. 파일을 연다
	// 2. 오브젝트 별로 저장할 데이터 분기

	// 타입 저장

	// 오브젝트 저장시 각 데이터도 따로 저장한다. Save_DESC();

	switch (type)
	{
	case OBJECT_TYPE_2D:
	{
		CGameObject_2D* oobj = static_cast<CGameObject_2D*>(obj);
		WriteFile(hFile, &(oobj->Get_UIDesc()), sizeof(UI_DESC), &dwByte, nullptr);
		WriteFile(hFile, &(oobj->Get_TextureDesc()), sizeof(TEXTURE_NAMES_DESC), &dwByte, nullptr);

		if (dwByte == 0)
			return S_OK;	
	}
	break;
	case OBJECT_TYPE_3D_STATIC:
	{
		CGameObject_3D_Static* oobj = static_cast<CGameObject_3D_Static*>(obj);
		WriteFile(hFile, &(oobj->Get_ModelDESC()), sizeof(MODEL_STATIC_DESC), &dwByte, nullptr);
		WriteFile(hFile, &(oobj->Get_ColliderDESC()), sizeof(COLLIDER_DESC), &dwByte, nullptr);

		// desc
		Save_DESC(DESC_DATA_STRNAME, FolderPath, filename, (void*) &(oobj->Get_ModelDESC()));
		Save_DESC(DESC_DATA_COLLIDER, FolderPath, filename, (void*)&(oobj->Get_ColliderDESC()));

	}
	break;
	case OBJECT_TYPE_3D_DYNAMIC:
	{
		CGameObject_3D_Dynamic* oobj = static_cast<CGameObject_3D_Dynamic*>(obj);
		WriteFile(hFile, &(oobj->Get_ModelDESC()), sizeof(MODEL_DYNAMIC_DESC), &dwByte, nullptr);

		// desc
		Save_DESC(DESC_DATA_STRNAME, FolderPath, filename, (void*)&(oobj->Get_ModelDESC()));

	}
	break;

	case OBJECT_TYPE_3D_STATIC_PARENT:
	{
		CGameObject_3D_Static2* oobj = static_cast<CGameObject_3D_Static2*>(obj);

		auto childList = oobj->Get_ChildList();

		for (auto& childObj: *childList)
		{
			WriteFile(hFile, &(childObj->Get_ModelDESC()), sizeof(MODEL_STATIC_DESC), &dwByte, nullptr);
			WriteFile(hFile, &(childObj->Get_ColliderDESC()), sizeof(COLLIDER_DESC), &dwByte, nullptr);
		}		
	}
	break;

	case OBJECT_TYPE_TERRAIN:
	{
		CGameObject_MyTerrain* oobj = static_cast<CGameObject_MyTerrain*>(obj);
		/*TERRAIN_DESC desc = (oobj->Get_TerrainDESC());
		WriteFile(hFile, &desc.meTerrainSize, sizeof(E_TERRAINSIZE), &dwByte, nullptr);
		WriteFile(hFile, &desc.mTextureMultiSize, sizeof(_uint), &dwByte, nullptr);

		WriteFile(hFile, &desc.mNoTileSize, sizeof(_uint), &dwByte, nullptr);
		for (int i = 0; i < desc.mNoTileSize; ++i)
		{
			_uint NoIndex = desc.mArrayIndes[i];
			WriteFile(hFile, &NoIndex, sizeof(_uint), &dwByte, nullptr);
		}

		WriteFile(hFile, &desc.mObjectSize, sizeof(_uint), &dwByte, nullptr);
		for (int i = 0; i < desc.mObjectSize; ++i)
		{
			MODEL_WORLD_DESC dummy = desc.mArrayModelObjects[i];
			WriteFile(hFile, &dummy, sizeof(MODEL_WORLD_DESC), &dwByte, nullptr);
		}*/
		Save_DESC(DESC_DATA_TERRAIN, FolderPath, filename, (void*)&(oobj->Get_TerrainDESC()));
	}

	break;
	case OBJECT_TYPE_END:

		break;
	default:
		break;
	}

	// 3. 파일 닫기
	CloseHandle(hFile);

	return S_OK;
}

// 1. 객체를 저장할 떄 같이 저장
// 2. 따로 저장/수정 가능하게 IMGUI 판다.
HRESULT CObjectIO::Save_DESC(E_DESC_DATA descid, wstring FolderPath, wstring filename, void* voidDesc)
{
	// #SAVE DESC 정보 저장
	// 확장자로 나누기 // ㅇ 
	_ulong			dwByte = 0;

	wstring ExeName = DESCEXE(descid);
	wstring DescPath = FolderPath + L"\\DESC\\" + filename + ExeName;
	HANDLE			hFile = CreateFile(DescPath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	switch (descid)
	{
	case DESC_DATA_WORLD:
		break;
	case DESC_DATA_TEXTURENAME:
		break;
	case DESC_DATA_STRNAME:
		break;
	case DESC_DATA_COLLIDER:
		break;
	case DESC_DATA_TERRAIN:
	{
		TERRAIN_DESC desc = (*(TERRAIN_DESC*)(voidDesc));
		WriteFile(hFile, &desc.meTerrainSize, sizeof(E_TERRAINSIZE), &dwByte, nullptr);
		WriteFile(hFile, &desc.mTextureMultiSize, sizeof(_uint), &dwByte, nullptr);

		WriteFile(hFile, &desc.mNoTileSize, sizeof(_uint), &dwByte, nullptr);
		for (int i = 0; i < desc.mNoTileSize; ++i)
		{
			WriteFile(hFile, &desc.mArrayIndes[i], sizeof(_uint), &dwByte, nullptr);
		}

		WriteFile(hFile, &desc.mObjectSize, sizeof(_uint), &dwByte, nullptr);
		for (int i = 0; i < desc.mObjectSize; ++i)
		{
			WriteFile(hFile, &desc.mArrayModelObjects[i], sizeof(MODEL_WORLD_DESC), &dwByte, nullptr);
		}
	}
		break;
	case DESC_DATA_END:
		break;
	default:
		break;
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CObjectIO::Load_DESC(E_DESC_DATA descid, wstring FolderPath, wstring filename, wstring Extension)
{
	// #LOADDESC 정보 로드 테스트 필요
	CGameObject_Creater* creater = GetSingle(CGameManager)->Get_CreaterManager();

	_ulong			dwByte = 0;

	wstring ExeName = Get_EXEDescName(descid);

	if ((Extension != ExeName))
		return S_FALSE;

	wstring DescPath = FolderPath + L"\\DESC\\" + filename;

	HANDLE			hFile = CreateFile(DescPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	// 정보 별로 읽어서 DESC MAP에 저장해둔다.

	switch (descid)
	{
	case DESC_DATA_TERRAIN:
		TERRAIN_DESC* desc = NEW TERRAIN_DESC;

		ReadFile(hFile, &desc->meTerrainSize, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &desc->mTextureMultiSize, sizeof(_uint), &dwByte, nullptr);

		ReadFile(hFile, &desc->mNoTileSize, sizeof(_uint), &dwByte, nullptr);
		if (desc->mNoTileSize != 0)
		{
			desc->mArrayIndes = NEW _uint[desc->mNoTileSize];
			for (int i = 0; i < desc->mNoTileSize; ++i)
			{
				ReadFile(hFile, &desc->mArrayIndes[i], sizeof(_uint), &dwByte, nullptr);
			}
		}

		ReadFile(hFile, &desc->mObjectSize, sizeof(_uint), &dwByte, nullptr);
		if (desc->mObjectSize != 0)
		{
			desc->mArrayModelObjects = NEW MODEL_WORLD_DESC[desc->mObjectSize];
			for (int i = 0; i < desc->mObjectSize; ++i)
			{
				ReadFile(hFile, &desc->mArrayModelObjects[i], sizeof(MODEL_WORLD_DESC), &dwByte, nullptr);
			}
		}
		


		// Desc 전역 저장
		Add_TerrainDesc(filename, desc);
		break;
	}


	if (dwByte == 0)
	{
		CloseHandle(hFile);
		return E_FAIL;
	}


	CloseHandle(hFile);

	return S_OK;
}

wstring CObjectIO::Get_EXEDescName(E_DESC_DATA descid)
{

	wstring exestr = STR_DATADESC_EXE(descid);
	if (exestr.empty())
		return exestr;
	exestr = exestr.substr(1, exestr.size());
	return exestr;

}

HRESULT CObjectIO::Add_TerrainDesc(wstring key, TERRAIN_DESC * value)
{

	if (Find_TerrainDesc(key) != nullptr)
		return E_FAIL;

	mMap_TerrainDesc.emplace(key, value);
	return S_OK;
}

TERRAIN_DESC * CObjectIO::Find_TerrainDesc(wstring key)
{
	auto iter = mMap_TerrainDesc.find(key);

	if (iter == mMap_TerrainDesc.end())
		return nullptr; 

	return iter->second;
}


HRESULT CObjectIO::LoadObject_Create(wstring FolderPath, wstring filename)
{
	// 로드와 생성을 같이 한다.
	wstring path = FolderPath + L"\\" + filename;

	HANDLE			hFile = CreateFile(path.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	if (false == Create_CreateMap_ProtoType(hFile, filename))
		return E_FAIL;

	CloseHandle(hFile);

	return S_OK;
}

bool CObjectIO::Create_CreateMap_ProtoType(HANDLE& hFile, wstring keyname)
{
	// #LOADOBJECT 저장된 객체 프로토타입으로 저장.

	CGameManager* GameInstance = GetSingle(CGameManager);
	CGameObject_Creater* creater =  GameInstance->Get_CreaterManager();

	CGameObject_Base* emptyObject = nullptr;

	E_OBJECT_TYPE	TypeDESC = OBJECT_TYPE_NONE;
	_ulong			dwByte = 0;

	ReadFile(hFile, &TypeDESC, sizeof(E_OBJECT_TYPE), &dwByte, nullptr);

	if (TypeDESC == OBJECT_TYPE_NONE)
		return S_FALSE;

	switch (TypeDESC)
	{
	case OBJECT_TYPE_2D:
	{
		UI_DESC uiDesc;
		TEXTURE_NAMES_DESC texDesc;

		ReadFile(hFile, &uiDesc, sizeof(UI_DESC), &dwByte, nullptr);
		ReadFile(hFile, &texDesc, sizeof(TEXTURE_NAMES_DESC), &dwByte, nullptr);

		if (dwByte == 0)
			return false;

		emptyObject = creater->CreateEmptyObject(GAMEOBJECT_2D);
		CGameObject_2D* oobj = static_cast<CGameObject_2D*>(emptyObject);
		oobj->Set_LoadUIDesc(uiDesc);
		oobj->Set_LoadTexDesc(texDesc);
	}
		break;
	case OBJECT_TYPE_3D_STATIC:
	{
		MODEL_STATIC_DESC modelDesc;
		COLLIDER_DESC colDesc;

		ReadFile(hFile, &modelDesc, sizeof(MODEL_STATIC_DESC), &dwByte, nullptr);
		ReadFile(hFile, &colDesc, sizeof(COLLIDER_DESC), &dwByte, nullptr);

		if (dwByte == 0)
			return false;

		emptyObject = creater->CreateEmptyObject(GAMEOBJECT_3D_STATIC);
		CGameObject_3D_Static* oobj = static_cast<CGameObject_3D_Static*>(emptyObject);

		oobj->Set_LoadModelDESC(modelDesc);
		oobj->Set_LoadColliderDESC(colDesc);
	}
		break;
	case OBJECT_TYPE_3D_STATIC_PARENT:
	{
		while (true)
		{
			MODEL_STATIC_DESC modelDesc;
			COLLIDER_DESC colDesc;


			ReadFile(hFile, &modelDesc, sizeof(MODEL_STATIC_DESC), &dwByte, nullptr);
			ReadFile(hFile, &colDesc, sizeof(COLLIDER_DESC), &dwByte, nullptr);
			if (dwByte == 0)
				break;
			if(emptyObject == nullptr)
				emptyObject = creater->CreateEmptyObject(GAMEOBJECT_3D_STATIC2);

			CGameObject_3D_Static* newchildObj = static_cast<CGameObject_3D_Static*>(creater->CreateEmptyObject(GAMEOBJECT_3D_STATIC));
			newchildObj->Set_LoadModelDESC(modelDesc);
			newchildObj->Set_LoadColliderDESC(colDesc);

			CGameObject_3D_Static2* oobj = static_cast<CGameObject_3D_Static2*>(emptyObject);

			oobj->Add_StaticObejct(newchildObj);
		}		
	}
		break;
	case OBJECT_TYPE_3D_DYNAMIC:
	{
		//UI_DESC uiDesc;
		//TEXTURE_UI_DESC texDesc;

		//if (dwByte == 0)
		//	return false;

		//ReadFile(hFile, &uiDesc, sizeof(UI_DESC), &dwByte, nullptr);
		//ReadFile(hFile, &texDesc, sizeof(TEXTURE_UI_DESC), &dwByte, nullptr);

		//if (dwByte == 0)
		//	return true; // Navi데이터 예외

		//emptyObject = creater->CreateEmptyObject(GAMEOBJECT_2D);
		//CGameObject_2D* obj2D = static_cast<CGameObject_2D*>(emptyObject);
		//obj2D->Set_LoadUIDesc(uiDesc);
		//obj2D->Set_LoadTexDesc(texDesc);
	}
		break;
	case OBJECT_TYPE_TERRAIN:
	{
		//TERRAIN_DESC desc;	

		//ReadFile(hFile, &desc.meTerrainSize, sizeof(E_TERRAINSIZE), &dwByte, nullptr);
		//ReadFile(hFile, &desc.mTextureMultiSize, sizeof(_uint), &dwByte, nullptr);

		//ReadFile(hFile, &desc.mNoTileSize, sizeof(_uint), &dwByte, nullptr);
		//if (desc.mNoTileSize != 0)
		//{
		//	desc.mArrayIndes = NEW _uint[desc.mNoTileSize];
		//	for (int i = 0; i < desc.mNoTileSize; ++i)
		//	{
		//		int index = -1;
		//		ReadFile(hFile, &index, sizeof(_uint), &dwByte, nullptr);
		//		if (index != -1)
		//			desc.mArrayIndes[i] = index;
		//	}
		//}

		//ReadFile(hFile, &desc.mObjectSize, sizeof(_uint), &dwByte, nullptr);
		//if (desc.mObjectSize != 0)
		//{
		//	desc.mArrayModelObjects = NEW MODEL_WORLD_DESC[desc.mObjectSize];

		//	for (int i = 0; i < desc.mObjectSize; ++i)
		//	{
		//		MODEL_WORLD_DESC worldDesc;
		//		ReadFile(hFile, &worldDesc, sizeof(MODEL_WORLD_DESC), &dwByte, nullptr);
		//		if (dwByte == 0)
		//			return false;

		//	}
		//}

		//emptyObject = creater->CreateEmptyObject(GAMEOBJECT_MYTERRAIN);
		//CGameObject_MyTerrain* terrainobj = static_cast<CGameObject_MyTerrain*>(emptyObject);
		//terrainobj->Set_LoadTerrainDESC(desc);
	}

	case OBJECT_TYPE_END:
		break;
	default:
		break;
	}

	if (emptyObject != nullptr)
	{
		creater->Add_MapObject(keyname, emptyObject);
		return true;
	}
	else
		return false;
}

CObjectIO * CObjectIO::Create()
{
	CObjectIO*	pInstance = NEW CObjectIO();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CObjectIO");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CObjectIO::Free()
{
	for (auto& desc : mMap_TerrainDesc)
	{
		Safe_Delete(desc.second);
	}
	mMap_TerrainDesc.clear();
}