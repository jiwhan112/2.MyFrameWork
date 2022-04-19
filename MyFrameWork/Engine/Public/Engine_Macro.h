#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define NEW new ( _NORMAL_BLOCK, __FILE__,__LINE__ )

#else
#define NEW new

#endif

#define DIS_Up				0b00000001
#define DIS_Down			0b00000110
#define DIS_DoubleDown		0b00000100
#define DIS_Press			0b11111111

// 색 관련
#define D3D11COLOR_ARGB(a,r,g,b) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

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

#define  MAX_FILENAME		256
#define  MAX_STR			256
#define  MAX_EXENAME		32

#define  MAX_BONES			128

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
				m_pInstance = NEW CLASSNAME;					\
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
#define NULL_CHECK_HR( _ptr) \
if( _ptr == 0){__debugbreak(); return E_FAIL;}

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

// #Tag STR_SHADER
#pragma region STR_SHADER

static const char* STR_TEX_DIFFUSE = "g_DiffuseTexture";
static const char* STR_TEX_NOMAL = "g_NormalTexture";

static const char* STR_MAT_WORLD = "g_WorldMatrix";
static const char* STR_MAT_VIEW = "g_ViewMatrix";
static const char* STR_MAT_PROJ = "g_ProjMatrix";

static const char* STR_LIGHT_DIR = "g_vLightDir";
static const char* STR_LIGHT_POS = "g_vLightPos";
static const char* STR_LIGHT_DIFFUSE = "g_vLightDiffuse";
static const char* STR_LIGHT_AMBIENT = "g_vLightAmbient";
static const char* STR_LIGHT_SPECULAR = "g_vLightSpecular";

static const char* STR_CAMPOS = "g_CameraPosition";

static const char* STR_BONES = "g_BoneMatrices";




#pragma endregion

// #Tag STR_FILEPATH
#pragma region STR_FILEPATH

	static const char* FILEPATH_RESOURCE = "..\\Bin\\Resources";
	static const char* FILEPATH_TXT_TEST = "..\\Bin\\Resources\\PathFinder\\test.txt";
	static const char* FILEPATH_TXT_RSOURCES = "..\\Bin\\Resources\\PathFinder\\path.txt";

#pragma endregion

