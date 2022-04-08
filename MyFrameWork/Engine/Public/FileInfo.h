#pragma once
#include "Base.h"

// ���� ���� ���� ���� Ŭ����
// �н� ���δ�
//	���� ���� �����ؼ� txt ������ ����

// ���� �ε� / ����� ���




class CFileInfo
	:public CBase
{
	DECLARE_SINGLETON(CFileInfo);


private:
	CFileInfo() = default;
	~CFileInfo() = default;

public:
	// ������ Ž���ϸ鼭 ��� ���� ����Ʈ ����
	HRESULT			FolderFinder(const wstring& FileFolder);

	// TXT ���Ϸ� MYFILEPATH ���� ���� // ��� �����̸� Ȯ����
	void SaveVectorToDat(const char* savetxtName = FILEPATH_TXT_RSOURCES);

	// TXT ������ �о PNG ���� ����Ʈ ����
	list<MYFILEPATH*> Load_TexturePng(const char* txtfilepath);


private:
	void SaveFilePathByVector(MYFILEPATH * path, wstring filepath, wstring filename);
	
	void FileOpenTest(const char* savetxtName = FILEPATH_TXT_TEST);

	
//	HRESULT Load_FBX(const char* txtfilepath);

	HRESULT Get_();
	HRESULT LoadMYPath();


private:
	list<MYFILEPATH*>		mListAllFile;

	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
};


