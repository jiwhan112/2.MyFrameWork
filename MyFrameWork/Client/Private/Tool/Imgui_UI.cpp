#include "stdafx.h"
#include "Tool/Imgui_UI.h"
#include "GameObject/GameObject_2D.h"
#include "FIleIO/ObjectIO.h"

CImgui_UI::CImgui_UI(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_UI::NativeConstruct()
{
	mCurrentUIObject = nullptr;
	return S_OK;
}

HRESULT CImgui_UI::Update(_double time)
{
	FAILED_CHECK(Render_UI());

	return S_OK;
}

void CImgui_UI::Set_UIObject(CGameObject_2D * obj)
{
	Safe_Release(mCurrentUIObject);
	mCurrentUIObject = obj;
	Safe_AddRef(mCurrentUIObject);
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
				if (ImGui::Button("Path_For_Sprite"))
				{
					// Resoure에 있는 모든 리소스 데이터 TXT로 저장
					Button_PathTxtSave(STR_FILEPATH_RESOURCE_SPRITE_L, STR_FILEPATH_RESOURCE_PATH_L,L"SpritePath.txt");
				}
				if (ImGui::Button("Path_For_Dat"))
				{
					// Resoure에 있는 모든 리소스 데이터 TXT로 저장
					Button_PathTxtSave(STR_FILEPATH_RESOURCE_DAT_L, STR_FILEPATH_RESOURCE_PATH_L,L"DatPath.txt");
				}
				if (ImGui::Button("Path_For_3D"))
				{
					// Resoure에 있는 모든 리소스 데이터 TXT로 저장
					Button_PathTxtSave(STR_FILEPATH_RESOURCE_3DMODEL_L, STR_FILEPATH_RESOURCE_PATH_L,L"3DPath.txt");
				}



				if (ImGui::Button("SaveTest"))
				{
					CObjectIO::SaverObject(OBJECT_TYPE_UI, STR_FILEPATH_RESOURCE_DAT_L,L"name.dat", mCurrentUIObject);

				}
				if (ImGui::Button("LoadTest"))
				{
					CObjectIO::LoadObject(STR_FILEPATH_RESOURCE_DAT_L, L"name.dat", mCurrentUIObject);
				}

				break;
			case 1:
				// 2D 오브젝트 수정 함수
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

void CImgui_UI::Button_PathTxtSave(wstring path, wstring txtpath, wstring txtname)
{
	// 텍스처 패스파인더

	// 탐색
	GetSingle(CGameInstance)->FolderFinder(path);

	// 탐색한 리스트 TXT 저장
	GetSingle(CGameInstance)->SaveVectorToDat(txtpath + L"\\" + txtname);
}

void CImgui_UI::Button_TextureLoader()
{
	// 텍스처 로드 툴

	// 스레드로 텍스처 로드 예정
}

HRESULT CImgui_UI::Edit_UIObject()
{
	if (mCurrentUIObject == nullptr)
		return E_FAIL;

	// UIDesc 정보만 변경해주면 알아서 오브젝트 업데이트 시에 수정된다.
	UIDESC myDesc = mCurrentUIObject->Get_UIDesc();

	IMGUI_TREE_BEGIN("Position")
	{
		ImGui::SliderInt("PosX", &myDesc.mPosX, 0, g_iWinCX, "PosX = %d");
		ImGui::SliderInt("PosY", &myDesc.mPosY, 0, g_iWinCY, "PosY= %d");
		IMGUI_TREE_END
	}

	IMGUI_TREE_BEGIN("Size")
	{
		ImGui::SliderInt("sizex", &myDesc.mSizeX, 1, g_iWinCX, "sizex = %d");
		ImGui::SliderInt("sizey", &myDesc.mSizeY, 1, g_iWinCY, "sizey = %d");
		IMGUI_TREE_END
	}

	IMGUI_TREE_BEGIN("Pivot")
	{
		ImGui::SliderFloat2("PivotXY", (float*)&myDesc.mPivot, 0.0f, 1.0f);
		IMGUI_TREE_END
	}
	mCurrentUIObject->Set_LoadUIDesc(myDesc);

	return S_OK;
}

HRESULT CImgui_UI::Edit_Texture()
{
	// 텍스처 선택 화면
	IMGUI_TREE_BEGIN("Textures")
	{
		static int selectedTex = -1;
		if (mListTextureKey == nullptr)
			mListTextureKey = (mCurrentUIObject->Get_TextureMap()->Get_MapKeyList());

		_uint cnt = 0;
		string selectTeture = "";
		for (auto iter = mListTextureKey->begin(); iter != mListTextureKey->end(); ++cnt, iter++)
		{
			if (ImGui::Selectable(iter->c_str(), selectedTex == cnt))
			{
				selectedTex = cnt;
				selectTeture = *iter;

			//	mUIObject->Get_TextureMap()->Set_TextureMap(selectTeture);

				TEXTUREDESC tex;
				strcpy_s(tex.mTextureKey_Diffuse, selectTeture.c_str());
				mCurrentUIObject->Set_LoadTexDesc(tex);

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
	Safe_Release(mCurrentUIObject);
}