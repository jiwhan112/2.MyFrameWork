#include "stdafx.h"
#include "Tool/Imgui_DESC.h"
#include "GameObject/Client_Object.h"

CImgui_DESC::CImgui_DESC(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_DESC::NativeConstruct()
{
	meModelMode = CImgui_DESC::TOOLMODE_DESC_NONE;
	FAILED_CHECK(INIT_DESCPathList());

	return S_OK;
}

HRESULT CImgui_DESC::Update(_double time)
{
//	CGameObject_Base* SelectObject = (CGameObject_Base*)GetSingle(CGameManager)->Get_ImGuiManager()->Get_SelectObject();	
	FAILED_CHECK(Render_UI());
	return S_OK;
}

HRESULT CImgui_DESC::Render_UI()
{
	if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_MAIN)))
	{
		if (ImGui::CollapsingHeader(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_DESC, "DESC")))
		{
			ImGui::Checkbox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_DESC, "DescSetting"), &mIsDesc);
			if (mIsDesc)
			{
				if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_DESC)))
				{
					DESC_SETTINGMODE();
					ImGui::End();
				}
			}
		}
		ImGui::End();
	}

	return S_OK;
}

void CImgui_DESC::DESC_SETTINGMODE()
{
	// 선택된 FBX 오브젝트 수정
	FAILED_CHECK_NONERETURN(Edit_DESC());
}

void CImgui_DESC::Reset_PathData()
{
	if (mListFilePath != nullptr)
	{
		Safe_Delete(mListFilePath);
	}
	const list<MYFILEPATH*>* NewPathList = GetSingle(CGameManager)->Get_PathList(CGameManager::PATHTYPE_DESC);
	if (NewPathList == nullptr)
		return;

	mListFilePath = new list<MYFILEPATH *>;
	for (auto& path : *NewPathList)
	{
		MYFILEPATH* pathdata = path;
		mListFilePath->push_back(pathdata);
	}
	for (int i =0; i<E_DESC_DATA::DESC_DATA_END;++i)
	{
		if (mListDescFileNameSTR[i].empty() == false)
		{
			mListDescFileNameSTR[i].clear();
		}

		wstring exestr = STR_DATADESC_EXE((E_DESC_DATA)i);
		if(exestr.empty())
			continue;
		exestr = exestr.substr(1, exestr.size());

		for (auto& file : *mListFilePath)
		{
			wstring wfilename = file->FileName;
			wstring wExeName = file->Extension;
			if (exestr == wExeName)
			{
				string filename = CHelperClass::Convert_Wstr2str(wfilename);
				mListDescFileNameSTR[i].push_front(filename);
			}			
		}
	}	
}

HRESULT CImgui_DESC::INIT_DESCPathList()
{
	// 경로 설정 가져오기
	Reset_PathData();	
	return S_OK;
}

HRESULT CImgui_DESC::Edit_DESC()
{
	// DESC 정보에 따라 출력
	if(ImGui::CollapsingHeader(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_DESC, "DESC")))
	{

		for (int i = 0; i < E_DESC_DATA::DESC_DATA_END; ++i)
		{
			wstring exestr = Get_EXEDescName((E_DESC_DATA)i);
			if(exestr.empty())
				continue;

			string strexestr = CHelperClass::Convert_Wstr2str(exestr);
			if (ImGui::BeginListBox(strexestr.c_str()))
			{
				_uint cnt = 0;
				
				//	string selectFBX = "";
				static ImGuiTextFilter filter;
				filter.Draw();

				for (auto iter = mListDescFileNameSTR[i].begin(); iter != mListDescFileNameSTR[i].end(); iter++)
				{
					if (filter.PassFilter(iter->c_str()))
					{
						if (ImGui::Selectable(iter->c_str(), mSelectIndex[i] == cnt))
						{
							mSelectIndex[i] = cnt;
						}
					}
					cnt++;
				}
				ImGui::EndListBox();
			}
		}
	}
	return S_OK;
}

wstring CImgui_DESC::Get_EXEDescName(E_DESC_DATA descid)
{
	wstring exestr = STR_DATADESC_EXE(descid);
	if (exestr.empty())
		return exestr;
	exestr = exestr.substr(1, exestr.size());
	return exestr;
}


CImgui_DESC * CImgui_DESC::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_DESC*	pInstance = NEW CImgui_DESC(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_DESC");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_DESC::Free()
{
	__super::Free();	
	Safe_Delete(mListFilePath);

}