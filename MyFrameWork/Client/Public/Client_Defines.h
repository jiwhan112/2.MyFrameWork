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







#define  STR_FILEPATH_RESOURCE_SPRITE_L			L"..\\Bin\\Resources\\Sprite"
#define  STR_FILEPATH_RESOURCE_DAT_L			L"..\\Bin\\Resources\\data"
#define  STR_FILEPATH_RESOURCE_3DMODEL_L		L"..\\Bin\\Resources\\TestFBX"
#define  STR_FILEPATH_RESOURCE_PATH_L			L"..\\Bin\\Resources\\PathTxT"
#define  STR_FILEPATH_RESOURCE_L				L"..\\Bin\\Resources"

#define  STR_FILEPATH_RESOURCE_DATPATHTXT_L		L"..\\Bin\\Resources\\PathTxT\\DatPath.txt"
#define  STR_FILEPATH_RESOURCE_SPRITETXT_L		L"..\\Bin\\Resources\\PathTxT\\SpritePath.txt"
#define  STR_FILEPATH_RESOURCE_3DPATHTXT_L		L"..\\Bin\\Resources\\PathTxT\\3DPath.txt"
