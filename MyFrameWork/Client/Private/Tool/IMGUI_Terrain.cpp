#include "stdafx.h"
#include "Tool/Imgui_Terrain.h"
#include "Cell.h"

#include "Camera_Client.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "GameObject/GameObject_3D_Static.h"
#include "GameObject/GameObject_3D_Static2.h"
#include "GameObject/GameObject_3D_Dynamic.h"

// #TODO: 맵툴 제작
// 맵 버텍스 수정
// 맵 저장 불러오기
// 맵 오브젝트 배치
// 큐브 셀 깔기

CImgui_Terrain::CImgui_Terrain(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_Terrain::NativeConstruct()
{
	mCurrent_TerrainObject = nullptr;

	mCameraClient = nullptr;
	meCreateTERRAIN_Layer = LAY_TERRAIN;
	meCreateOBJ_Layer = LAY_OBJECT;

	return S_OK;
}

HRESULT CImgui_Terrain::Update(_double time)
{
	if (mCameraClient == nullptr)
	{
		mCameraClient = static_cast<CCamera_Client*>(GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_CAMERA)));
		Safe_AddRef(mCameraClient);
	}

	CGameObject_Base* SelectObject = (CGameObject_Base*)GetSingle(CGameManager)->Get_ImGuiManager()->Get_SelectObject();

	if (SelectObject != nullptr)
	{
		E_OBJECT_TYPE type = SelectObject->Get_ObjectTypeID_Client();
		if (type == OBJECT_TYPE_TERRAIN)
		{
			mCurrent_TerrainObject = static_cast<CGameObject_MyTerrain*>(SelectObject);
			mCameraClient->Set_CameraMode(CCamera_Client::CAMERA_MODE_TARGET, SelectObject);
		}
	}
	else
	{
		if (mCameraClient)
		{
			mCameraClient->ReleaseTarget();
		}

		SelectObject = nullptr;
		mCurrent_TerrainObject = nullptr;
		Safe_Release(mCurrent_PickObject);

		meToolMode = CImgui_Terrain::E_TOOLMODE_TERRAIN_END;
	}

	Tick_PickObject(time);
	FAILED_CHECK(Render_UI());
	SelectObject = nullptr;

	return S_OK;
}

HRESULT CImgui_Terrain::Render_UI()
{
	if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_MAIN)))
	{
		if (ImGui::CollapsingHeader(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Terrain")))
		{
			// 빈 3D 오브젝트 생성
			RENDER_CREATEEMPTY();

			// 파일 불러오기
			RENDER_CREATE_PROTO();
			
			Combo_MapMode();

			//// Terrain 수정
			//if (meToolMode == CImgui_Terrain::E_TOOLMODE_TERRAIN_MAP)
			//{
			//	RENDER_MAP_MODE();
			//}
			//// Terrain에 오브젝트 넣기
			//else if (meToolMode == CImgui_Terrain::E_TOOLMODE_TERRAIN_OBJ)
			//{
			//	RENDER_MAP_OBJ_MODE();
			//}

			ImGui::Checkbox("TerrainSetting", &mIsTerrainSetting);

			if (mIsTerrainSetting)
			{
				if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_TERRAIN)))
				{
					WINDOW_TERRAIN();
					ImGui::End();
				}
			}
		}
		ImGui::End();
	}

	return S_OK;
}

void CImgui_Terrain::Combo_MapMode()
{
	const char* items[] = { "Map", "ObjectMap" };
	static int item_current = 0;

	ImGui::Combo(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "MapMode"), &item_current, items, IM_ARRAYSIZE(items));
	switch (item_current)
	{
	case 0:
		meToolMode = CImgui_Terrain::E_TOOLMODE_TERRAIN_MAP;
		break;
	case 1:
		meToolMode = CImgui_Terrain::E_TOOLMODE_TERRAIN_OBJ;
		break;
	default:
		break;
	}

}

void CImgui_Terrain::RENDER_CREATEEMPTY()
{
	// 빈 오브젝트 클론
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Create_Terrain")))
	{
		_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
		CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_MYTERRAIN);

		// 이미 만들어진 오브젝트 추가
		GetSingle(CGameInstance)->Push_Object(levelindex, TAGLAY(meCreateTERRAIN_Layer), createobj);
	}
}

void CImgui_Terrain::RENDER_CREATE_PROTO()
{
}

void CImgui_Terrain::RENDER_MAP_MODE()
{
}

void CImgui_Terrain::RENDER_MAP_OBJ_MODE()
{
}

void CImgui_Terrain::WINDOW_TERRAIN()
{
	if (mCurrent_TerrainObject)
	{
		// 선택된 TERRAIN 오브젝트 수정
		FAILED_CHECK_NONERETURN(Edit_TERRAIN());

		if (meToolMode == CImgui_Terrain::E_TOOLMODE_TERRAIN_OBJ)
			FAILED_CHECK_NONERETURN(Edit_OBJECTS());
	

	}
}

void CImgui_Terrain::Tick_PickObject(_double time)
{
	if (mCurrent_PickObject)
	{
		mCurrent_PickObject->Tick(time);
		mCurrent_PickObject->Render();
	}
}



HRESULT CImgui_Terrain::Edit_TERRAIN()
{
	if (meToolMode == CImgui_Terrain::E_TOOLMODE_TERRAIN_MAP)
	{
		IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Size"))
		{
			TERRAIN_DESC terrainDesc = mCurrent_TerrainObject->Get_TerrainDESC();

			ImGui::DragInt(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "TexSize"),
				(int*)&terrainDesc.mTextureMultiSize,
				1.0f, 10, 200, "%d");

			mCurrent_TerrainObject->Set_LoadTerrainDESC(terrainDesc);

			if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "TerrainSize")))
			{
				static _uint eSize = 0;
				static const _uint eMaxSize = E_TERRAINSIZE::TERRAINSIZE_END;
				eSize++;
				eSize %= eMaxSize;
				mCurrent_TerrainObject->Set_TerrainMode(E_TERRAINSIZE(eSize));
			}

			IMGUI_TREE_END
		}

		// 네비메시

		IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Navi"))
		{
			if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "AutoNavi")))
			{
				// 네비메시 데이터 자동 세팅
				CNavigation* navi = mCurrent_TerrainObject->Get_ComNavimesh();
				CVIBuffer_Terrain* terrainbuffer = mCurrent_TerrainObject->Get_TerrainBuffer();

				// 1. 정점을 가져온다.
				_float3* vertexes = terrainbuffer->Get_VerrtexPosition();
				_uint vertexSize = terrainbuffer->Get_VerrtexSize();
				_uint numX = terrainbuffer->Get_XZ()[0];
				_uint numZ = terrainbuffer->Get_XZ()[1];

				//	list<_float3> Newfloat3List;

				/*	for (_uint i = 0; i< vertexSize;++i)
					{
						_float3 position = vertexes[i];
						Newfloat3List.push_back(position);
					}*/
					// 랜덤

					// 버텍스 그대로 네비에 넘김
				list<_float3*> NewFloat3ArrayList;
				_float3 vPoints[3];

				for (_uint z = 0; z < numZ - 1; z++)
				{
					for (_uint x = 0; x < numX - 1; x++)
					{
						_uint		iIndex = z * numX + x;

						_uint		iIndices[4] = {
							iIndex + numX,
							iIndex + numX + 1,
							iIndex + 1,
							iIndex
						};

						vPoints[0] = vertexes[iIndices[0]];
						vPoints[1] = vertexes[iIndices[1]];
						vPoints[2] = vertexes[iIndices[2]];

						_float3* newPoint1 = NEW _float3[3];
						newPoint1[0] = vPoints[0];
						newPoint1[1] = vPoints[1];
						newPoint1[2] = vPoints[2];
						NewFloat3ArrayList.push_back(newPoint1);

						vPoints[0] = vertexes[iIndices[0]];
						vPoints[1] = vertexes[iIndices[2]];
						vPoints[2] = vertexes[iIndices[3]];

						_float3* newPoint2 = NEW _float3[3];
						newPoint2[0] = vPoints[0];
						newPoint2[1] = vPoints[1];
						newPoint2[2] = vPoints[2];
						NewFloat3ArrayList.push_back(newPoint2);
					}
				}

				navi->SetUp_NewNaviMesh(NewFloat3ArrayList);
				for (auto& p : NewFloat3ArrayList)
				{
					Safe_Delete_Array(p);
				}
				NewFloat3ArrayList.clear();
			}

			ImGui::Text("SaveNaviData");
			{
				CNavigation* navi = mCurrent_TerrainObject->Get_ComNavimesh();
				static char InputText[128] = "";

				ImGui::InputTextWithHint("savetext_Navi", "enter Navi Name", InputText, IM_ARRAYSIZE(InputText));
				if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "NaviData")))
				{
					// dat 파일 경로에 저장
					string str = InputText;
					wstring wstr = CHelperClass::Convert_str2wstr(str);
					wstring savewstr = STR_FILEPATH_RESOURCE_DAT_L;
					savewstr += L"\\" + wstr + L".dat";
					navi->Save_NaviMeshData(savewstr);
				}
				
				ImGui::Text("ReMoveData");
				if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "ReMove")))
				{
					navi->Remove_NaviMeshData();
				}
			}

			ImGui::Text("NoTile");
			{
				// 피킹시 해당 타일 옵션 변경
				CNavigation* navi = mCurrent_TerrainObject->Get_ComNavimesh();

				navi->Pick_ChangeCellOption(_float4x4::Identity, CCell::CELLTYPE_STOP);

				//static char InputText[128] = "";
				//ImGui::InputTextWithHint("savetext_Navi", "enter Navi Name", InputText, IM_ARRAYSIZE(InputText));
				//if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "NaviData")))
				//{
				//	// dat 파일 경로에 저장
				//	string str = InputText;
				//	wstring wstr = CHelperClass::Convert_str2wstr(str);
				//	wstring savewstr = STR_FILEPATH_RESOURCE_DAT_L;
				//	savewstr += L"\\" + wstr + L".dat";
				//	navi->Save_NaviMeshData(savewstr);
				//}

				//ImGui::Text("ReMoveData");
				//if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "ReMove")))
				//{
				//	navi->Remove_NaviMeshData();
				//}

			}
			


			
			IMGUI_TREE_END
		}
	}

	return S_OK;
}

HRESULT CImgui_Terrain::Edit_OBJECTS()
{
	// 만들어놓은 3D 깡통 오브젝트 배치 
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
	CGameObject_3D_Static2* PickObject = nullptr;


	if (ImGui::BeginListBox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Objects")))
	{
		auto ProtoParentModelList = Create_Manager->Get_MapObject_Type(OBJECT_TYPE_3D_STATIC_PARENT);
		if (ProtoParentModelList != nullptr)
		{

			static int selectObjectIndex = -1;
			_uint cnt = 0;
			static wstring selectObjectStr = L"";
			for (auto& protoString : *ProtoParentModelList)
			{
				cnt++;

				if (ImGui::Selectable(protoString.c_str(), selectObjectIndex == cnt))
				{
					selectObjectIndex = cnt;
					selectObjectStr = CHelperClass::Convert_str2wstr(protoString);

					
				}

				if (ImGui::Button(STR_IMGUI_IDSTR(IMGUI_TITLE_TERRAIN, "Click_Create")))
				{
					_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
					CGameObject_Base* base = Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectStr, TAGLAY(meCreateOBJ_Layer));
					if (base != nullptr)
						Create_PickObject(base);
				}
			}

			// 피킹된 위치에 따라가게 둠


			Safe_Delete(ProtoParentModelList);
		}


		

	

		ImGui::EndListBox();
	}


	return S_OK;
}

void CImgui_Terrain::Create_PickObject(CGameObject_Base* obj)
{
	Safe_Release(mCurrent_PickObject);
	mCurrent_PickObject = obj;
}


CImgui_Terrain * CImgui_Terrain::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_Terrain*	pInstance = NEW CImgui_Terrain(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CImgui_Terrain::Free()
{
	__super::Free();
	Safe_Release(mCameraClient);
	Safe_Release(mCurrent_TerrainObject);
	Safe_Release(mCurrent_PickObject);

}