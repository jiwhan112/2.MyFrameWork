#pragma once

#pragma warning (disable : 4251)

// d3d11.h만 추가하면 된다.
// d3d11.lib;
#include <d3d11.h>
#include <DirectXMath.h>
#include <typeinfo>

using namespace DirectX;


#define DIRECTINPUT_VERSION 0x0800
// Dinput에 필요한 라이브러리
// dinput8.lib;dxguid.lib;
#include <dinput.h>

#include <process.h>

#include <vector>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG


#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"

using namespace Engine;
