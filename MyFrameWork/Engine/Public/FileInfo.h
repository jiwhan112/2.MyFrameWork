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
	void SaveVectorToDat(wstring savetxtName, wstring ExtensionName = L"");

	// �ش� Txt������ �о Path ����Ʈ ����
	list<MYFILEPATH*> Load_ExtensionList(wstring txtfilepath, string exe, bool bFlag);

	// Full �н��� ��θ� ����
	wstring Get_PathData(wstring Fullpath);

private:
	void SaveFilePathByVector(MYFILEPATH * path, wstring filepath, wstring filename);

	void FileOpenTest(wstring savetxtName);

private:
	list<MYFILEPATH*>		mListFilePathData;

	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
};
