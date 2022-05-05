#include "stdafx.h"
#include "Tool/Imgui_Base.h"
#include "GameObject.h"

CImgui_Base::CImgui_Base(ID3D11Device * device, ID3D11DeviceContext * context)
	:mDevice(nullptr), mDeviceContext(nullptr)
{
}

void CImgui_Base::Free()
{
	Safe_Release(mDevice);
	Safe_Release(mDeviceContext);
}