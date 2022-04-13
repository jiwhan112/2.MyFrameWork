#pragma once


BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

// 클라이언트의 DESC 정보 저장 불러오기 클래스

// 바이너리로 DESC파일 저장 및 불러오기 수행

class CObjectIO
{

private:
	CObjectIO() = default;
	~CObjectIO() = default;


public:
	// 오브젝트 타입별로 저장 수행
	static void SaverObject(E_OBJECT_TYPE type, wstring FolderPath, wstring filename, CGameObject* obj);

	// 연 파일을 넘겨받고 데이터 저장
	static void SaverData(ofstream* fwrite, E_OBJECT_DATA_TYPE type, const void * desc);

	// 데이터를 불러와서 생성기에 넘김
	static void  LoadObject(wstring FolderPath, wstring filename, CGameObject* obj);
//	static void* LoadData(ifstream* fRead, E_OBJECT_DATA_TYPE type, string FolderPath, string filename);
//	static void* LoadData(ifstream* fRead, E_OBJECT_DATA_TYPE type, string FolderPath, string filename);


private:
	// 각 데이터별로 저장
//	static void SaveData(ofstream * fwrite, const void* desc, _uint size);
	static void SaveOBJECT(ofstream* fwrite, E_OBJECT_TYPE* desc);
	static void SaveUIDESC(ofstream* fwrite, UIDESC* desc);
	static void SaveTEXTUREDESC(ofstream* fwrite, TEXTUREDESC* desc);



	// 각 데이터별로 로드


};


END