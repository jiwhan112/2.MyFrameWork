#pragma once


BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

// Ŭ���̾�Ʈ�� DESC ���� ���� �ҷ����� Ŭ����

// ���̳ʸ��� DESC���� ���� �� �ҷ����� ����

class CObjectIO
{

private:
	CObjectIO() = default;
	~CObjectIO() = default;


public:
	// ������Ʈ Ÿ�Ժ��� ���� ����
	static void SaverObject(E_OBJECT_TYPE type, wstring FolderPath, wstring filename, CGameObject* obj);

	// �� ������ �Ѱܹް� ������ ����
	static void SaverData(ofstream* fwrite, E_OBJECT_DATA_TYPE type, const void * desc);

	// �����͸� �ҷ��ͼ� �����⿡ �ѱ�
	static void  LoadObject(wstring FolderPath, wstring filename, CGameObject* obj);
//	static void* LoadData(ifstream* fRead, E_OBJECT_DATA_TYPE type, string FolderPath, string filename);
//	static void* LoadData(ifstream* fRead, E_OBJECT_DATA_TYPE type, string FolderPath, string filename);


private:
	// �� �����ͺ��� ����
//	static void SaveData(ofstream * fwrite, const void* desc, _uint size);
	static void SaveOBJECT(ofstream* fwrite, E_OBJECT_TYPE* desc);
	static void SaveUIDESC(ofstream* fwrite, UIDESC* desc);
	static void SaveTEXTUREDESC(ofstream* fwrite, TEXTUREDESC* desc);



	// �� �����ͺ��� �ε�


};


END