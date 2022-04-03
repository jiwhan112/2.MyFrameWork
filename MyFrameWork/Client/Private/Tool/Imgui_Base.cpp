#include "stdafx.h"
#include "Tool/Imgui_Base.h"

CImgui_Base::CImgui_Base(ID3D11Device * device, ID3D11DeviceContext * context)
	:mDevice(mDevice), mDeviceContext(context)
{
}

void CImgui_Base::Free()
{
	Safe_Release(mDevice);
	Safe_Release(mDeviceContext);
}
