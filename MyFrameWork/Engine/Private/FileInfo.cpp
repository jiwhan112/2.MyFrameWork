#include "FileInfo.h"
#include "io.h"
#include <sstream>



IMPLEMENT_SINGLETON(CFileInfo);

//void CFileInfo::DirInfoExtraction(const wstring & FileFolder, list<FILEPATH*>& rPathInfoList, E_FILETYPE type)
//{
//	// ������ �ִ� ��� ���� ��� ����
//	wstring	wstrFilePath = FileFolder + L"\\*.*";
//
//	// mfc���� �����ϴ� ���� �� ��� ���� ���� Ŭ����
//	CFileFind		Find;
//
//	// �־��� ��ο� ������ ������ Ȯ���ϴ� �Լ�
//	// �������� ������ false ����, �����ϸ� true����
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
//			// GetFilePath : ���� ã�� ��θ� ������ �Լ�
//			DirInfoExtraction(wstring(Find.GetFilePath()), rPathInfoList, type);
//		}
//
//		else // ������ ã�� ��Ȳ
//		{
//			// ã���� �ý��� ������ ��� �ǳʶڴ�.
//			if (Find.IsSystem())
//				continue;
//
//			// Ȯ���� �˻� type
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
//			// ���� ��� ���¿��� ���ϸ��� Ȯ���� ���� �߶�
//			PathRemoveFileSpec(szPath);
//
//			// ������ ������ ���ؼ� ����ü ��� ������ ������ ä����
//			pImgPath->iCount = DirFileCount(szPath);
//
//			// GetFileTitle : ���ϸ��� ������ �Լ�(Ȯ���ڸ� ����)
//			// L"AKIHA_AKI01_000"
//			wstring	wstrTextureName = Find.GetFileTitle().GetString();
//
//			// substr(����, ��) : ���ۿ��� ���� �ش��ϴ� ���ڿ��� ������ �Լ�
//			// L"AKIHA_AKI01_00%d.png"
//			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d." + StrFiletype;
//			
//			TCHAR	szBuf[MAX_STR] = L"";
//			lstrcpy(szBuf, Find.GetFilePath().GetString());
//			// D:\����ȯ\124��\Frame124\Texture\Stage\Player\Attack\AKIHA_AKI01_000.png
//
//			PathRemoveFileSpec(szBuf);
//			// D:\����ȯ\124��\Frame124\Texture\Stage\Player\Attack\
//
//			// �� ���ڿ��� �����Ͽ� �ϼ��� ��θ� ù ��° ���ڰ��� ����
//			PathCombine(szBuf, szBuf, wstrTextureName.c_str());
//			// D:\����ȯ\124��\Frame124\Texture\Stage\Player\Attack\AKIHA_AKI01_00%d.png
//
//			pImgPath->wstrPath = ConvertRelativePath(szBuf);
//			// ..\Texture\Stage\Player\Attack\AKIHA_AKI01_00%d.png
//
//			PathRemoveFileSpec(szBuf);
//			// ..\Texture\Stage\Player\Attack\
//
//			// PathFindFileName : ���ϸ��� ã�ų� ���� ������ ��θ� ã�Ƴ��� �Լ�
//			pImgPath->wstrStateKey = PathFindFileName(szBuf);
//
//			PathRemoveFileSpec(szBuf);
//			// ..\Texture\Stage\Player\
//
//			// PathFindFileName : ���ϸ��� ã�ų� ���� ������ ��θ� ã�Ƴ��� �Լ�
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

static std::string to_utf8(const wchar_t* buffer, int len)
{
	int nChars = ::WideCharToMultiByte(
		CP_UTF8,
		0,
		buffer,
		len,
		NULL,
		0,
		NULL,
		NULL);
	if (nChars == 0) return "";

	string newbuffer;
	newbuffer.resize(nChars);
	::WideCharToMultiByte(
		CP_UTF8,
		0,
		buffer,
		len,
		const_cast<char*>(newbuffer.c_str()),
		nChars,
		NULL,
		NULL);

	return newbuffer;
}

static std::string to_utf8(const std::wstring& str)
{
	return to_utf8(str.c_str(), (int)str.size());
}

static vector<string> StringSplit(string input, char de)
{
	vector<string> answer;
	stringstream ss(input);
	string temp;

	while (getline(ss, temp, de))
	{
		answer.push_back(temp);
	}
	return answer;
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
		int checkDirFile = isFileOrDir(fd);//���� ��ü ���� Ȯ��(���� or ���丮)

		// ���丮��� ���
		switch (checkDirFile)
		{

		case 0: // ���丮 
		{
			string filename = fd.name;
			wstring newFolderPath = FileFolder;
			wstring newDirName;
			newDirName.assign(filename.begin(), filename.end());

			FolderFinder(newFolderPath + L"\\" + newDirName);
		}
			break;
		case 1: // ����
		{

			MYFILEPATH* myFilePath = NEW MYFILEPATH;

			string filename = fd.name;
			wstring newFullPath = FileFolder;
			wstring Filename;
			Filename.assign(filename.begin(), filename.end());
			SaveFilePathByVector(myFilePath, newFullPath + L"\\" + Filename, Filename);

			mListAllFile.push_front(myFilePath);
		}
			break;
		case 2: // �Ұ���
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
	for (int i =0; i< filename.size();i++)
	{
		if (filename[i] == L'.')
		{
			exeName += filename.substr(i+1, filename.size() - 1);
			break;
		}
	}

	lstrcpy(path->Extension, exeName.c_str());
	path->FileCount = 1;
}

void CFileInfo::SaveVectorToDat(const char* savetxtName)
{
	// txt���Ͽ� ��ü ���ҽ� ���� ����
	if (mListAllFile.empty())
		return;

	ofstream fWrite(savetxtName, ios::out);
	if (fWrite.is_open())
	{
		for (auto pathdata : mListAllFile)
		{
			wstring Fullpath = pathdata->FullPath;
			wstring FileName = pathdata->FileName;
			wstring Extension = pathdata->Extension;

			_tchar buf[16] = L"";
			_itow_s(pathdata->FileCount, buf, 10);
			wstring FileCount = buf;


			wstring filetxt =
				Fullpath + L'|' +
				FileName + L'|' +
				Extension + L'|' +
				FileCount + L'|';

			// ���� ����
			string outText = to_utf8(filetxt);
			fWrite << outText << "\n";
		}		
	}
	fWrite.close();
}

void CFileInfo::FileOpenTest(const char * savetxtName)
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

list<MYFILEPATH*> CFileInfo::Load_TexturePng(const char * txtfilepath)
{
	list<MYFILEPATH*> pngPathList;

	ifstream fRead(txtfilepath, ios::in);

	if (false == fRead.is_open())
		return pngPathList;

	string line;
	while (!fRead.eof())
	{ 
		getline(fRead, line);
		vector<string> vecStr = StringSplit(line,'|');
		if (vecStr.size() != 4)
			continue;

		if (vecStr[2] != "png")
			continue;

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

void CFileInfo::Free()
{
	if (mListAllFile.empty() == false)
	{
		for (auto pathdata :mListAllFile)
		{
			Safe_Delete(pathdata);
		}
		mListAllFile.clear();

	}
}


//l CFileInfo::FindType(CFileFind& Find, E_FILETYPE type)
//{
//	wstring StrCompare;
//	wstring StrFiletype = Find.GetFileName().Right(3);
//	// Ư�� Ȯ���ڸ� ã��
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