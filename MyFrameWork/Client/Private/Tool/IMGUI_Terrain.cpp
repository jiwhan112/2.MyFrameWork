#include "stdafx.h"
#include "Tool/Imgui_Terrain.h"

#include "Camera_Client.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "GameObject/GameObject_3D_Static.h"
#include "GameObject/GameObject_3D_Dynamic.h"

// #TODO: ���� ����
// �� ���ؽ� ����
// �� ���� �ҷ�����
// �� ������Ʈ ��ġ
// ť�� �� ���

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

		//	mCameraClient->Set_CameraMode(CCamera_Client::CAMERA_MODE_TARGET, SelectObject);

		if (type == OBJECT_TYPE_TERRAIN)
		{
			mCurrent_TerrainObject = static_cast<CGameObject_MyTerrain*>(SelectObject);
			meToolMode = CImgui_Terrain::E_TOOLMODE_TERRAIN_MAP;
		}
		else
			SelectObject = nullptr;
	}
	else
	{
		if (mCameraClient)
		{
			mCameraClient->ReleaseTarget();
		}

		SelectObject = nullptr;
		mCurrent_TerrainObject = nullptr;
		meToolMode = CImgui_Terrain::E_TOOLMODE_TERRAIN_END;
	}

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
			// �� 3D ������Ʈ ����
			RENDER_CREATEEMPTY();

			// ���� �ҷ�����
			RENDER_CREATE_PROTO();

			// �� ��忡 �´� ����
			if (meToolMode == CImgui_Terrain::E_TOOLMODE_TERRAIN_MAP)
			{
				// Ÿ�Կ� ���� ����
				RENDER_MAP_MODE();
			}

			if (meToolMode == CImgui_Terrain::E_TOOLMODE_TERRAIN_OBJ)
			{
				RENDER_MAP_OBJ_MODE();
			}

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

void CImgui_Terrain::RENDER_CREATEEMPTY()
{
	// �� ������Ʈ Ŭ��
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Create_Terrain")))
	{
		_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
		CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_MYTERRAIN);

		// �̹� ������� ������Ʈ �߰�
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
		// ���õ� TERRAIN ������Ʈ ����
		FAILED_CHECK_NONERETURN(Edit_TERRAIN());
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

		// �׺�޽�

		IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "Navi"))
		{
			if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_TERRAIN, "AutoNavi")))
			{
				// �׺�޽� ������ �ڵ� ����
				CNavigation* navi = mCurrent_TerrainObject->Get_ComNavimesh();
				CVIBuffer_Terrain* terrainbuffer = mCurrent_TerrainObject->Get_TerrainBuffer();

				// 1. ������ �����´�.
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
					// ����

					// ���ؽ� �״�� �׺� �ѱ�
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

				// 3���� ������ �ѱ�
				// for (auto iter = Newfloat3List.begin(); iter != Newfloat3List.end();)
				// {
				// 	for (int i =0; i<3 ; ++i)
				// 	{
				// 		vPoints[i] = *iter;
				// 		iter++;
				// 		if (iter == Newfloat3List.end())
				// 			break;
				// 	}
				//
				// 	if (iter == Newfloat3List.end())
				// 		break;
				// 	NewFloat3ArrayList.push_back(vPoints);
				// }

				navi->SetUp_NewNaviMesh(NewFloat3ArrayList);
				for (auto& p : NewFloat3ArrayList)
				{
					Safe_Delete_Array(p);
				}
				NewFloat3ArrayList.clear();
			}
			IMGUI_TREE_END
		}
	}

	return S_OK;
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
}