#pragma once



#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

//#define GET_INSTANCE(CLASSNAME)	[](){											\
//	CLASSNAME*	pInstance = CLASSNAME::GetInstance();							\
//	if(nullptr == pInstance) {													\
//	char	szMessage[MAX_PATH] = "";											\
//	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
//	strcat_s(szMessage, "is nullptr");											\
//	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
//	else {																		\
//	pInstance->AddRef();}														\
//	return pInstance;															\
//	}();
//
//#define RELEASE_INSTANCE(CLASSNAME)	[](){										\
//	CLASSNAME*	pInstance = CLASSNAME::GetInstance();							\
//	if(nullptr == pInstance) {													\
//	char	szMessage[MAX_PATH] = "";											\
//	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
//	strcat_s(szMessage, "is nullptr");											\
//	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
//	else {																		\
//	pInstance->Release();}														\
//	}();

#define GetSingle(SINGLETON) SINGLETON::GetInstance()

#define DIS_Up				0b00000001
#define DIS_Down			0b00000110
#define DIS_DoubleDown		0b00000100
#define DIS_Press			0b11111111

#define BEGIN(NAMESPACE) namespace NAMESPACE { 

#define END }

#define MSGBOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK);

#define NO_COPY(CLASSNAME)										\
		private:												\
		CLASSNAME(const CLASSNAME&);							\
		CLASSNAME& operator = (const CLASSNAME&);				

#define DECLARE_SINGLETON(CLASSNAME)							\
		NO_COPY(CLASSNAME)										\
		private:												\
		static CLASSNAME*	m_pInstance;						\
		public:													\
		static CLASSNAME*	GetInstance( void );				\
		static unsigned long DestroyInstance( void );					

#define IMPLEMENT_SINGLETON(CLASSNAME)							\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;				\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{			\
			if(nullptr == m_pInstance) {						\
				m_pInstance = new CLASSNAME;					\
			}													\
			return m_pInstance;									\
		}														\
		unsigned long CLASSNAME::DestroyInstance( void ) {		\
			unsigned long dwRefCnt = 0;							\
			if(nullptr != m_pInstance)	{						\
				dwRefCnt = m_pInstance->Release();				\
				if(0 == dwRefCnt)								\
				m_pInstance = nullptr;							\
			}													\
			return dwRefCnt;									\
		}

#define  UPDATENONE		0
#define  UPDATEERROR	-1

// 디버그 체크 메크로
// {__asm { int 3; }; 코드 레벨에서 Break 포인트 걸기 86비트 전용
// __debugbreak(); 코드 레벨에서 Break 포인트 걸기 64비트 전용

// NULL 체크1 / 기본 
#define NULL_CHECK( _ptr) \
if( _ptr == 0){__debugbreak(); return;}


// NULL 체크2 / 노 리턴 
#define NULL_CHECK_BREAK( _ptr) \
if( _ptr == 0){__debugbreak();}

// NULL 체크3 / 리턴 수정 
#define NULL_CHECK_RETURN( _ptr, _return) \
if( _ptr == 0){__debugbreak(); return _return;}

// NULL 체크4 / 메세지박스
#define NULL_CHECK_MSG( _ptr, _message) \
if( _ptr == 0){MessageBox(NULL, _message, L"System Message", MB_OK);__debugbreak();}

// NULL 체크4 / 메세지박스 / 리턴 수정
#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message ) \
if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__debugbreak();return _return;}

// IF 체크 / TRUE
#define IF_CHECK_TRUE( _IF) \
if(_IF){__debugbreak();}

// IF 체크 / FALSE
#define IF_CHECK_FALSE( _IF) \
if(_IF){__debugbreak();}


//#define IF_CHECK_MESSAGE( _IF, _message) \
if( _IF){MessageBox(NULL, _message, L"System Message", MB_OK);__debugbreak();};


// HRESULT 오류체크

// HRESULT 오류체크1 / 기본
#define FAILED_CHECK(_hr) \
if(((HRESULT)(_hr)) < 0) { MSGBOX("Fail"); __debugbreak();\
return E_FAIL;}

// HRESULT 오류체크2 / 노리턴
#define FAILED_CHECK_NONERETURN(_hr) \
if(((HRESULT)(_hr)) < 0) { MSGBOX("Fail"); __debugbreak();}

// HRESULT 오류체크3 / 리턴 수정
#define FAILED_CHECK_RETURN(_hr, _return) if( ((HRESULT)(_hr)) < 0 ) \
{ MessageBox(NULL, L"Failed", L"System Error",MB_OK); __debugbreak();\
return _return;}

// HRESULT 오류체크4 / 메세지 수정
#define FAILED_CHECK_MSG( _hr, _message) if( ((HRESULT)(_hr)) < 0 ) \
{ MessageBox(NULL, _message, L"System Message",MB_OK); __debugbreak();return E_FAIL;}

// HRESULT 오류체크5 / 리턴 메세지 수정
#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message) if( ((HRESULT)(_hr)) < 0 ) \
{ MessageBox(NULL, _message, L"System Message"sp,MB_OK); __debugbreak();return _return;}



#define FAILED_UPDATE(_func) if(0>_func) {__debugbreak(); return -1;}