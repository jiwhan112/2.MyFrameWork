#include "stdafx.h"
#include "Tool/Imgui_Model.h"
#include "Camera_Client.h"
#include "GameObject/GameObject_3D_Static.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_3D_Static2.h"

#include "Model.h"
#include "Animation.h"

// #TODO: 모델 애니메이션 툴 제작
// 모델은 충돌체 씌울 수 있게
// 애니메이션은 각 애니메이션 재생할 수 있게.

CImgui_Model::CImgui_Model(ID3D11Device * device, ID3D11DeviceContext * context)
	:CImgui_Base(device, context)
{
}

HRESULT CImgui_Model::NativeConstruct()
{
	mCurrent_ModelStaticObject = nullptr;

	mCameraClient = nullptr;
	meCreateLayer = LAY_OBJECT;

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

		mCameraClient->Set_CameraMode(CCamera_Client::CAMERA_MODE_TARGET, SelectObject);

		if (type == OBJECT_TYPE_3D_STATIC)
		{
			mCurrent_ModelStaticObject = static_cast<CGameObject_3D_Static*>(SelectObject);
			meModelMode = CImgui_Model::TOOLMODE_MODEL_STATIC;
		}

		else if (type == OBJECT_TYPE_3D_DYNAMIC)
		{
			mCurrent_ModelDynamicObject = static_cast<CGameObject_3D_Dynamic*>(SelectObject);
			meModelMode = CImgui_Model::TOOLMODE_MODEL_DYNAMIC;
		}
		else if (type == OBJECT_TYPE_3D_STATIC_PARENT)
		{
			mCurrent_ModelStaticObject_Parent = static_cast<CGameObject_3D_Static2*>(SelectObject);
			meModelMode = CImgui_Model::TOOLMODE_MODEL_STATIC_PARENT;
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
		if (ImGui::CollapsingHeader(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "3D_MODEL")))
		{
			// 빈 3D 오브젝트 생성
			RENDER_CREATEEMPTY();

			// 파일 불러오기
			RENDER_CREATE_PROTO();

			// 각 모드에 맞는 세팅
			if (meModelMode == CImgui_Model::TOOLMODE_MODEL_STATIC)
			{
				// 타입에 따라 저장
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
		// 선택된 FBX 오브젝트 수정
		FAILED_CHECK_NONERETURN(Edit_FBX());
		FAILED_CHECK_NONERETURN(Edit_ANI());
		FAILED_CHECK_NONERETURN(Edit_COL());
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

		// 선택 원형 오브젝트 클론
		if (ImGui::Button(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "Create_Clone_Static")))
		{
			_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
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

		// 선택 원형 오브젝트 클론
		if (ImGui::Button(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "Create_Clone_Dynamic")))
		{
			_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
			Create_Manager->Create_ObjectClone_Prefab(idx, selectObjectStr, TAGLAY(LAY_OBJECT));
		}

		IMGUI_TREE_END
	}
}

void CImgui_Model::RENDER_STATIC_MODE()
{
	FAILED_CHECK_NONERETURN(SAVER_MODE());

	// 저장
	//CObjectIO* Object_IO_Manager = GetSingle(CGameManager)->Get_ObjectIOManager();

	// STATIC 저장
	//IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "StaticSaver"))
	//{
	//	if (mCurrent_ModelStaticObject != nullptr)
	//	{
	//		static char ObjectName[128] = "";
	//		ImGui::InputTextWithHint("savetext_static", "enter Obj Name", ObjectName, IM_ARRAYSIZE(ObjectName));
	//		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Save")))
	//		{
	//			// dat 파일 경로에 저장
	//			string str = ObjectName;
	//			wstring wstr = CHelperClass::Convert_str2wstr(str);
	//			Object_IO_Manager->SaverObject(OBJECT_TYPE_3D_STATIC, STR_FILEPATH_RESOURCE_DAT_L, wstr + L".dat", mCurrent_ModelStaticObject);
	//		}
	//	}
	//	IMGUI_TREE_END
	//}
}

void CImgui_Model::RENDER_DYNAMIC_MODE()
{
	FAILED_CHECK_NONERETURN(SAVER_MODE());

	//// 저장
	//CObjectIO* Object_IO_Manager = GetSingle(CGameManager)->Get_ObjectIOManager();

	//// STATIC 저장
	//IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "DynamicSaver"))
	//{
	//	if (mCurrent_ModelDynamicObject != nullptr)
	//	{
	//		static char ObjectName[128] = "";
	//		ImGui::InputTextWithHint("savetext_dynamic", "enter Obj Name", ObjectName, IM_ARRAYSIZE(ObjectName));
	//		if (ImGui::Button("SaveTest"))
	//		{
	//			// dat 파일 경로에 저장
	//			string str = ObjectName;
	//			wstring wstr = CHelperClass::Convert_str2wstr(str);
	//			Object_IO_Manager->SaverObject(OBJECT_TYPE_3D_DYNAMIC, STR_FILEPATH_RESOURCE_DAT_L, wstr + L".dat", mCurrent_ModelDynamicObject);
	//		}
	//	}
	//	IMGUI_TREE_END
	//}
}

void CImgui_Model::RENDER_STATIC_PARENT_MODE()
{
	FAILED_CHECK_NONERETURN(SAVER_MODE());
}


void CImgui_Model::RENDER_CREATEEMPTY()
{
	// 빈 오브젝트 클론
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Create_Static")))
	{
		_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
		CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_STATIC);

		// 이미 만들어진 오브젝트 추가
		GetSingle(CGameInstance)->Push_Object(levelindex, TAGLAY(meCreateLayer), createobj);
	}

	ImGui::SameLine();

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Create_Dynamic")))
	{
		_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
		CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_DYNAMIC);

		// 이미 만들어진 오브젝트 추가
		GetSingle(CGameInstance)->Push_Object(levelindex, TAGLAY(meCreateLayer), createobj);
	}

	ImGui::SameLine();

	if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Create_Static2")))
	{
		_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
		CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_STATIC2);

		// 이미 만들어진 오브젝트 추가
		GetSingle(CGameInstance)->Push_Object(levelindex, TAGLAY(meCreateLayer), createobj);
	}

}

HRESULT CImgui_Model::SAVER_MODE()
{
	// 저장
	CObjectIO* Object_IO_Manager = GetSingle(CGameManager)->Get_ObjectIOManager();

	// 저장
	IMGUI_TREE_BEGIN(STR_IMGUI_IDSTR(IMGUI_TITLE_FBX, "ObjectSaver"))
	{
		if (mCurrent_ModelStaticObject != nullptr)
		{
			static char ObjectName[128] = "";
			ImGui::InputTextWithHint("savetext_3DObj", "enter Obj Name", ObjectName, IM_ARRAYSIZE(ObjectName));
			if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Save")))
			{
				// dat 파일 경로에 저장
				string str = ObjectName;
				wstring wstr = CHelperClass::Convert_str2wstr(str);
				switch (meModelMode)
				{
				case CImgui_Model::TOOLMODE_MODEL_STATIC:
					FAILED_CHECK(Object_IO_Manager->SaverObject(OBJECT_TYPE_3D_STATIC, STR_FILEPATH_RESOURCE_DAT_L, wstr + L".dat", mCurrent_ModelStaticObject));
					break;
				case CImgui_Model::TOOLMODE_MODEL_DYNAMIC:
					FAILED_CHECK(Object_IO_Manager->SaverObject(OBJECT_TYPE_3D_DYNAMIC, STR_FILEPATH_RESOURCE_DAT_L, wstr + L".dat", mCurrent_ModelDynamicObject));
					break;
				case CImgui_Model::TOOLMODE_MODEL_STATIC_PARENT:
					FAILED_CHECK(Object_IO_Manager->SaverObject(OBJECT_TYPE_3D_STATIC_PARENT, STR_FILEPATH_RESOURCE_DAT_L, wstr + L".dat", mCurrent_ModelStaticObject_Parent));
					break;
				}

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
	// FBX 파일에 따라 모델 바꾸기
	// 저장 불러오기

	// 텍스처 선택 화면
	INIT_FBXPathList();

	static int selectedTex = -1;

	// 정적 FBX 처리
	if (meModelMode == CImgui_Model::TOOLMODE_MODEL_STATIC)
	{
		if (mFBX_Static_pathList->empty())
			return E_FAIL;

		IMGUI_TREE_BEGIN("FbxFiles")
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
			IMGUI_TREE_END
		}
	}

	// 동적 FBX 처리
	if (meModelMode == CImgui_Model::TOOLMODE_MODEL_DYNAMIC)
	{
		if (mFBX_Dynamic_pathList->empty())
			return E_FAIL;

		// FBX 파일
		IMGUI_TREE_BEGIN("FbxFiles")
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
			IMGUI_TREE_END
		}
	}

	// 부모 정적 FBX 처리
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
			_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
			CGameObject* createobj = Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_STATIC);
			mCurrent_ModelStaticObject_Parent->Add_StaticObejct((CGameObject_3D_Static*)createobj);
		}

		if (ImGui::Button(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "DeleteChild")))
		{
			mCurrent_ModelStaticObject_Parent->Delefe_StaticObejct(selectIndex);
			selectIndex = -1;

		}

		// 선택 된 객체 모델 수정
		if (ImGui::BeginListBox(STR_IMGUI_IDSTR(CImgui_Base::IMGUI_TITLE_FBX, "Edit")))
		{
			IMGUI_TREE_BEGIN("FbxFiles")
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
			// 선택 된 객체 위치 수정

			IMGUI_TREE_BEGIN("Transform")
			{
				if (0 <= selectIndex)
				{
					CGameObject_3D_Static* SelectObjectChild = mCurrent_ModelStaticObject_Parent->Get_ChildOfIndex(selectIndex);
					CTransform* trnascom = SelectObjectChild->Get_TransformCom();


					_float4x4 Worldmat = trnascom->GetWorldMatrix();
					_float3		scale;
					_float3		position;
					_quaterion	rotaion;
					// _float3		worldRot;

					Worldmat.Decompose(scale,rotaion,position);

					// 위치
					ImGui::DragFloat3("Position",(float*)&position,0.1f,-50,50);
					_float4x4 TransMat = _float4x4::CreateTranslation(position);


					// 회전
					//ImGui::DragFloat3("Rotaion", (float*)&worldRot, 0.01f, 0, 1);
					//_float4x4 RotMat = _float4x4::CreateFromQuaternion(rotaion);

					// 크기
					ImGui::DragFloat3("Scale", (float*)&scale, 0.01f, 0.1f, 100);
					_float4x4 scaleMat = _float4x4::CreateScale(scale);
					

					_float4x4 newWorldMat = scaleMat* TransMat;
					trnascom->Set_WorldMat(newWorldMat);
					
				}
				IMGUI_TREE_END
			}

			// 선택 된 객체 충돌박스 수정

			IMGUI_TREE_BEGIN("Collider")
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

	// 애니메이션 에디터
	// #TODO: 애니메이션 출력

	CModel* modelCom = mCurrent_ModelDynamicObject->Get_ComModel();
	auto pVecAni = modelCom->Get_VecAnimations();
	if (pVecAni == nullptr)
		return S_FALSE;

	static int selectedINT = -1;

	IMGUI_TREE_BEGIN("AniFiles")
	{
		// 선택시 파일에 있는 애니메이션 출력
		_uint cnt = 0;
		string SelectStr = "";
		static ImGuiTextFilter filter;
		filter.Draw();

		for (auto iter = pVecAni->begin(); iter != pVecAni->end(); ++cnt, iter++)
		{
			char AniName[256] = "";
			strcpy_s(AniName, (*iter)->Get_Name());

			if (filter.PassFilter(AniName))
			{
				if (ImGui::Selectable(AniName, selectedINT == cnt))
				{
					selectedINT = cnt;
					SelectStr = AniName;
					mCurrent_ModelDynamicObject->Get_ComModel()->Set_AniString(SelectStr);

					// MODEL_DYNAMIC_DESC fbx;
					//	strcpy_s(fbx.mModelName, selectFBX.c_str());
					//mCurrent_ModelDynamicObject->Set_LoadModelDynamicDESC(fbx);
				}
			}
		}

		IMGUI_TREE_END
	}

	return S_OK;
}

HRESULT CImgui_Model::Edit_COL()
{
	if (meModelMode == CImgui_Model::TOOLMODE_MODEL_STATIC)
	{
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
	Safe_Release(mCameraClient);
	Safe_Delete(mFBX_Static_pathList);
	Safe_Delete(mProtoStaticModelList);
	Safe_Delete(mFBX_Dynamic_pathList);
	Safe_Delete(mProtoDynamicModelList);
}