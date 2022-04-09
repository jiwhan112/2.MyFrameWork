#include "stdafx.h"
#include "Tool/Imgui_UI.h"
#include "GameObject/GameObject_2D.h"


CImgui_UI::CImgui_UI(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_UI::NativeConstruct()
{
	mUIObject = nullptr;
	return S_OK;
}

HRESULT CImgui_UI::Update(_double time)
{

	FAILED_CHECK(Render_UI());

	return S_OK;
}

void CImgui_UI::Set_UIObject(CGameObject_2D * obj)
{
	Safe_Release(mUIObject);
	mUIObject = obj;
	Safe_AddRef(mUIObject);
}

HRESULT CImgui_UI::Render_UI()
{
	if (ImGui::Begin(TAG_IMGUI(CImgui_Base::IMGUI_CHANEL_TEST)))
	{
		if (ImGui::CollapsingHeader("UI"))
		{
			
			ImGui::Combo("Mode", &item_current, mUIItems, IM_ARRAYSIZE(mUIItems));
			switch (item_current)
			{
			case 0:
				if (ImGui::Button("PathFinderButton"))
				{
					// Resoure�� �ִ� ��� ���ҽ� ������ TXT�� ����
					Button_PathTxtSave();
				}
				break;
			case 1:
				// 2D ������Ʈ ���� �Լ�
				FAILED_CHECK(Edit_UIObject());
				FAILED_CHECK(Edit_Texture());

				break;
			}
			// Load
			


		}

		//static bool bAppOveraly = false;
		//ImGui::Checkbox("Check", &bAppOveraly);

		ImGui::End();
	}
	return S_OK;
}


void CImgui_UI::Button_PathTxtSave()
{
	// �ؽ�ó �н����δ�

	// Ž��
	GetSingle(CGameInstance)->FolderFinder(STR_FILEPATH_RESOURCE);
	// Ž���� ����Ʈ TXT ����
	GetSingle(CGameInstance)->SaveVectorToDat();

}

void CImgui_UI::Button_TextureLoader()
{
	// �ؽ�ó �ε� ��

	// ������� �ؽ�ó �ε� ����

}

HRESULT CImgui_UI::Edit_UIObject()
{
	if (mUIObject == nullptr)
		return E_FAIL;

	// UIDesc ������ �������ָ� �˾Ƽ� ������Ʈ ������Ʈ �ÿ� �����ȴ�.
	UIDESC myDesc = mUIObject->Get_UIDesc();

	ImGui::Text("Pos"); 
	ImGui::SliderInt("PosX", &myDesc.mPosX, 0, g_iWinCX, "PosX = %d");
	ImGui::SliderInt("PosY", &myDesc.mPosY,0, g_iWinCY, "PosY= %d");

	ImGui::Text("size");
	ImGui::SliderInt("sizex", &myDesc.mSizeX, 1, g_iWinCX, "sizex = %d");
	ImGui::SliderInt("sizey", &myDesc.mSizeY, 1, g_iWinCY, "sizey = %d");
	mUIObject->Set_LoadUIDesc(myDesc);

	



	// �ؽ�ó ����
	//CTexture_map * Com_TextureMap = static_cast<CTexture_map*>(mUIObject->Get_Component(TAGCOM(COMPONENT_TEXTURE_MAP)));
	//if (Com_Trans == nullptr)
	//	return E_FAIL;
	//

	// ������ ���̺�




	return S_OK;

}

HRESULT CImgui_UI::Edit_Texture()
{
	// �ؽ�ó ���� ȭ�� �����

	// ��� �ؽ�ó �̸� ����

	IMGUI_TREE_BEGIN("Textures")
	{
		static int selectedTex = -1;
		if (mListTextureKey == nullptr)
			mListTextureKey = (mUIObject->Get_TextureMap()->Get_MapKeyList());

		_uint cnt = 0;
		string selectTeture = "";
		for ( auto iter = mListTextureKey->begin(); iter !=  mListTextureKey->end();++cnt,iter++)
		{
			if (ImGui::Selectable(iter->c_str(), selectedTex == cnt))
			{
				selectedTex = cnt;
				selectTeture = *iter;
				mUIObject->Get_TextureMap()->Set_TextureMap(selectTeture);

			}	
		}



		IMGUI_TREE_END

	}
	return S_OK;
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
	Safe_Release(mUIObject);


}
