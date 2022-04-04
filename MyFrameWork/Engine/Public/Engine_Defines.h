#pragma once

#pragma warning (disable : 4251)
#pragma warning(disable : 4010)

// d3d11.h만 추가하면 된다.
// d3d11.lib;
#include <d3d11.h>
#include <DirectXMath.h>
#include <typeinfo>

// 셰이더 관련 Include
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include <DirectXCollision.h>


// 텍스처 로드 관련 Include
// DDS 및 이외의 텍스처 로드 단 tga는 지원 안함
// DDS가 가장 빠르다.

// 도큐먼트: https://github.com/microsoft/DirectXTK12/wiki/DirectXTK
#include <Direct_TK/DDSTextureLoader.h>
#include <Direct_TK/WICTextureLoader.h>
#include <Direct_TK/ScreenGrab.h>
#include <wincodec.h>

// assimp 설명
// https://github.com/assimp/assimp/wiki/FBX-File-format
// FBX 설명
// https://banexdevblog.wordpress.com/2014/06/23/a-quick-tutorial-about-the-fbx-ascii-format/

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
using namespace Assimp;

using namespace DirectX;

#define DIRECTINPUT_VERSION 0x0800
// Dinput에 필요한 라이브러리
// dinput8.lib;dxguid.lib;
#include <dinput.h>
#include <process.h>

//#include <string>
//#include <fstream>
//#include <tchar.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <time.h>

using namespace std;

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Engine_Easing.h"

using namespace Engine;
