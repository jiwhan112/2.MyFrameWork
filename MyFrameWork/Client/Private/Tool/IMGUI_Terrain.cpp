#include "stdafx.h"
#include "Tool/Imgui_Terrain.h"
#include "Tool/Imgui_DESC.h"

#include "Cell.h"
#include "GameObject/Client_Object.h"
#include "GameObject/Dungeon_Objects.h"

CImgui_Terrain::CImgui_Terrain(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_Terrain::NativeConstruct()
{
	mCurrent_TerrainObject = nullptr;

	mCameraClient = nullptr;
	meCreateTERRAIN_Layer = LAY_TERRAIN_DUNGEON;
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
			mCameraClient->Set_CameraMode(CCamera_Client::CAMERA_MODE_TERRAIN, CCamera_Client::CAMERA_MOVEPOS_STATE_END, SelectObject);
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
			CREATE_LOAD_DESC();

			ImGui::Checkbox("TerrainSetting", &mIsTerrainSetting);

			if (mIsTerrainSetting)
			{
				if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_TERRAIN)))
				{
					Combo_MapMode();
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
	_uint idx = GetSingle(CGameManager)->Get_CurrentLevel();

	//if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Create_Empty_WorldMap")))
	//{
	//	if (nullptr == GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects())
	//	{
	//		GetSingle(CGameManager)->Get_DaungonManager()->NativeConstruct_Level(LEVEL_TOOL);
	//	}

	//	// 툴에서의 월드 맵
	//	mWorldMap = (CGameObject_MyTerrain*)Create_Manager->CreateEmptyObject(GAMEOBJECT_MYTERRAIN);
	//	NULL_CHECK_BREAK(mWorldMap);
	//	Safe_AddRef(mWorldMap);
	//	mWorldMap->Set_MapType(CGameObject_MyTerrain::MAPTYPE_WORLD);
	//	mWorldMap->Init_SetupInit(true);
	//	FAILED_CHECK_NONERETURN(GetSingle(CGameInstance)->Push_Object(idx, TAGLAY(LAY_TERRAIN), mWorldMap));
	//}


	//if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Create_Empty_DungeonMap")))
	//{
	//	if (nullptr == GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects())
	//	{
	//		GetSingle(CGameManager)->Get_DaungonManager()->NativeConstruct_Level(LEVEL_TOOL);
	//	}

	//	// 툴에서의 월드 맵
	//	mDungeonMap = (CGameObject_MyTerrain*)Create_Manager->CreateEmptyObject(GAMEOBJECT_MYTERRAIN);
	//	NULL_CHECK_BREAK(mDungeonMap);
	//	Safe_AddRef(mDungeonMap);
	//	mDungeonMap->Set_MapType(CGameObject_MyTerrain::MAPTYPE_DUNGEON);
	//	mDungeonMap->Init_SetupInit(true);
	//	FAILED_CHECK_NONERETURN(GetSingle(CGameInstance)->Push_Object(idx, TAGLAY(LAY_TERRAIN), mDungeonMap));
	//}

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Create_WorldMap")))
	{
		if (nullptr == GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects())
		{
			GetSingle(CGameManager)->Get_DaungonManager()->NativeConstruct_Level(LEVEL_TOOL);
		}

		// 툴에서의 월드 맵
		mWorldMap = (CGameObject_MyTerrain*)Create_Manager->CreateEmptyObject(GAMEOBJECT_MYTERRAIN);
		NULL_CHECK_BREAK(mWorldMap);
		Safe_AddRef(mWorldMap);
		mWorldMap->Set_MapType(CGameObject_MyTerrain::MAPTYPE_WORLD);
		mWorldMap->Init_SetupInit();
		mWorldMap->Get_MODEL_WORLD_DESC_List(&mListWorldObjects_Desc);
		FAILED_CHECK_NONERETURN(GetSingle(CGameInstance)->Push_Object(idx, TAGLAY(LAY_TERRAIN_DUNGEON), mWorldMap));

	}

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Create_DungeonMap")))
	{
		if (nullptr == GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects())
		{
			GetSingle(CGameManager)->Get_DaungonManager()->NativeConstruct_Level(LEVEL_TOOL);
		}

		// 툴에서의 던전맵
		mDungeonMap = (CGameObject_MyTerrain*)Create_Manager->CreateEmptyObject(GAMEOBJECT_MYTERRAIN);
		NULL_CHECK_BREAK(mDungeonMap);
		Safe_AddRef(mDungeonMap);
		mDungeonMap->Set_MapType(CGameObject_MyTerrain::MAPTYPE_DUNGEON);
		mDungeonMap->Init_SetupInit();
		mWorldMap->Get_MODEL_WORLD_DESC_List(&mListWorldObjects_Desc);
		FAILED_CHECK_NONERETURN(GetSingle(CGameInstance)->Push_Object(idx, TAGLAY(LAY_TERRAIN_WORLD), mDungeonMap));

		// 타일 추가
		GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects()->Setup_Terrain_Daungeon(mDungeonMap);
	}
}

void CImgui_Terrain::CREATE_LOAD_DESC()
{
	// Terrain Desc 정보로 불러오기
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	//IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_TERRAIN, "Create_Terrain"))
	//{
	//	mDescList = Create_Manager->Get_MapObject_Type(OBJECT_TYPE_TERRAIN);

	//	static int selectObjectIndex = -1;
	//	_uint cnt = 0;
	//	static wstring selectObjectStr = L"";
	//	for (auto& protoString : *mDescList)
	//	{
	//		if (ImGui::Selectable(protoString.c_str(), selectObjectIndex == cnt))
	//		{
	//			selectObjectIndex = cnt;
	//			selectObjectStr = CHelperClass::Convert_str2wstr(protoString);
	//		}
	//		cnt++;
	//	}

	//	// 선택 원형 오브젝트 클론
	//	if (ImGui::Button(STR_IMGUI_IDSTR(IMGUI_TITLE_TERRAIN, "Create_Terrain_DESC")))
	//	{
	//		_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();

	//		// 빈오브젝트에 생성하는 방법

	//	}

	//	IMGUI_TREE_END
	//}
	
	// 이전 코드
	//IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_TERRAIN, "Proto_Parent"))
	//{
	//	if (mProtoMapObjectList == nullptr)
	//		mProtoMapObjectList = Create_Manager->Get_MapObject_Type(OBJECT_TYPE_TERRAIN);

	//	static int selectObjectIndex = -1;
	//	_uint cnt = 0;
	//	static wstring selectObjectStr = L"";
	//	for (auto& protoString : *mProtoMapObjectList)
	//	{
	//		if (ImGui::Selectable(protoString.c_str(), selectObjectIndex == cnt))
	//		{
	//			selectObjectIndex = cnt;
	//			selectObjectStr = CHelperClass::Convert_str2wstr(protoString);
	//		}
	//		cnt++;
	//	}

	//	// 선택 원형 오브젝트 클론
	//	if (ImGui::Button(STR_IMGUI_IDSTR(IMGUI_TITLE_TERRAIN, "Create_Clone_Map")))
	//	{
	//		_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
	//		Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectStr, TAGLAY(LAY_TERRAIN));
	//	}

	//	IMGUI_TREE_END
	//}
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
		// Terrain Object 설정 
		if (meToolMode == CImgui_Terrain::E_TOOLMODE_TERRAIN_MAP)
		{
			FAILED_CHECK_NONERETURN(Edit_TERRAIN());
		}

		// 맵 만들기
		else if (meToolMode == CImgui_Terrain::E_TOOLMODE_TERRAIN_OBJ)
		{
			FAILED_CHECK_NONERETURN(Edit_OBJECTS());
		}

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

			//if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "TerrainSize")))
			//{
			//	static _uint eSize = 0;
			//	static const _uint eMaxSize = E_TERRAINSIZE::TERRAINSIZE_END;
			//	eSize++;
			//	eSize %= eMaxSize;
			//	mCurrent_TerrainObject->Set_TerrainMode(E_TERRAINSIZE(eSize));
			//}

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

				if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON)& DIS_Down)
				{
					CNavigation* navi = mCurrent_TerrainObject->Get_ComNavimesh();
					navi->Pick_ChangeCellOption(_float4x4::Identity, CCell::CELLTYPE_STOP);
				}

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

		// CamDIr
		static _float3 DirData = _float3(-0.7, -1.5f, 1);
		CCamera_Client* camera = (CCamera_Client*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_CAMERA));
		if (camera)
		{
			ImGui::DragFloat3("DirCamera", (float*)&DirData, 0.01f, -2, 2);

			camera->Set_CamerDIr(DirData);

			//if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_INGAME, "ButtOnDir")))
			//{
			//	camera->Set_CamerDIr(DirData);
			//}

		}

		// ShaderValue
		static _float Range = 1.0f;
		ImGui::DragFloat("BrushRange", &Range, 0.1f, 1.f, 25.f);
		mCurrent_TerrainObject->mRadius = Range;

		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Create_FiterTexture")))
		{
			mCurrent_TerrainObject->SaveCurrentFiterMap();
		}

		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "UpdateY")))
		{
			// 높이 테스트
			mCurrent_TerrainObject->Get_TerrainBuffer()->UpdateY(0.5f);
		}

		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "SetHeightMap")))
		{
			// 높이 버퍼 수정
			// 네비 메시 새로 씌우기
			mCurrent_TerrainObject->Set_HeightNewMap();
		}
		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "NEW_FITER")))
		{
			// 높이 버퍼 수정
			// 네비 메시 새로 씌우기
			mCurrent_TerrainObject->UpdateFiterTextue();
		}
		
		if (ImGui::CollapsingHeader(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "FiterMake")))
		{
			// 타일 브러시 일단 랜덤으로 사용
			static ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft;

			const char* items[] = { "SourceNONE","SourceA", "SourceR", "SourceG", "SourceB" };
			static int item_current_idx = 0; // Here we store our selection data as an index.
			const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)

			if (ImGui::BeginCombo("FIterCombo", combo_preview_value, flags))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
					{
						item_current_idx = n;
					}
				}
				ImGui::EndCombo();
			}
			static float fVal = 1.0f;
			ImGui::DragFloat("Fiter_Value", &fVal, 0.01f, 0.01f, 1.f);

			if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON)& DIS_Press)
			{
				_float3 WorldPos = GetSingle(CGameManager)->Get_PickPos();
				mCurrent_TerrainObject->UpdateFiterTextue_TOOL((CGameObject_MyTerrain::E_SOURCETYPE)(item_current_idx), WorldPos, Range, fVal);

			}
			if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "SAVE_FITER")))
			{
				// 높이 버퍼 수정
				// 네비 메시 새로 씌우기
				mCurrent_TerrainObject->SaveCurrentFiterMap();
			}
			
		}
	
		
	}

	return S_OK;
}

HRESULT CImgui_Terrain::Edit_OBJECTS()
{
	// 만들어놓은 3D 깡통 오브젝트 배치 
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
	CGameObject_3D_Static2* PickObject = nullptr;
	_uint idx = GetSingle(CGameManager)->Get_CurrentLevel();

	// 타일 모드랑 오브젝트 모드 나눠서 구현

	// Object 정보
	if (ImGui::BeginListBox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Objects")))
	{
		if (mProtoStaticObjectList == nullptr)
			mProtoStaticObjectList = Create_Manager->Get_MapObject_Type(OBJECT_TYPE_3D_STATIC);
		if (mProtoStaticObjectList != nullptr)
		{
			// 객체 선택
			static int selectObjectIndex = -1;
			_uint cnt = 0;
			static wstring selectObjectWStr = L"";
			static string selectObjectStr = "";
			for (auto& protoString : *mProtoStaticObjectList)
			{
				cnt++;
				if (ImGui::Selectable(protoString.c_str(), selectObjectIndex == cnt))
				{
					selectObjectIndex = cnt;
					selectObjectStr = protoString;
					selectObjectWStr = CHelperClass::Convert_str2wstr(protoString);

					// 선택 객체 생성
					if (selectObjectIndex != -1)
					{
						CGameObject_Base* pickobj = Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectWStr, TAGLAY(meCreateOBJ_Layer));
						static_cast<CGameObject_3D_Static*>(pickobj)->Set_PickObject();
						Set_PickObject(pickobj);

					}

				}
			}
			ImGui::EndListBox();

			if (ImGui::Button(STR_IMGUI_IDSTR(IMGUI_TITLE_TERRAIN, "DeletePick")))
			{
				if (mCurrent_PickObject)
				{
					mCurrent_PickObject->Set_Dead();
					Safe_Release(mCurrent_PickObject);
					mCurrent_PickObject = nullptr;
				}
			}

			// 객체 배치
			static bool bBatchAble = false;
			ImGui::Checkbox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "AbleToggle"), &bBatchAble);

			// 현재 픽 오브젝트
			if (bBatchAble && mCurrent_PickObject)
			{
				if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE::MBS_RBUTTON)& DIS_Down)
				{
					// 맵에 배치 시킨다.
					CGameObject_Base* cloneObject = Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectWStr, TAGLAY(meCreateOBJ_Layer));
					cloneObject->Get_ComTransform()->Set_WorldMat(mCurrent_PickObject->Get_ComTransform()->GetWorldFloat4x4());
					mListWorldObjects.push_front(cloneObject);
					Safe_AddRef(cloneObject);

					MODEL_WORLD_DESC worldDesc;
					strcpy_s(worldDesc.mProtoName, selectObjectStr.c_str());
					worldDesc.mWorldMat = cloneObject->Get_ComTransform()->GetWorldFloat4x4();
					mListWorldObjects_Desc.push_front(worldDesc);

				}
			}

			// 오브젝트 수정 인터페이스
			if (mCurrent_PickObject)
			{
				CTransform* trans = mCurrent_PickObject->Get_ComTransform();
				_double timer = 0.03f;
				// 버튼 회전 
				if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_Q) & DIS_Press)
				{
					// 현재 픽된 오브젝트 회전 
					
					trans->Turn(_float3::Up, timer);
					
				}
				else if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_E) & DIS_Press)
				{
					trans->Turn(_float3::Down, timer);
				}

				// 크기 변환
				if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_Z) & DIS_Press)
				{
					// 현재 픽된 오브젝트 회전 
					trans->Scaling(_float3::One, timer);

				}
				else if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_C) & DIS_Press)
				{
					// 현재 픽된 오브젝트 회전 

					trans->Scaling(-_float3::One, timer);

				}
				// 위아래
				static _float offsetY = 0;
				if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_R) & DIS_Down)
				{
					offsetY += 0.1f;
					((CGameObject_3D_Static*)mCurrent_PickObject)->Set_OffsetY(offsetY);

				}
				else if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_F) & DIS_Down)
				{
					offsetY -= 0.1f;
					((CGameObject_3D_Static*)mCurrent_PickObject)->Set_OffsetY(offsetY);
				}
			}


			// 오브젝트 삭제
			if (ImGui::Button(STR_IMGUI_IDSTR(IMGUI_TITLE_TERRAIN, "Delete_MAP_Object")))
			{
				if (mListWorldObjects.empty() == false)
				{
					auto removeObj = mListWorldObjects.front();
					mListWorldObjects.pop_front();
					mListWorldObjects_Desc.pop_front();
					Safe_Release(removeObj);
					removeObj->Set_Dead();
				}
			}

			
			// 타일 깔기
			if (ImGui::Button(STR_IMGUI_IDSTR(IMGUI_TITLE_TERRAIN, "Map_SelectLoadButton")))
			{
				wstring keyname = CHelperClass::Convert_str2wstr(CImgui_DESC::mStrSelectName[E_DESC_DATA::DESC_DATA_TERRAIN]);

				TERRAIN_DESC* DungeonDesc = GetSingle(CGameManager)->Get_ObjectIOManager()->Find_TerrainDesc(keyname);
				GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects()->ResetTile_Tool(DungeonDesc);
			}

			if (ImGui::CollapsingHeader(STR_IMGUI_IDSTR(IMGUI_TITLE_TERRAIN, "Map_Saver")))
			{
			
				FAILED_CHECK(SAVER_MODE());
			}

		}
	}
	return S_OK;

}



HRESULT CImgui_Terrain::SAVER_MODE()
{
	// 저장
	CObjectIO* Object_IO_Manager = GetSingle(CGameManager)->Get_ObjectIOManager();

	// 저장
	IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_TERRAIN, "ObjectSaver"))
	{
		// 맵 저장

		// 오브젝트
		static char ObjectName[128] = "";
		ImGui::InputTextWithHint("savetext_3DObj", "enter Obj Name", ObjectName, IM_ARRAYSIZE(ObjectName));
		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Save")))
		{
			// dat 파일 경로에 저장
			string str = ObjectName;
			wstring wstr = CHelperClass::Convert_str2wstr(str);
			
			// 오브젝트랑 타일정보는 툴의 정보를 기반으로 새로 저장
			auto TileList =  GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects()->Get_TileList();
			list<_uint> tilelistindex;
			if (TileList != nullptr)
			{
				for (auto& tile : *TileList)
				{
					_uint tileindex = tile->Get_TileIndex();
					tilelistindex.push_front(tileindex);
				}
			}

			mCurrent_TerrainObject->SaveDESC_Objects(tilelistindex,mListWorldObjects_Desc);
			FAILED_CHECK(Object_IO_Manager->SaverObject(OBJECT_TYPE_TERRAIN, STR_FILEPATH_RESOURCE_DAT_L, wstr , mCurrent_TerrainObject));
		}
		IMGUI_TREE_END
	}
	return S_OK;
}

void CImgui_Terrain::Set_PickObject(CGameObject_Base* obj)
{
	if (mCurrent_PickObject != obj)
	{
		Safe_Release(mCurrent_PickObject);
		if (mCurrent_PickObject)
			mCurrent_PickObject->Set_Dead();
		mCurrent_PickObject = obj;
		Safe_AddRef(mCurrent_PickObject);
	}
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
	Safe_Release(mDungeonMap);
	Safe_Release(mWorldMap);


	Safe_Delete(mProtoStaticObjectList);
	
	for (auto& obj: mListWorldObjects)
	{
		Safe_Release(obj);
	}
	mListWorldObjects.clear();

}