#include "stdafx.h"
#include "FIleIO/ObjectIO.h"
#include "GameObject/GameObject_2D.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_3D_Static.h"
#include "GameObject/GameObject_3D_Static2.h"
#include "GameObject/GameObject_MyTerrain.h"

HRESULT CObjectIO::NativeConstruct()
{
	ZeroMemory(&mObjectType, sizeof(E_OBJECT_TYPE));
	ZeroMemory(&mUIDesc, sizeof(UI_DESC));
	ZeroMemory(&mTexDesc, sizeof(TEXTURE_UI_DESC));

	return S_OK;
}

HRESULT CObjectIO::SaverObject(E_OBJECT_TYPE type, wstring FolderPath, wstring filename, CGameObject_Base* obj)
{
	// #SAVE 오브젝트 저장

	_ulong			dwByte = 0;
	wstring path = FolderPath + L"\\" + filename;

	HANDLE			hFile = CreateFile(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	WriteFile(hFile, &(obj->Get_ObjectTypeID()), sizeof(E_OBJECT_TYPE), &dwByte, nullptr);

	// 1. 파일을 연다
	// 2. 오브젝트 별로 저장할 데이터 분기

	// 타입 저장

	switch (type)
	{
	case OBJECT_TYPE_2D:
	{
		CGameObject_2D* oobj = static_cast<CGameObject_2D*>(obj);
		WriteFile(hFile, &(oobj->Get_UIDesc()), sizeof(UI_DESC), &dwByte, nullptr);
		WriteFile(hFile, &(oobj->Get_TextureDesc()), sizeof(TEXTURE_UI_DESC), &dwByte, nullptr);

		if (dwByte == 0)
			return S_OK;	
	}
	break;
	case OBJECT_TYPE_3D_STATIC:
	{
		CGameObject_3D_Static* oobj = static_cast<CGameObject_3D_Static*>(obj);
		WriteFile(hFile, &(oobj->Get_ModelDESC()), sizeof(MODEL_STATIC_DESC), &dwByte, nullptr);
		WriteFile(hFile, &(oobj->Get_ColliderDESC()), sizeof(COLLIDER_DESC), &dwByte, nullptr);

	}
	break;
	case OBJECT_TYPE_3D_DYNAMIC:
	{
		CGameObject_3D_Dynamic* oobj = static_cast<CGameObject_3D_Dynamic*>(obj);
		WriteFile(hFile, &(oobj->Get_ModelDESC()), sizeof(MODEL_DYNAMIC_DESC), &dwByte, nullptr);

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
		//CGameObject_MyTerrain* oobj = static_cast<CGameObject_2D*>(obj);
		//SaverData(&fWrite, OBJECT_TYPE_DATA_OBJECT, &(oobj->Get_ObjectTypeID()));
		//SaverData(&fWrite, OBJECT_TYPE_DATA_UIDESC, &(oobj->Get_UIDesc()));
		//SaverData(&fWrite, OBJECT_TYPE_DATA_TEXTUREDESC, &(oobj->Get_TextureDesc()));
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
}

bool CObjectIO::Create_CreateMap_ProtoType(HANDLE& hFile, wstring keyname)
{
	// #LOAD 저장된 객체 프로토타입으로 저장.

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
		TEXTURE_UI_DESC texDesc;

		ReadFile(hFile, &uiDesc, sizeof(UI_DESC), &dwByte, nullptr);
		ReadFile(hFile, &texDesc, sizeof(TEXTURE_UI_DESC), &dwByte, nullptr);

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
}

// 선택한 오브젝트를 저장한다.

// 1. 클릭시 다이얼 로그 생성.
//CFileDialog		Dlg(FALSE,
//	L"dat", // .dat파일로 저장
//	L"*.dat",
//	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//	L"Data Files(*.dat)|*.dat||",
//	cwnd);

//TCHAR	szPath[MAX_PATH] = L"";
//GetCurrentDirectory(MAX_PATH, szPath);
//PathRemoveFileSpec(szPath);

//lstrcat(szPath, g_FilePath_ObjectPathData_Save.c_str());
//Dlg.m_ofn.lpstrInitialDir = szPath;

//if (IDOK == Dlg.DoModal())
//{
//	// 이름

//	CString				str = Dlg.GetPathName().GetString();
//	CString				Filename = PathFindFileName(str);

//	TCHAR				NEWName[64] = L"";

//	lstrcpy(NEWName, Filename);
//	PathRemoveExtension(NEWName);

//	const TCHAR*		pGetPath = str.GetString();

//	HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

//	if (INVALID_HANDLE_VALUE == hFile)
//		return E_FAIL;

//	DWORD	dwByte = 0;

//	// 파일 ID / 정보

//	OUTPUT_ID id;
//	id.FILEID = OUTPUT_OBJECT;
//	lstrcpy(id.strObjectName, NEWName);

//	obj->Set_OUTPUTData_Save();
//	WriteFile(hFile, &id, sizeof(OUTPUT_ID), &dwByte, nullptr);
//	WriteFile(hFile, &obj->Get_OutputData(), sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);

//	CloseHandle(hFile);