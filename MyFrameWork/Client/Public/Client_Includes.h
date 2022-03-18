#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: MY INCLUDES
#include "Client_Defines.h"
#include "../Public/Imgui/imgui.h"
#include "../Public/Imgui/imgui_impl_win32.h"
#include "../Public/Imgui/imgui_impl_dx11.h"