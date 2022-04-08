#pragma once
#include "Base.h"

// 파일 정보 추출 정적 클래스
// 패스 파인더
//	파일 정보 추출해서 txt 파일을 만듬

// 파일 로드 / 저장시 사용




class CFileInfo
	:public CBase
{
	DECLARE_SINGLETON(CFileInfo);


private:
	CFileInfo() = default;
	~CFileInfo() = default;

public:
	// 폴더를 탐색하면서 모든 파일 리스트 저장
	HRESULT			FolderFinder(const wstring& FileFolder);

	// TXT 파일로 MYFILEPATH 정보 저장 // 경로 파일이름 확장자
	void SaveVectorToDat(const char* savetxtName = FILEPATH_TXT_RSOURCES);

	// TXT 파일을 읽어서 PNG 파일 리스트 제작
	list<MYFILEPATH*> Load_TexturePng(const char* txtfilepath);


private:
	void SaveFilePathByVector(MYFILEPATH * path, wstring filepath, wstring filename);
	
	void FileOpenTest(const char* savetxtName = FILEPATH_TXT_TEST);

	
//	HRESULT Load_FBX(const char* txtfilepath);

	HRESULT Get_();
	HRESULT LoadMYPath();


private:
	list<MYFILEPATH*>		mListAllFile;

	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};


