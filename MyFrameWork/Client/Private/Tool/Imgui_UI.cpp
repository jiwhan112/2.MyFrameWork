#include "stdafx.h"
#include "Tool/Imgui_UI.h"


CImgui_UI::CImgui_UI(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_UI::NativeConstruct()
{
	
	return S_OK;
}

HRESULT CImgui_UI::Update(_double time)
{


	return S_OK;
}


void CImgui_UI::TextureLoad()
{
	//텍스처 로드 툴
	
	// 1. 텍스처 경로에서 모든 png 파일 검사
	// 2. 모든 파일 로드 하고 이름 출력
	// 3. 

}

void CImgui_UI::UISaver()
{
	// UI 이미지
	// 
	// UI 텍스처가 보여지고 

}


CImgui_UI * CImgui_UI::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_UI*	pInstance = NEW CImgui_UI(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_UI::Free()
{
	__super::Free();

}
