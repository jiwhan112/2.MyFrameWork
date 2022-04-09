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




typedef struct tag_UIDESC
{
	_int mPosX;
	_int mPosY;
	_int mSizeX;
	_int mSizeY;
	_float2 mPivot;
	bool	mIsScreen;

}UIDESC;


#define  STR_FILEPATH_RESOURCE L"..\\Bin\\Resources\\Sprite"
