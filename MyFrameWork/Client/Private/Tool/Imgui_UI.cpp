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
	//�ؽ�ó �ε� ��
	
	// 1. �ؽ�ó ��ο��� ��� png ���� �˻�
	// 2. ��� ���� �ε� �ϰ� �̸� ���
	// 3. 

}

void CImgui_UI::UISaver()
{
	// UI �̹���
	// 
	// UI �ؽ�ó�� �������� 

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
