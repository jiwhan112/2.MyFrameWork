#include "stdafx.h"
#include "Tool/Imgui_Model.h"
#include "GameObject/Client_Object.h"
#include "GameObject/ParticleManager.h"


#include "Model.h"
#include "AnimationClip.h"
#include "HierarchyNode.h"
#include "Animatior.h"

CImgui_Model::CImgui_Model(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_Model::NativeConstruct()
{
	mCurrent_ModelStaticObject = nullptr;

	mCameraClient = nullptr;
	meCreateLayer = LAY_OBJECT;
	meCreateLayer_Particle = LAY_EFFECT;

	return S_OK;
}

HRESULT CImgui_Model::Update(_double time)
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

		if (type == OBJECT_TYPE_3D_STATIC)
		{
			mCameraClient->Set_CameraMode(CCamera_Client::CAMERA_MODE_TARGET, CCamera_Client::CAMERA_MOVEPOS_STATE_END, SelectObject);
			mCurrent_ModelStaticObject = static_cast<CGameObject_3D_Static*>(SelectObject);
			meModelMode = CImgui_Model::TOOLMODE_MODEL_STATIC;
		}

		else if (type == OBJECT_TYPE_3D_DYNAMIC)
		{
			mCameraClient->Set_CameraMode(CCamera_Client::CAMERA_MODE_TARGET, CCamera_Client::CAMERA_MOVEPOS_STATE_END, SelectObject);
			mCurrent_ModelDynamicObject = static_cast<CGameObject_3D_Dynamic*>(SelectObject);
			meModelMode = CImgui_Model::TOOLMODE_MODEL_DYNAMIC;
		}
		else if (type == OBJECT_TYPE_3D_STATIC_PARENT)
		{
			mCameraClient->Set_CameraMode(CCamera_Client::CAMERA_MODE_TARGET, CCamera_Client::CAMERA_MOVEPOS_STATE_END, SelectObject);
			mCurrent_ModelStaticObject_Parent = static_cast<CGameObject_3D_Static2*>(SelectObject);
			meModelMode = CImgui_Model::TOOLMODE_MODEL_STATIC_PARENT;
		}
		else if (type == OBJECT_TYPE_3D_STATIC_TILES)
		{
			mCameraClient->Set_CameraMode(CCamera_Client::CAMERA_MODE_TARGET, CCamera_Client::CAMERA_MOVEPOS_STATE_END,SelectObject);
			mCurrent_Model_Tiles = static_cast<CGameObject_3D_Tile*>(SelectObject);
			meModelMode = CImgui_Model::TOOLMODE_MODEL_TILE;
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

		mCurrent_ModelStaticObject = nullptr;
		mCurrent_ModelDynamicObject = nullptr;
		mCurrent_ModelStaticObject_Parent = nullptr;
		mCurrent_Model_Tiles = nullptr;

		meModelMode = CImgui_Model::TOOLMODE_MODEL_END;
	}

	FAILED_CHECK(Render_UI());

	SelectObject = nullptr;

	return S_OK;
}

HRESULT CImgui_Model::Render_UI()
{
	if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_MAIN)))
	{
		ImGui::Checkbox("ParticleSetting", &mIsPartilceSetting);

		if (mIsPartilceSetting)
		{
			if (ImGui::Begin("Particle_Tool"))
			{

				// ???? ?????? ???? ???????? ????
				static ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft;

				const char* items[] = { "Particle2D","Particle3D","ParticleENd"};
				static int item_current_idx = 0; // Here we store our selection data as an index.
				const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)

				if (ImGui::BeginCombo("ParticleCombo", combo_preview_value, flags))
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

				if (item_current_idx == 0)
				{
					Edit_PARTICLE_2D();
				}
				else if (item_current_idx == 1)
				{
					Edit_PARTICLE_3D();
				}				
				ImGui::End();
			}
		}

		else
		{



			if (ImGui::CollapsingHeader(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "3D_MODEL")))
			{
				// ?? 3D ???????? ????
				RENDER_CREATEEMPTY();

				// ???? ????????
				RENDER_CREATE_PROTO();

				// ?? ?????? ???? ????
				if (meModelMode == CImgui_Model::TOOLMODE_MODEL_STATIC)
				{
					// ?????? ???? ????
					RENDER_STATIC_MODE();
				}

				if (meModelMode == CImgui_Model::TOOLMODE_MODEL_DYNAMIC)
				{
					RENDER_DYNAMIC_MODE();
				}
				if (meModelMode == CImgui_Model::TOOLMODE_MODEL_STATIC_PARENT)
				{
					RENDER_STATIC_PARENT_MODE();
				}

				ImGui::Checkbox("ModelSetting", &mIsModelSetting);

				if (mIsModelSetting)
				{
					if (ImGui::Begin(STR_IMGUITITLE(CImgui_Base::IMGUI_TITLE_FBX)))
					{
						FBX_SETTINGMODE();
						ImGui::End();
					}
				}

			}
		}
		ImGui::End();
	}

	return S_OK;
}

void CImgui_Model::FBX_SETTINGMODE()
{
	if (mCurrent_ModelDynamicObject ||
		mCurrent_ModelStaticObject || 
		mCurrent_ModelStaticObject_Parent)
	{
		if (mCurrent_ModelStaticObject)
			mCurrent_ModelStaticObject->Get_ComTransform()->Set_LoadTransDesc(1.f, 0.2f, 1.0f);

		if(mCurrent_ModelDynamicObject)
			mCurrent_ModelDynamicObject->Get_ComTransform()->Set_LoadTransDesc(1.f, 0.2f, 1.0f);


		if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_Q & DIS_Press))
		{
			if (mCurrent_ModelStaticObject)
				mCurrent_ModelStaticObject->Get_ComTransform()->Turn(_float3::Up, 0.3f);
			if (mCurrent_ModelDynamicObject)
				mCurrent_ModelDynamicObject->Get_ComTransform()->Turn(_float3::Up, 0.3f);
		}
		if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_E & DIS_Press))
		{
			if (mCurrent_ModelStaticObject)
				mCurrent_ModelStaticObject->Get_ComTransform()->Turn(_float3::Down, 0.3f);
			if (mCurrent_ModelDynamicObject)
				mCurrent_ModelDynamicObject->Get_ComTransform()->Turn(_float3::Down, 0.3f);
		}

		// ?????? FBX ???????? ????
		FAILED_CHECK_NONERETURN(Edit_FBX());
		FAILED_CHECK_NONERETURN(Edit_ANI());
		FAILED_CHECK_NONERETURN(Edit_SETTING());
	}
}

void CImgui_Model::RENDER_CREATE_PROTO()
{
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "Proto_Static"))
	{
		if (mProtoStaticModelList == nullptr)
			mProtoStaticModelList = Create_Manager->Get_MapObject_Type(OBJECT_TYPE_3D_STATIC);

		static int selectObjectIndex = -1;
		_uint cnt = 0;
		static wstring selectObjectStr = L"";
		for (auto& protoString : *mProtoStaticModelList)
		{
			cnt++;

			if (ImGui::Selectable(protoString.c_str(), selectObjectIndex == cnt))
			{
				selectObjectIndex = cnt;
				selectObjectStr = CHelperClass::Convert_str2wstr(protoString);
			}
		}

		// ???? ???? ???????? ????
		if (ImGui::Button(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "Create_Clone_Static")))
		{
			_uint idx = GetSingle(CGameManager)->Get_CurrentLevel();
			Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectStr, TAGLAY(LAY_OBJECT));
		}

		IMGUI_TREE_END
	}

	IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "Proto_Dynamic"))
	{
		if (mProtoDynamicModelList == nullptr)
			mProtoDynamicModelList = Create_Manager->Get_MapObject_Type(OBJECT_TYPE_3D_STATIC);

		static int selectObjectIndex = -1;
		_uint cnt = 0;
		static wstring selectObjectStr = L"";
		for (auto& protoString : *mProtoDynamicModelList)
		{
			cnt++;

			if (ImGui::Selectable(protoString.c_str(), selectObjectIndex == cnt))
			{
				selectObjectIndex = cnt;
				selectObjectStr = CHelperClass::Convert_str2wstr(protoString);
			}
		}

		// ???? ???? ???????? ????
		if (ImGui::Button(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "Create_Clone_Dynamic")))
		{
			_uint idx = GetSingle(CGameManager)->Get_CurrentLevel();
			Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectStr, TAGLAY(LAY_OBJECT));
		}

		IMGUI_TREE_END
	}

	IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "Proto_Parent"))
	{
		if (mProtoParentModelList == nullptr)
			mProtoParentModelList = Create_Manager->Get_MapObject_Type(OBJECT_TYPE_3D_STATIC_PARENT);

		static int selectObjectIndex = -1;
		_uint cnt = 0;
		static wstring selectObjectStr = L"";
		for (auto& protoString : *mProtoParentModelList)
		{
			cnt++;

			if (ImGui::Selectable(protoString.c_str(), selectObjectIndex == cnt))
			{
				selectObjectIndex = cnt;
				selectObjectStr = CHelperClass::Convert_str2wstr(protoString);
			}
		}

		// ???? ???? ???????? ????
		if (ImGui::Button(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "Create_Clone_Parent")))
		{
			_uint idx = GetSingle(CGameManager)->Get_CurrentLevel();
			Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectStr, TAGLAY(LAY_OBJECT));
		}

		IMGUI_TREE_END
	}

}

void CImgui_Model::RENDER_STATIC_MODE()
{
	FAILED_CHECK_NONERETURN(SAVER_MODE());
}

void CImgui_Model::RENDER_DYNAMIC_MODE()
{
	FAILED_CHECK_NONERETURN(SAVER_MODE());

	
}

void CImgui_Model::RENDER_STATIC_PARENT_MODE()
{
	FAILED_CHECK_NONERETURN(SAVER_MODE());
}


void CImgui_Model::RENDER_CREATEEMPTY()
{
	// ?? ???????? ????
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Create_Static")))
	{
		_uint idx = GetSingle(CGameManager)->Get_CurrentLevel();
		CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_STATIC);

		// ???? ???????? ???????? ????
		GetSingle(CGameInstance)->Push_Object(idx, TAGLAY(meCreateLayer), createobj);
	}

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Create_Dynamic")))
	{
		_uint idx = GetSingle(CGameManager)->Get_CurrentLevel();
		CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_DYNAMIC);

		// ???? ???????? ???????? ????
		GetSingle(CGameInstance)->Push_Object(idx, TAGLAY(meCreateLayer), createobj);
	}

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Create_Static2")))
	{
		_uint idx = GetSingle(CGameManager)->Get_CurrentLevel();
		CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_STATIC2);

		// ???? ???????? ???????? ????
		GetSingle(CGameInstance)->Push_Object(idx, TAGLAY(meCreateLayer), createobj);
	}

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Create_TILE")))
	{
		_uint idx = GetSingle(CGameManager)->Get_CurrentLevel();
		CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_TILE);
		// ???? ???????? ???????? ????
		GetSingle(CGameInstance)->Push_Object(idx, TAGLAY(meCreateLayer), createobj);

	}


}

HRESULT CImgui_Model::SAVER_MODE()
{
	// ????
	CObjectIO* Object_IO_Manager = GetSingle(CGameManager)->Get_ObjectIOManager();

	// ????
	IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "ObjectSaver"))
	{
		
			static char ObjectName[128] = "";
			ImGui::InputTextWithHint("savetext_3DObj", "enter Obj Name", ObjectName, IM_ARRAYSIZE(ObjectName));
			if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Save")))
			{
				// dat ???? ?????? ????
				string str = ObjectName;
				wstring wstr = CHelperClass::Convert_str2wstr(str);
				switch (meModelMode)
				{
				case CImgui_Model::TOOLMODE_MODEL_STATIC:
					FAILED_CHECK(Object_IO_Manager->SaverObject(OBJECT_TYPE_3D_STATIC, STR_FILEPATH_RESOURCE_DAT_L, wstr , mCurrent_ModelStaticObject));
					break;
				case CImgui_Model::TOOLMODE_MODEL_DYNAMIC:
					FAILED_CHECK(Object_IO_Manager->SaverObject(OBJECT_TYPE_3D_DYNAMIC, STR_FILEPATH_RESOURCE_DAT_L, wstr , mCurrent_ModelDynamicObject));
					break;
				case CImgui_Model::TOOLMODE_MODEL_STATIC_PARENT:
					FAILED_CHECK(Object_IO_Manager->SaverObject(OBJECT_TYPE_3D_STATIC_PARENT, STR_FILEPATH_RESOURCE_DAT_L, wstr , mCurrent_ModelStaticObject_Parent));
					break;
				}


			}
		IMGUI_TREE_END
	}
	return S_OK;
}

void CImgui_Model::INIT_FBXPathList()
{
	if (mFBX_Static_pathList == nullptr)
	{
		const list<MYFILEPATH*>* FBXPathList = GetSingle(CGameManager)->Get_PathList(CGameManager::PATHTYPE_FBX_STATIC);
		if (FBXPathList == nullptr)
			return;

		mFBX_Static_pathList = NEW list<string>();

		wstring wstr;
		for (auto& path : *FBXPathList)
		{
			wstr = path->FileName;
			string str = CHelperClass::Convert_Wstr2str(wstr);
			mFBX_Static_pathList->push_front(str);
		}
	}

	if (mFBX_Dynamic_pathList == nullptr)
	{
		const list<MYFILEPATH*>* FBXPathList = GetSingle(CGameManager)->Get_PathList(CGameManager::PATHTYPE_FBX_DYNAMIC);
		if (FBXPathList == nullptr)
			return;

		mFBX_Dynamic_pathList = NEW list<string>();

		wstring wstr;
		for (auto& path : *FBXPathList)
		{
			wstr = path->FileName;
			string str = CHelperClass::Convert_Wstr2str(wstr);
			mFBX_Dynamic_pathList->push_front(str);
		}
	}
}

HRESULT CImgui_Model::Edit_FBX()
{
	// FBX ?????? ???? ???? ??????
	// ???? ????????

	// ?????? ???? ????
	INIT_FBXPathList();

	static int selectedTex = -1;

	// ???? FBX ????
	if (meModelMode == CImgui_Model::TOOLMODE_MODEL_STATIC)
	{
		if (mFBX_Static_pathList->empty())
			return E_FAIL;

		if (ImGui::BeginListBox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "FbxFiles")))
		{

			_uint cnt = 0;
			string selectFBX = "";
			static ImGuiTextFilter filter;
			filter.Draw();

			for (auto iter = mFBX_Static_pathList->begin(); iter != mFBX_Static_pathList->end(); ++cnt, iter++)
			{
				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), selectedTex == cnt))
					{
						selectedTex = cnt;
						selectFBX = *iter;
						MODEL_STATIC_DESC fbx;

						strcpy_s(fbx.mModelName, selectFBX.c_str());
						mCurrent_ModelStaticObject->Set_LoadModelDESC(fbx);
					}
				}
			}

			ImGui::EndListBox();
		}

		
		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Add_Pass")))
		{
			static int shaderpass = 0;
			mCurrent_ModelStaticObject->Set_ShdaerPass(shaderpass);
			shaderpass++;
			shaderpass %= 2;
		}


	
	}

	// ???? FBX ????
	if (meModelMode == CImgui_Model::TOOLMODE_MODEL_DYNAMIC)
	{
		if (mFBX_Dynamic_pathList->empty())
			return E_FAIL;

		// FBX ????
		if (ImGui::BeginListBox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "FbxFiles")))
		{
			_uint cnt = 0;
			string selectFBX = "";
			static ImGuiTextFilter filter;
			filter.Draw();

			for (auto iter = mFBX_Dynamic_pathList->begin(); iter != mFBX_Dynamic_pathList->end(); ++cnt, iter++)
			{
				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), selectedTex == cnt))
					{
						selectedTex = cnt;
						selectFBX = *iter;
						MODEL_DYNAMIC_DESC fbx;

						strcpy_s(fbx.mModelName, selectFBX.c_str());
						mCurrent_ModelDynamicObject->Set_LoadModelDynamicDESC(fbx);
					}
				}
			}
			ImGui::EndListBox();
		}
	}

	// ???? ???? FBX ????
	if (meModelMode == CImgui_Model::TOOLMODE_MODEL_STATIC_PARENT)
	{
		if (mFBX_Static_pathList->empty())
			return E_FAIL;

		static _int selectIndex = -1;
		_uint cnt = 0;
		if (ImGui::BeginListBox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "ChildObjects")))
		{			
			auto childlist = mCurrent_ModelStaticObject_Parent->Get_ChildList();

			for (auto& obj : *childlist)
			{
				if (obj == nullptr)
					continue;

				char buf[128] = "";
				string name = obj->Get_ModelDESC().mModelName;
				sprintf_s(buf, "%d:Model:%s", cnt, name.c_str());

				if (ImGui::Selectable(buf, selectIndex == cnt))
				{
					selectIndex = cnt;
				}
				cnt++;
			}

			ImGui::EndListBox();
		}

		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "CreateChild")))
		{
			CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
			CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_STATIC);
			mCurrent_ModelStaticObject_Parent->Add_StaticObejct((CGameObject_3D_Static*)createobj);
		}

		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "DeleteChild")))
		{
			mCurrent_ModelStaticObject_Parent->Delete_StaticObejct(selectIndex);
			selectIndex = -1;

		}

		// ???? ?? ???? ???? ????
		if (ImGui::BeginListBox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Edit")))
		{
			IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "FbxFiles_Child"))
			{
				if (0 <= selectIndex)
				{
					_uint cnt = 0;
					string selectFBX = "";
					static ImGuiTextFilter filter;
					filter.Draw();

					for (auto iter = mFBX_Static_pathList->begin(); iter != mFBX_Static_pathList->end(); ++cnt, iter++)
					{
						if (filter.PassFilter(iter->c_str()))
						{
							if (ImGui::Selectable(iter->c_str(), selectedTex == cnt))
							{
								selectedTex = cnt;
								selectFBX = *iter;
								MODEL_STATIC_DESC fbx;
								strcpy_s(fbx.mModelName, selectFBX.c_str());
								mCurrent_ModelStaticObject_Parent->Get_ChildOfIndex(selectIndex)->Set_LoadModelDESC(fbx);
							}
						}
					}
				}
				IMGUI_TREE_END
			}

			IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "Parent_Transform"))
			{
				CTransform* parentTrans = mCurrent_ModelStaticObject_Parent->Get_ComTransform();
				_float4x4	Worldmat = parentTrans->GetWorldMatrix();
				_float3		position = Worldmat.Translation();

				// ????
				ImGui::DragFloat3("Position", (float*)&position, 0.1f, -50, 50);
				_float4x4 TransMat = _float4x4::CreateTranslation(position);

				parentTrans->Set_WorldMat(TransMat);
				IMGUI_TREE_END
			}

			// ???? ?? ???? ???? ????
			IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "Child_Transform"))
			{
				if (0 <= selectIndex)
				{
					CGameObject_3D_Static* SelectObjectChild = mCurrent_ModelStaticObject_Parent->Get_ChildOfIndex(selectIndex);
					CTransform* trnascom = SelectObjectChild->Get_ComTransform();


					_float4x4 WorldLocalMat = trnascom->GetWorldLocalFloat4x4();
					_float3		scale;
					_float3		position;
					_quaterion	rotaion;
					// _float3		worldRot;

					WorldLocalMat.Decompose(scale,rotaion,position);

					// ????
					ImGui::DragFloat3("Position",(float*)&position,0.1f,-50,50);
					_float4x4 TransMat = _float4x4::CreateTranslation(position);


					// ????
					//ImGui::DragFloat3("Rotaion", (float*)&worldRot, 0.01f, 0, 1);
					//_float4x4 RotMat = _float4x4::CreateFromQuaternion(rotaion);

					// ????
					ImGui::DragFloat3("Scale", (float*)&scale, 0.01f, 0.1f, 100);
					_float4x4 scaleMat = _float4x4::CreateScale(scale);
					

					_float4x4 newWorldMat = scaleMat* TransMat;
					trnascom->Set_WorldMat_Local(newWorldMat);
					
				}
				IMGUI_TREE_END
			}

			// ???? ?? ???? ???????? ????
			IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "Collider"))
			{
				CGameObject_3D_Static* SelectObjectChild = mCurrent_ModelStaticObject_Parent->Get_ChildOfIndex(selectIndex);
				COLLIDER_DESC desc = SelectObjectChild->Get_ColliderDESC();

				ImGui::DragFloat3("ColliderSize", (float*)&desc.mSize, 0.01f, 0.1f, 100);

				if (ImGui::Button("AABB"))
				{
					desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_AABB;
				}
				if (ImGui::Button("OBB"))
				{
					desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_OBB;
				}

				if (ImGui::Button("SPHERE"))
				{
					desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
				}

				SelectObjectChild->Set_LoadColliderDESC(desc);
				IMGUI_TREE_END
			}
			ImGui::EndListBox();
		}		

	

	}
	return S_OK;
}

HRESULT CImgui_Model::Edit_ANI()
{
	if (meModelMode != CImgui_Model::TOOLMODE_MODEL_DYNAMIC)
		return S_FALSE;

	// #TODO: ?????????? ??????
	// ?????????? ?????? ???? ?????????? ????
	// ?????????? ?????? ?????? ?????????? ???? ??????

	CModel* modelCom = mCurrent_ModelDynamicObject->Get_ComModel();
	CAnimatior* animator = modelCom->Get_Animaitor();

	// Selects
	static string Select_Bonestr = "";
	static string Select_AniStr = "";

	if (ImGui::BeginListBox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Bones")))
	{
		auto pVecBones = modelCom->Get_VecHierarchy();
		if (pVecBones == nullptr)
			return S_FALSE;

		// ?????? ?????? ?? ???? ????
		static int selectBone = -1;
		_uint cnt = 0;
		static ImGuiTextFilter filter;
		filter.Draw();

		static _float4x4 mat = _float4x4::Identity;
		
		for (auto iter = pVecBones->begin(); iter != pVecBones->end(); ++cnt, iter++)
		{
			char bufName[256] = "";
			strcpy_s(bufName, (*iter)->Get_Name());

			if (filter.PassFilter(bufName))
			{
				if (ImGui::Selectable(bufName, selectBone == cnt))
				{
					selectBone = cnt;
					Select_Bonestr = bufName;
				//	mat = (*iter)->Get_CombinedTransformationMatrix();
				}
			}
		}

		if (selectBone != -1)
		{
			mat = (*pVecBones)[selectBone]->Get_CombinedTransformationMatrix();
			// mCurrent_ModelDynamicObject->Setup_Colluder2(mat,0.1f);
		}


		ImGui::EndListBox();
	}


	if (ImGui::BeginListBox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "AniFiles")))
	{

		auto pVecAni = animator->Get_VecAnimaions();

		if (pVecAni == nullptr)
			return S_FALSE;


		// ?????? ?????? ???? ?????????? ????
		static int selectAni = -1;

		_uint cnt = 0;
		
		static ImGuiTextFilter filter;
		filter.Draw();

		for (auto iter = pVecAni->begin(); iter != pVecAni->end(); ++cnt, iter++)
		{
			char bufName[256] = "";
			strcpy_s(bufName, (*iter)->Get_Name());

			if (filter.PassFilter(bufName))
			{
				if (ImGui::Selectable(bufName, selectAni == cnt))
				{
					selectAni = cnt;
					Select_AniStr = bufName;
					animator->Set_AniString(Select_AniStr);

					// MODEL_DYNAMIC_DESC fbx;
					//	strcpy_s(fbx.mModelName, selectFBX.c_str());
					//mCurrent_ModelDynamicObject->Set_LoadModelDynamicDESC(fbx);
				}
			}
		}

		
		ImGui::EndListBox();


		_float currentTime = animator->Get_CurrentAnimaion()->Get_AniMationTime();
		ImGui::SliderFloat("ANIMATIONTIME",&currentTime, 0, animator->Get_CurrentAnimaion()->Get_MaxAnimaionTime());
	///	animator->Get_CurrentAnimaion()->Set_AniMationTime(currentTime);

		_float blendTime = (_float)animator->Get_BlendMaxTime();
		ImGui::DragFloat("BLENDTIMER", &blendTime, 0.01f, 0.01f, 10.f);
		animator->Set_BlendMaxTime(blendTime);


		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Walk")))
		{
			animator->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_WALK);
		}
		ImGui::SameLine();
		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "run")))
		{
			animator->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_RUN);

		}
		ImGui::SameLine();

		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "idle")))
		{
			animator->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE);

		}

	}

	



	static _int select = -1;

	if (ImGui::BeginListBox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "SocketObjectTest")))
	{		
		// ???? ????
		if (!mSocketList.empty())
		{
			// ???? ???????? ???? 
			_int cnt = 0;

			static ImGuiTextFilter filter;
			filter.Draw();

			for (auto iter = mSocketList.begin(); iter != mSocketList.end(); ++cnt, iter++)
			{
				char bufName[256] = "";
				string str = (*iter)->Get_SocketDesc().mSocketName;
				sprintf_s(bufName, 256, "Sock_%s_%d", str.c_str(),cnt);

				if (filter.PassFilter(bufName))
				{
					if (ImGui::Selectable(bufName, select == cnt))
					{
						select = cnt;
						mSelectSocketObject = *iter;
					}
				}
			}
		}
		ImGui::EndListBox();
	}

	if (mSelectSocketObject)
	{
		_float3 pos = mSelectSocketObject->Get_WorldPostition();
		ImGui::DragFloat3("SocketWorld:", (float*)&pos, 0.1f, -100, 100);
		mSelectSocketObject->Set_Position(pos);
	}


	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "SocketButton")))
	{
		CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

		CGameObject_3D_Socket::SOCKETDESC desc;
		// ??????
		desc.mSocketName = Select_Bonestr.c_str();
		if (strlen(desc.mSocketName) < 1)
		{
			ImGui::EndListBox();
			return S_FALSE;
		}

		// ???? ????
		desc.mTargetModel = mCurrent_ModelDynamicObject->Get_ComModel();
		desc.mTransform = mCurrent_ModelDynamicObject->Get_ComTransform();

		// ???? ???????? ??????
		CGameObject_3D_Socket* Socket = (CGameObject_3D_Socket*)Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_SOCKET);

		Socket->Set_LoadSocketDESC("crea_SnotPickaxe.fbx", desc);
		mSocketList.push_front(Socket);

		_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
		Create_Manager->PushObject((CGameObject_Base**)&Socket, levelindex, TAGLAY(meCreateLayer));

	}

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "ClearSocket")))
	{

		for (auto& socket: mSocketList)
		{
			socket->Set_Dead();

		}
		mSocketList.clear();
	}

	return S_OK;
}

HRESULT CImgui_Model::Edit_SETTING()
{
	
	if (meModelMode == CImgui_Model::TOOLMODE_MODEL_STATIC)
	{
		// Collider
		IMGUI_TREE_BEGIN("Collider_Setting")
		{
			COLLIDER_DESC desc = mCurrent_ModelStaticObject->Get_ColliderDESC();

			ImGui::DragFloat3("ColliderSize", (float*)&desc.mSize, 0.01f, 0.1f, 100);

			if (ImGui::Button("AABB"))
			{
				desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_AABB;
			}
			if (ImGui::Button("OBB"))
			{
				desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_OBB;
			}

			if (ImGui::Button("SPHERE"))
			{
				desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
			}

			mCurrent_ModelStaticObject->Set_LoadColliderDESC(desc);
			IMGUI_TREE_END
		}

		// Quaterion
		IMGUI_TREE_BEGIN("Quaterion_Setting")
		{
			CTransform* Transform = mCurrent_ModelStaticObject->Get_ComTransform();

			static _float3 rotDegree = _float3();
			ImGui::DragFloat3("RotTest", (float*)&rotDegree, 0.1, -360, 360);

			_float3 Radian = _float3(XMConvertToRadians(rotDegree.x), XMConvertToRadians(rotDegree.y), XMConvertToRadians(rotDegree.z));

			ImGui::Text("Radian: %.2f,%.2f,%f.2", Radian.x, Radian.y, Radian.z);

			Transform->Set_Rotate(Radian);
			IMGUI_TREE_END
		}

		// QuaterionTest
		IMGUI_TREE_BEGIN("Target_RotSetting")
		{
			CTransform* Transform = mCurrent_ModelStaticObject->Get_ComTransform();

			static _float3 TargetRot_Degreee = _float3();
			ImGui::DragFloat3("TargetRot_Degreee", (float*)&TargetRot_Degreee, 0.1, -360, 360);

			static _float Time = 0;
			ImGui::DragFloat("Time", &Time, 0.1, 0, 1);

			_quaterion myqu = Transform->Get_DegreeFormQuater(_float3(0, 0, 0));
			_quaterion targetqu = Transform->Get_DegreeFormQuater(TargetRot_Degreee);
			
			_quaterion squ = _quaterion::Slerp(myqu, targetqu,Time);			
			_float4x4 rotmat = _float4x4::CreateFromQuaternion(squ);
			 Transform->Set_Rotate(rotmat);


			IMGUI_TREE_END
		}

		IMGUI_TREE_BEGIN("Target_RotSetting2")
		{
			CTransform* Transform = mCurrent_ModelStaticObject->Get_ComTransform();

			static _float3 TargetPos = _float3();
			ImGui::DragFloat3("TargetPosition", (float*)&TargetPos, 1, -360, 360);

			_float3 TargetDir = TargetPos - _float3();
			TargetDir.Normalize();

			TargetDir = _float3(XMConvertToDegrees(TargetDir.x),
				XMConvertToDegrees(TargetDir.y), XMConvertToDegrees(TargetDir.z));
		

			static _float Time = 0;
			ImGui::DragFloat("Time", &Time, 0.1, 0, 1);

			_quaterion myqu = Transform->Get_DegreeFormQuater(_float3(0, 0, 0));
			_quaterion targetqu = Transform->Get_DegreeFormQuater(TargetDir);

			_quaterion squ = _quaterion::Slerp(myqu, targetqu, Time);
			_float4x4 rotmat = _float4x4::CreateFromQuaternion(squ);
			Transform->Set_Rotate(rotmat);


			IMGUI_TREE_END

		}
	}

	return S_OK;
}

HRESULT CImgui_Model::Edit_PARTICLE_2D()
{
	CParticleManager* PartilceManager = GetSingle(CGameManager)->Get_PartilceManager();

	static string PartilceImageName_Diffuse= "";


	if (ImGui::Button("CreateParticle2D"))
	{
		PARTICLECREATEDESC	createDesc;
		createDesc.Count = 1;
		createDesc.MinTime = 3;
		createDesc.MaxTime = 5;
		createDesc.MinDistance = 5;
		createDesc.MaxDistance = 5;
		createDesc.MinDir = _float3(-1, -1, -1);
		createDesc.MaxDir = _float3(1, 1, 1);;

		// ????
		if (PartilceImageName_Diffuse.length() > 2)
			PartilceManager->Create_Partilce_Instance2D_Tool(PartilceImageName_Diffuse, createDesc);

	}



	return S_OK;
}

HRESULT CImgui_Model::Edit_PARTICLE_3D()
{
	CParticleManager* PartilceManager = GetSingle(CGameManager)->Get_PartilceManager();

	// 3D???? ????
	INIT_FBXPathList();
	if (mFBX_Static_pathList->empty())
		return E_FAIL;

	static int selectedTex = -1;

	// ???? FBX ????
	
	static string ParticleModelName = "";
	if (ImGui::BeginListBox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "ParticleFBX")))
	{

		_uint cnt = 0;
		static ImGuiTextFilter filter;
		filter.Draw();

		for (auto iter = mFBX_Static_pathList->begin(); iter != mFBX_Static_pathList->end(); ++cnt, iter++)
		{
			if (filter.PassFilter(iter->c_str()))
			{
				if (ImGui::Selectable(iter->c_str(), selectedTex == cnt))
				{
					selectedTex = cnt;
					ParticleModelName = *iter;
				}
			}
		}
		ImGui::EndListBox();
	}


	if(ImGui::Button("CreateParticle3D"))
	{
		PARTICLECREATEDESC	createDesc;
		createDesc.Count = 1;
		createDesc.MinTime = 3;
		createDesc.MaxTime = 5;
		createDesc.MinDistance = 5;
		createDesc.MaxDistance = 5;
		createDesc.MinDir = _float3(-1, -1, -1);
		createDesc.MaxDir = _float3(1, 1, 1);;

		// ????
		if (ParticleModelName.length() > 2)
			PartilceManager->Create_Partilce_3D_Tool(ParticleModelName,createDesc);

	}
	if (ImGui::Button("DelPartilce"))
	{
		// ???? 
		PartilceManager->Delete_Particle();
	}

	// ImGui::DragFloat3();
	static int CameraZ = 5;
	ImGui::DragInt("CameraZ",&CameraZ, 1, 3, 20);

	if (ImGui::Button("NewCamera"))
	{
		mCameraClient->Set_CameraMode_Tool(CCamera_Client::CAMERA_MODE_FROZEN);
		mCameraClient->Set_NewCameraPos(_float3(0, 0, -CameraZ), _float3(0, 0, 0));

	}

	return S_OK;
}

CImgui_Model * CImgui_Model::Create(ID3D11Device* deviec, ID3D11DeviceContext* context)
{
	CImgui_Model*	pInstance = NEW CImgui_Model(deviec, context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CImgui_Model");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_Model::Free()
{
	__super::Free();
	mSocketList.clear();

	Safe_Release(mCameraClient);

	Safe_Release(mCurrent_ModelStaticObject);
	Safe_Release(mCurrent_ModelDynamicObject);
	Safe_Release(mCurrent_ModelStaticObject_Parent);
	Safe_Release(mCurrent_Model_Tiles);

	Safe_Delete(mFBX_Static_pathList);
	Safe_Delete(mProtoStaticModelList);
	Safe_Delete(mFBX_Dynamic_pathList);
	Safe_Delete(mProtoDynamicModelList);

	Safe_Delete(mProtoParentModelList);
}