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


#define PLAYGAMESOUND GetSingle(CGameInstance)->PlaySound_
#define SOUNDVOL_EFFECT		0.8f
#define SOUNDVOL_EFFECT2	0.6f
#define SOUNDVOL_EFFECT3	1.f

#define  KEYDOWN(x)				GetSingle(CGameInstance)->Get_DIKeyState(x)&DIS_Down
#define  KEYUP(x)				GetSingle(CGameInstance)->Get_DIKeyState(x)&DIS_Up
#define  KEYPRESS(x)			GetSingle(CGameInstance)->Get_DIKeyState(x)&DIS_Press

#define  MOUSEDOWN(x)			GetSingle(CGameInstance)->Get_DIMouseButtonState(x)&DIS_Down
#define  MOUSEUP(x)				GetSingle(CGameInstance)->Get_DIMouseButtonState(x)&DIS_Up
#define  MOUSEPRESS(x)			GetSingle(CGameInstance)->Get_DIMouseButtonState(x)&DIS_Press



#define  STR_FILEPATH_RESOURCE_SPRITE_L				L"..\\Bin\\Resources\\Sprite"
#define  STR_FILEPATH_RESOURCE_DAT_L				L"..\\Bin\\Resources\\data"
#define  STR_FILEPATH_RESOURCE_3DMODEL_L			L"..\\Bin\\Resources\\Game"
#define  STR_FILEPATH_RESOURCE_3DMODEL_STATIC_L		L"..\\Bin\\Resources\\Game\\StaticModel"
#define  STR_FILEPATH_RESOURCE_3DMODEL_DYNAMIC_L	L"..\\Bin\\Resources\\Game\\DynamicModel"
#define  STR_FILEPATH_RESOURCE_PATH_L				L"..\\Bin\\Resources\\PathTxT"
#define  STR_FILEPATH_RESOURCE_L					L"..\\Bin\\Resources"

#define  STR_FILEPATH_RESOURCE_DATPATHTXT_L			L"..\\Bin\\Resources\\PathTxT\\DatPath.txt"
#define  STR_FILEPATH_RESOURCE_SPRITETXT_L			L"..\\Bin\\Resources\\PathTxT\\SpritePath.txt"
#define  STR_FILEPATH_RESOURCE_3DTEXPATHHTXT_L			L"..\\Bin\\Resources\\PathTxT\\Sprite3DPath.txt"
#define  STR_FILEPATH_RESOURCE_3DPATHTXT_STATIC_L	L"..\\Bin\\Resources\\PathTxT\\3DPath_Static.txt"
#define  STR_FILEPATH_RESOURCE_3DPATHTXT_DYNAMIC_L	L"..\\Bin\\Resources\\PathTxT\\3DPath_Dynamic.txt"

#define STR_FILENAME_DATA_TXT						L"DatPath.txt"
#define STR_FILENAME_SPRITE_TXT						L"SpritePath.txt"
#define STR_FILENAME_3DSPRITE_TXT					L"Sprite3DPath.txt"
#define STR_FILENAME_3DSTATIC_TXT					L"3DPath_Static.txt"
#define STR_FILENAME_3DDYNAMIC_TXT					L"3DPath_Dynamic.txt"
