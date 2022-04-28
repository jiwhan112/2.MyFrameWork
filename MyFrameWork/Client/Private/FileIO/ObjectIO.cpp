#include "stdafx.h"
#include "FIleIO/ObjectIO.h"
#include "GameObject/GameObject_2D.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_3D_Static.h"
#include "GameObject/GameObject_MyTerrain.h"

HRESULT CObjectIO::NativeConstruct()
{
	ZeroMemory(&mObjectType,sizeof(E_OBJECT_TYPE));
	ZeroMemory(&mUIDesc, sizeof(UIDESC));
	ZeroMemory(&mTexDesc,sizeof(TEXTUREDESC));

	return S_OK;
}

HRESULT CObjectIO::SaverObject(E_OBJECT_TYPE type, wstring FolderPath, wstring filename, CGameObject_Base* obj)
{
	// #INIT ObjectSave 추가
	ofstream fWrite(FolderPath + L"\\" + filename, ios::out | ios::binary);
	if (fWrite.is_open() == false)
	{
		return E_FAIL;
	}

	// 1. 파일을 연다
	// 2. 오브젝트 별로 저장할 데이터 분기

	switch (type)
	{
	case OBJECT_TYPE_2D:
	{
		CGameObject_2D* oobj = static_cast<CGameObject_2D*>(obj);
		SaverData(&fWrite, OBJECT_TYPE_DATA_OBJECT, &(oobj->Get_ObjectTypeID()));
		SaverData(&fWrite, OBJECT_TYPE_DATA_UIDESC, &(oobj->Get_UIDesc()));
		SaverData(&fWrite, OBJECT_TYPE_DATA_TEXTUREDESC, &(oobj->Get_TextureDesc()));

	}
		break;
	case OBJECT_TYPE_3D_STATIC:
	{
		CGameObject_3D_Static* oobj = static_cast<CGameObject_3D_Static*>(obj);
		SaverData(&fWrite, OBJECT_TYPE_DATA_OBJECT, &(oobj->Get_ObjectTypeID()));
		SaverData(&fWrite, OBJECT_TYPE_DATA_MODEL_STATICDESC, &(oobj->Get_ModelDESC()));

	}
	case OBJECT_TYPE_3D_DYNAMIC:
	{
		CGameObject_3D_Dynamic* oobj = static_cast<CGameObject_3D_Dynamic*>(obj);
		//SaverData(&fWrite, OBJECT_TYPE_DATA_OBJECT, &(oobj->Get_ObjectTypeID()));
		//SaverData(&fWrite, OBJECT_TYPE_DATA_MODEL_DYNAMICDESC, &(oobj->Get_UIDesc()));

	}
	case OBJECT_TYPE_TERRAIN:
	{
		//CGameObject_MyTerrain* oobj = static_cast<CGameObject_2D*>(obj);
		//SaverData(&fWrite, OBJECT_TYPE_DATA_OBJECT, &(oobj->Get_ObjectTypeID()));
		//SaverData(&fWrite, OBJECT_TYPE_DATA_UIDESC, &(oobj->Get_UIDesc()));
		//SaverData(&fWrite, OBJECT_TYPE_DATA_TEXTUREDESC, &(oobj->Get_TextureDesc()));

	}
	case OBJECT_TYPE_END:

		break;
	default:
		break;
	}

	// 3. 파일 닫기
	fWrite.close();

	return S_OK;
}



HRESULT CObjectIO::SaverData(ofstream* fwrite,E_OBJECT_DATA_TYPE type,const void * desc)
{

	switch (type)
	{
	case OBJECT_TYPE_DATA_OBJECT:
		SaveOBJECT(fwrite, (E_OBJECT_TYPE*)desc);
		break;
	case OBJECT_TYPE_DATA_UIDESC:
		SaveUIDESC(fwrite, (UIDESC*)desc);
		break;
	case OBJECT_TYPE_DATA_TEXTUREDESC:
		SaveTEXTUREDESC(fwrite,(TEXTUREDESC*)desc);
		break;
	case OBJECT_TYPE_DATA_MODEL_STATICDESC:
		SaveMODELSTATICDESC(fwrite, (MODEL_STATIC_DESC*)desc);
		break;
	case OBJECT_TYPE_DATA_MODEL_DYNAMICDESC:
		SaveMODELDYNAMICDESC(fwrite, (MODEL_DYNAMIC_DESC*)desc);
		break;
		
	case OBJECT_TYPE_DATA_END:
		break;
	default:
		break;
	}
	return S_OK;


}

HRESULT CObjectIO::LoadObject(wstring FolderPath, wstring filename, char** pData, E_OBJECT_TYPE* type)
{ 
	// #INIT ObjectLoad 추가

	ifstream fRead(FolderPath + L"\\" + filename, ios::in | ios::binary);
	if (fRead.is_open() == false)
	{
		return E_FAIL;
	}
	_uint bufsize = 0;

	char* objType  = NEW char[sizeof(E_OBJECT_DATA_TYPE)];

	// 데이터 가장 위쪽에 현재 저장된 타입의 ID 해석.
	fRead.read(objType, sizeof(E_OBJECT_DATA_TYPE));

	E_OBJECT_TYPE	TypeDESC;
	memcpy(&TypeDESC, objType, sizeof(E_OBJECT_TYPE));
	*type = TypeDESC;

	// 타입에 따라 오브젝트 해석방식이 달라짐
	switch (*type)
	{
	case OBJECT_TYPE_2D:
	{
		// 로드 확인 완료
		bufsize = sizeof(UIDESC) + sizeof(TEXTUREDESC);
		*pData = NEW char[bufsize];
		UIDESC uiDesc;
		TEXTUREDESC texDesc;
		int offset = sizeof(UIDESC);

		fRead.read(*pData, sizeof(UIDESC));
		memcpy(&uiDesc, *pData, sizeof(UIDESC));

		fRead.read(*pData + offset, sizeof(TEXTUREDESC));
		memcpy(&texDesc, *pData+ offset, sizeof(TEXTUREDESC));
		
	}
	break;

	case OBJECT_TYPE_3D_STATIC:
	{
		bufsize = sizeof(MODEL_STATIC_DESC);
		*pData = NEW char[bufsize];
		MODEL_STATIC_DESC ModelDesc;


		fRead.read(*pData, sizeof(MODEL_STATIC_DESC));
		memcpy(&ModelDesc, *pData, sizeof(MODEL_STATIC_DESC));
		int a = 5;

	}
	break;
	case OBJECT_TYPE_3D_DYNAMIC:
	{

		bufsize = sizeof(MODEL_DYNAMIC_DESC);
		*pData = NEW char[bufsize];
		MODEL_DYNAMIC_DESC ModelDesc;


		fRead.read(*pData, sizeof(MODEL_DYNAMIC_DESC));
		memcpy(&ModelDesc, *pData, sizeof(MODEL_DYNAMIC_DESC));

	}
	break;
	case OBJECT_TYPE_END:

		break;
	default:
		break;
	}
	Safe_Delete_Array(objType);
//	Safe_Delete_Array(DataLoad);

	return S_OK;

}




HRESULT CObjectIO::SaveOBJECT(ofstream * fwrite, E_OBJECT_TYPE* desc)
{
	char * newdesc = NEW char[sizeof(E_OBJECT_TYPE)];

	memcpy(newdesc, desc, sizeof(E_OBJECT_TYPE));

	fwrite->write((char*)desc, sizeof(E_OBJECT_TYPE));
	Safe_Delete_Array(newdesc);

	return S_OK;


}

HRESULT CObjectIO::SaveUIDESC(ofstream * fwrite, UIDESC* desc)
{
	char * newdesc = NEW char[sizeof(UIDESC)];

	memcpy(newdesc,desc,sizeof(UIDESC));

	fwrite->write(newdesc, sizeof(UIDESC));

	Safe_Delete_Array(newdesc);


	return S_OK;

}

HRESULT CObjectIO::SaveTEXTUREDESC(ofstream * fwrite, TEXTUREDESC* desc)
{
	char * newdesc = NEW char[sizeof(TEXTUREDESC)];

	memcpy(newdesc, desc, sizeof(TEXTUREDESC));

	fwrite->write(newdesc, sizeof(TEXTUREDESC));
	Safe_Delete_Array(newdesc);

	return S_OK;


}
HRESULT CObjectIO::SaveMODELSTATICDESC(ofstream * fwrite, MODEL_STATIC_DESC* desc)
{
	char * newdesc = NEW char[sizeof(MODEL_STATIC_DESC)];

	memcpy(newdesc, desc, sizeof(MODEL_STATIC_DESC));

	fwrite->write(newdesc, sizeof(MODEL_STATIC_DESC));
	Safe_Delete_Array(newdesc);

	return S_OK;


}
HRESULT CObjectIO::SaveMODELDYNAMICDESC(ofstream * fwrite, MODEL_DYNAMIC_DESC* desc)
{
	char * newdesc = NEW char[sizeof(MODEL_DYNAMIC_DESC)];

	memcpy(newdesc, desc, sizeof(MODEL_DYNAMIC_DESC));

	fwrite->write(newdesc, sizeof(MODEL_DYNAMIC_DESC));
	Safe_Delete_Array(newdesc);

	return S_OK;


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


