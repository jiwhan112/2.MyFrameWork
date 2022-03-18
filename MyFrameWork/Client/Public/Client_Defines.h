#pragma once

// 창 사이즈
static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_BOSS, LEVEL_END };

extern HINSTANCE		g_hInst;
extern HWND				g_hWnd;

struct ID3D11Device;
struct ID3D11DeviceContext;

extern ID3D11Device*		g_D3D11Device;
extern ID3D11DeviceContext* g_D3D11DeviceContext;

// extern float				g_Timer;

namespace Client {}

using namespace Client;

