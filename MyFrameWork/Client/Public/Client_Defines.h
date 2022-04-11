#pragma once

// 창 사이즈
static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

extern HINSTANCE		g_hInst;
extern HWND				g_hWnd;

extern double			g_Timer;

//struct ID3D11Device;
//struct ID3D11DeviceContext;
//
//extern ID3D11Device*		g_D3D11Device;
//extern ID3D11DeviceContext* g_D3D11DeviceContext;

namespace Client {}

using namespace Client;


// 데이터 타입 별 정보
enum E_OBJECT_DATA_TYPE
{
	OBJECT_TYPE_DATA_OBJECT,
	OBJECT_TYPE_DATA_UIDESC,
	OBJECT_TYPE_DATA_TEXTUREDESC,
	OBJECT_TYPE_DATA_END,
};


// 오브젝트 별 파일 정보
enum E_OBJECT_TYPE
{
	OBJECT_TYPE_UI,
	OBJECT_TYPE_END,

};


typedef struct tag_UIDESC
{
	_int mPosX ;
	_int mPosY ;
	_int mSizeX ;
	_int mSizeY ;
	_float2 mPivot;

}UIDESC;


typedef struct tag_TEXTUREDESC
{
	char mTextureKey[MAX_STR] = "";
}TEXTUREDESC;




#define  STR_FILEPATH_RESOURCE_L		L"..\\Bin\\Resources\\Sprite"
#define  STR_FILEPATH_RESOURCE_			"..\\Bin\\Resources\\Sprite"

#define  STR_FILEPATH_DATA_L		L"..\\Bin\\Resources\\Data"
#define  STR_FILEPATH_DATA			"..\\Bin\\Resources\\Data"
