#include "FileInfo.h"
#include "io.h"
#include <sstream>

IMPLEMENT_SINGLETON(CFileInfo);

//void CFileInfo::DirInfoExtraction(const wstring & FileFolder, list<FILEPATH*>& rPathInfoList, E_FILETYPE type)
//{
//	// 폴더에 있는 모든 파일 경로 저장
//	wstring	wstrFilePath = FileFolder + L"\\*.*";
//
//	// mfc에서 제공하는 파일 및 경로 제어 관련 클래스
//	CFileFind		Find;
//
//	// 주어진 경로에 파일의 유무를 확인하는 함수
//	// 존재하지 않으면 false 리턴, 존재하면 true리턴
//	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());
//
//	while (bContinue)
//	{
//		bContinue = Find.FindNextFile();
//
//		if (Find.IsDots())
//			continue;
//
//		else if (Find.IsDirectory())
//		{
//			// GetFilePath : 현재 찾은 경로를 얻어오는 함수
//			DirInfoExtraction(wstring(Find.GetFilePath()), rPathInfoList, type);
//		}
//
//		else // 파일을 찾은 상황
//		{
//			// 찾은게 시스템 파일인 경우 건너뛴다.
//			if (Find.IsSystem())
//				continue;
//
//			// 확장자 검사 type
//			if (false == FindType(Find, type))
//				continue;
//
//			wstring StrFiletype = Find.GetFileName().Right(3);
//
//			IMGPATH*		pImgPath = new IMGPATH;
//			TCHAR			szPath[MAX_PATH] = L"";
//
//			lstrcpy(szPath, Find.GetFilePath().GetString());
//
//			// 현재 경로 상태에서 파일명과 확장자 명을 잘라냄
//			PathRemoveFileSpec(szPath);
//
//			// 파일의 개수를 구해서 구조체 멤버 변수의 값으로 채워줌
//			pImgPath->iCount = DirFileCount(szPath);
//
//			// GetFileTitle : 파일명만 얻어오는 함수(확장자명 제외)
//			// L"AKIHA_AKI01_000"
//			wstring	wstrTextureName = Find.GetFileTitle().GetString();
//
//			// substr(시작, 끝) : 시작에서 끝에 해당하는 문자열을 얻어오는 함수
//			// L"AKIHA_AKI01_00%d.png"
//			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d." + StrFiletype;
//
//			TCHAR	szBuf[MAX_STR] = L"";
//			lstrcpy(szBuf, Find.GetFilePath().GetString());
//			// D:\유준환\124기\Frame124\Texture\Stage\Player\Attack\AKIHA_AKI01_000.png
//
//			PathRemoveFileSpec(szBuf);
//			// D:\유준환\124기\Frame124\Texture\Stage\Player\Attack\
//
//			// 두 문자열을 결합하여 완성된 경로를 첫 번째 인자값에 저장
//			PathCombine(szBuf, szBuf, wstrTextureName.c_str());
//			// D:\유준환\124기\Frame124\Texture\Stage\Player\Attack\AKIHA_AKI01_00%d.png
//
//			pImgPath->wstrPath = ConvertRelativePath(szBuf);
//			// ..\Texture\Stage\Player\Attack\AKIHA_AKI01_00%d.png
//
//			PathRemoveFileSpec(szBuf);
//			// ..\Texture\Stage\Player\Attack\
//
//			// PathFindFileName : 파일명을 찾거나 가장 마지막 경로를 찾아내는 함수
//			pImgPath->wstrStateKey = PathFindFileName(szBuf);
//
//			PathRemoveFileSpec(szBuf);
//			// ..\Texture\Stage\Player\
//
//			// PathFindFileName : 파일명을 찾거나 가장 마지막 경로를 찾아내는 함수
//			pImgPath->wstrObjKey = PathFindFileName(szBuf);
//
//			rPathInfoList.push_back(pImgPath);
//
//			Find.FindFile(Find.GetFilePath());
//			Find.FindNextFile();
//		}
//	}
//}

static int isFileOrDir(_finddatai64_t fd)
{
	/*
	#define _A_NORMAL 0x00 // Normal file - No read/write restrictions
	#define _A_RDONLY 0x01 // Read only file
	#define _A_HIDDEN 0x02 // Hidden file
	#define _A_SYSTEM 0x04 // System file
	#define _A_SUBDIR 0x10 // Subdirectory
	#define _A_ARCH   0x20 // Archive file
	*/

	// 파일인지 디렉토리인지 확인

	if (fd.name[0] == '.')
		return 2;

	if (fd.attrib == 16)
	{
		return 0;
	}
	else if (fd.attrib & 32)
	{
		return 1;
	}

	return 2;
}



HRESULT CFileInfo::FolderFinder(const wstring& FileFolder)
{
	wstring	wstrFilePath = FileFolder + L"\\*.*";
	string strFilePath;
	strFilePath.assign(wstrFilePath.begin(), wstrFilePath.end());
	//	wstrFilePath.assign(strFilePath.begin(), strFilePath.end());

	struct _finddatai64_t fd;

	intptr_t handle = _findfirsti64(strFilePath.c_str(), &fd);

	do
	{
		int checkDirFile = isFileOrDir(fd);//현재 객체 종류 확인(파일 or 디렉토리)

		// 디렉토리라면 재귀
		switch (checkDirFile)
		{
		case 0: // 디렉토리
		{
			string filename = fd.name;
			wstring newFolderPath = FileFolder;
			wstring newDirName;
			newDirName.assign(filename.begin(), filename.end());

			FolderFinder(newFolderPath + L"\\" + newDirName);
		}
		break;
		case 1: // 파일
		{
			MYFILEPATH* myFilePath = NEW MYFILEPATH;

			string filename = fd.name;
			wstring newFullPath = FileFolder;
			wstring Filename;
			Filename.assign(filename.begin(), filename.end());
			SaveFilePathByVector(myFilePath, newFullPath + L"\\" + Filename, Filename);

			mListFilePathData.push_front(myFilePath);
		}
		break;
		case 2: // 불가능
			break;
		}
	} while (_findnexti64(handle, &fd) == 0);

	_findclose(handle);

	return S_OK;
}

void CFileInfo::SaveFilePathByVector(MYFILEPATH * path, wstring filepath, wstring filename)
{
	if (path == nullptr)
		path = NEW MYFILEPATH;

	lstrcpy(path->FullPath, filepath.c_str());
	lstrcpy(path->FileName, filename.c_str());

	wstring exeName = L"";
	for (int i = 0; i < filename.size(); i++)
	{
		if (filename[i] == L'.')
		{
			exeName += filename.substr(i + 1, filename.size() - 1);
			break;
		}
	}

	lstrcpy(path->Extension, exeName.c_str());
	path->FileCount = 1;
}

void CFileInfo::SaveVectorToDat(wstring savetxtName, wstring ExtensionName)
{
	bool isAllFile = true;
	if (ExtensionName.size() > 2)
		isAllFile = false;

	// txt파일에 전체 리소스 정보 저장
	if (mListFilePathData.empty())
		return;

	ofstream fWrite(savetxtName, ios::out);
	if (fWrite.is_open())
	{
		for (auto pathdata : mListFilePathData)
		{
			wstring Fullpath = pathdata->FullPath;
			wstring FileName = pathdata->FileName;
			wstring Extension = pathdata->Extension;

			if (isAllFile == false && Extension != ExtensionName)
				continue;

			_tchar buf[16] = L"";
			_itow_s(pathdata->FileCount, buf, 10);
			wstring FileCount = buf;

			wstring filetxt =
				Fullpath + L'|' +
				FileName + L'|' +
				Extension + L'|' +
				FileCount + L'|';

			// 문서 저장
			string outText = CHelperClass::to_utf8(filetxt);
			fWrite << outText << "\n";
		}
	}
	if (mListFilePathData.empty() == false)
	{
		for (auto pathdata : mListFilePathData)
		{
			Safe_Delete(pathdata);
		}
		mListFilePathData.clear();
	}
	fWrite.close();
}

void CFileInfo::FileOpenTest(wstring savetxtName)
{
	return;
	//ofstream fWrite(savetxtName, ios::out);
	//if (fWrite.is_open())
	//{
	//	wstring Fullpath = L"asdawdsdwdsa";
	//	string outtext = to_utf8(Fullpath);
	//	fWrite << outtext<< outtext;

	//}
	//if (fWrite.is_open())
	//	fWrite.close();
}

list<MYFILEPATH*> CFileInfo::Load_ExtensionList(wstring txtfilepath, string exe,bool bflag)
{
	list<MYFILEPATH*> pngPathList;

	ifstream fRead(txtfilepath, ios::in);

	if (false == fRead.is_open())
		return pngPathList;

	string line;
	while (!fRead.eof())
	{
		getline(fRead, line);
		vector<string> vecStr = CHelperClass::StringSplit(line, '|');
		if (vecStr.size() != 4)
			continue;
		if (bflag)
		{
			if (vecStr[2] != exe)
				continue;
		}
		else
		{
			if (vecStr[2] == exe)
				continue;
		}
		MYFILEPATH* path = NEW MYFILEPATH;

		vector<wstring>  vecWStr;
		vecWStr.reserve(vecStr.size());

		for (auto str : vecStr)
		{
			wstring wstr;
			wstr.assign(str.begin(), str.end());
			vecWStr.push_back(wstr);
		}

		lstrcpy(path->FullPath, vecWStr[0].c_str());
		lstrcpy(path->FileName, vecWStr[1].c_str());
		lstrcpy(path->Extension, vecWStr[2].c_str());
		path->FileCount = 1;
		pngPathList.push_front(path);
	}

	fRead.close();

	return pngPathList;
}

wstring CFileInfo::Get_PathData(wstring Fullpath)
{
	wstring returnPath = L"";
	wstring::iterator endIter = Fullpath.end();
	endIter--;
	_uint cnt = Fullpath.length();

	for (; endIter != Fullpath.begin(); endIter--)
	{
		if (Fullpath[cnt--] == '\\')
		{
			endIter++;
			break;
		}
	}

	returnPath.assign(Fullpath.begin(), endIter);
	returnPath += '\\';

	return returnPath;
}

void CFileInfo::Free()
{
	if (mListFilePathData.empty() == false)
	{
		for (auto pathdata : mListFilePathData)
		{
			Safe_Delete(pathdata);
		}
		mListFilePathData.clear();
	}
}

//l CFileInfo::FindType(CFileFind& Find, E_FILETYPE type)
//{
//	wstring StrCompare;
//	wstring StrFiletype = Find.GetFileName().Right(3);
//	// 특정 확장자만 찾음
//	switch (type)
//	{
//	case FILETYPE_PNG:
//		StrCompare = L"png";
//		break;
//	case FILETYPE_DDS:
//		StrCompare = L"dds";
//		break;
//	case FILETYPE_XML:
//		StrCompare = L"xml";
//		break;
//	case FILETYPE_DAT:
//		StrCompare = L"dat";
//		break;
//	}
//
//	if (StrFiletype != StrCompare)
//		return  false;
//	return true;
//}