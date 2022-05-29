#pragma once

namespace Engine
{
	class CTagFinder
	{
	public:
		CTagFinder(const wchar_t* pTag) : m_pTag(pTag) {}
		~CTagFinder() = default;

	public:
		template <typename T>
		bool operator () (T& Pair)
		{
			if (0 == lstrcmp(Pair.first, m_pTag))
				return true;
			return false;
		}

	private:
		const wchar_t*			m_pTag = nullptr;
	};

	template <typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete[] pPointer;
			pPointer = nullptr;
		}
	}

	template <typename T>
	unsigned long Safe_AddRef(T& pInstance)
	{
		unsigned long		dwRefCnt = 0;

		if (nullptr != pInstance)
			dwRefCnt = pInstance->AddRef();

		return dwRefCnt;
	}

	template <typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}

		return dwRefCnt;
	}

	// wstr <-> str º¯È¯

	class CHelperClass
	{
	public:
		static const void Convert_string_wstring(wstring& wstr, string& str, bool wstr2str)
		{
			if (wstr2str)
			{
				str.assign(wstr.begin(), wstr.end());
			}
			else
			{
				wstr.assign(str.begin(), str.end());
			}
		}
		static const string Convert_Wstr2str(wstring& wstr)
		{
			string str;
			str.assign(wstr.begin(), wstr.end());
			return str;
		}

		static const wstring Convert_str2wstr(string& str)
		{
			wstring wstr;
			wstr.assign(str.begin(), str.end());
			return wstr;
		}


		static vector<string> String_Split(string input, char delimiter)
		{
			vector<string> answer;
			stringstream ss(input);
			string temp;

			while (getline(ss, temp, delimiter))
			{
				answer.push_back(temp);
			}

			return answer;
		}

		static int RandomInt(int min, int max)
		{
			if (max <= min)
				return min;

			return rand() % (min + max) - min;
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

		template <typename Container>
		static bool in_quote(const Container& cont, const std::string& s)
		{
			return std::search(cont.begin(), cont.end(), s.begin(), s.end()) !=
				cont.end();
		}



	};
}