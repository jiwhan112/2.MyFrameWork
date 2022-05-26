#include "stdafx.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "Animatior.h"

CGameObject_3D_Dynamic::CGameObject_3D_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC;
}

CGameObject_3D_Dynamic::CGameObject_3D_Dynamic(const CGameObject_3D_Dynamic& rhs)
	: CGameObject_Base(rhs)
	, mModelDesc(rhs.mModelDesc)

{
}

HRESULT CGameObject_3D_Dynamic::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	// 데이터 디폴트 세팅
	if (strlen(mModelDesc.mModelName) < 2)
	{
		string str("crea_Snot_a.fbx");
		strcpy_s(mModelDesc.mModelName, str.c_str());
	}
	
	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	mComTransform->Scaled(_float3(0.5f,0.5f,0.5f));

	mComModel->SetUp_AnimIndex(0);

	// #TESTCODE 충돌체 
	COLLIDER_DESC desc[3];
	desc[0].meColliderType = CCollider::E_COLLIDER_TYPE::COL_AABB;
	desc[1].meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc[2].meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc[1].mOffset = _float3(2, 0, 0);
	desc[1].mSize = _float3(0.5f, 0.5f, 0.5f);;
	desc[2].mOffset = _float3(-2, 0, 0);
	desc[2].mSize = _float3(1, 1, 1);;
	Add_ColliderDesc(desc,3);
	Update_Collider();

	mIsMove = false;

	mComModel->Get_Animaitor()->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE);

	return S_OK;
}

_int CGameObject_3D_Dynamic::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	if (mComNaviMesh == nullptr)
	{
		CGameObject_MyTerrain* terrain = (CGameObject_MyTerrain*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN));
		if (terrain != nullptr)
		{
			mComNaviMesh = (CNavigation*)terrain->Get_ComNavimesh()->Clone(nullptr);
			mComNaviMesh->Set_NaviObjType(CNavigation::NAVI_OBJTYPE_PLAYER);
		}
	}

	if (mComListCollider != nullptr)
	{
		for (auto& col : *mComListCollider)
		{
			col->Update_Transform(mComTransform->GetWorldFloat4x4());
			GetSingle(CGameManager)->Add_ColliderObject(CColliderManager::E_COLLIDEROBJ_TYPE::COLLIDEROBJ_DYNAMIC, this);
		}

	}

	if (mIsMove)
	{
		mComModel->SetUp_AnimIndex(29);

		// 경로 리스트에 있는 셀을 뺴서 이동
		// #Tag 네비메시 움직임나중에 수정 
		if (mIsMoveCell)
		{
			// 셀 이동
			mGoalPosition = mMoveCell->Get_CenterPoint();
			// Test
			_float3 newLook = _float3(mGoalPosition.x, mStartPosition.y, mGoalPosition.z);
			mComTransform->LookAt(newLook);
			mTimer += TimeDelta;
			mCurrentPosition = GetSingle(CGameInstance)->Easing3(TYPE_Linear, mStartPosition, mGoalPosition, mTimer, mTimeMax);
			if (mTimer > mTimeMax)
				mIsMoveCell = false;
			if (_float3::Distance(mCurrentPosition, mGoalPosition) < 1.0f)
				mIsMoveCell = false;
		}

		else
		{
			// 셀 설정
			if (mCurrentPathList.empty())
			{
				mIsMove = false;
				mIsMoveCell = false;
				mTimeMax = 0.3f;
			}
			else
			{
				mMoveCell = mCurrentPathList.front();
				mCurrentPathList.pop_front();
				mIsMoveCell = true;
				mStartPosition = Get_WorldPostition();
				mTimer = 0;
				mComNaviMesh->Move_OnNavigation(_float4(0, 0, 0, 0));
			}
			
			
		}
	}
	return UPDATENONE;
}

_int CGameObject_3D_Dynamic::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	CGameObject_MyTerrain* terrain = (CGameObject_MyTerrain*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN));
	if (terrain != nullptr)
	{	
		
		FAILED_CHECK(Set_Terrain_HeightY(terrain));		

		if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON)& DIS_Down)
		{
			// #TODO 네비메시 길찾기 적용 
			_float3 worldPickPos = GetSingle(CGameManager)->Get_PickPos();
			_uint StartIndex = mComNaviMesh->Get_CurrentCellIndex();
			_uint GoalIndex = StartIndex;
			
			if (mIsMove == false && mCurrentPathList.empty())
			{
				if (mComNaviMesh->Get_PickPosForIndex(worldPickPos, &GoalIndex))
				{
					mCurrentPathList = mComNaviMesh->AstartPathFind(StartIndex, GoalIndex);
					mTimeMax = 0.3f;
					mIsMove = true;
				}
			}

			// For.Debug
			//if (mComNaviMesh->Get_PickPosForIndex(worldPickPos, &GoalIndex))
			//{

			//	mCurrentPathList = mComNaviMesh->AstartPathFind(StartIndex, GoalIndex);
			//	mIsMove = true;
			//	for (auto& cell : mCurrentPathList)
			//	{
			//		cell->Set_TileType(CCell::CELLTYPE_DEBUG);
			//	}
			//}
		}
	}
	

	mComModel->Update_CombinedTransformationMatrices(TimeDelta);
	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	return UPDATENONE;
}

HRESULT CGameObject_3D_Dynamic::Render()
{
	FAILED_CHECK(Set_ConstantTable_World());
	FAILED_CHECK(Set_ConstantTable_Light());

	if (mComModel != nullptr)
	{
		_uint iNumMaterials = mComModel->Get_NumMaterials();

		// 재질 개수만큼 루프
		for (int i = 0; i < iNumMaterials; ++i)
		{
			// 1. 텍스처 설정
			mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);
			// 2. 랜더링
			// 여기서 뼈를 넘긴다.
			FAILED_CHECK(mComModel->Render(mComShader, mCurrentShaderPass, i, STR_BONES));
		}
	}

#ifdef _DEBUG
	if (mComListCollider != nullptr)
	{
		for (auto obj : *mComListCollider)
		{
			obj->Render();
		}
	}

	CGameObject* obj = GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN));
	if (obj != nullptr&& mComNaviMesh !=nullptr) 
	{
		CTransform* terraintrans = obj->Get_ComTransform();
		mComNaviMesh->Render(terraintrans);
	}
	
#endif // _DEBUG

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Set_LoadModelDynamicDESC(const MODEL_DYNAMIC_DESC & desc)
{
	memcpy(&mModelDesc, &desc, sizeof(MODEL_DYNAMIC_DESC));

	// 해당 모델 컴포넌트로 변경
	if (mComModel != nullptr)
	{
		Safe_Release(mComModel);
		mComModel = nullptr;
	}

	string strModel = mModelDesc.mModelName;
	wstring ModelName = CHelperClass::Convert_str2wstr(strModel);

	FAILED_CHECK(__super::Release_Component(TEXT("Com_Model")));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, ModelName.c_str(), TEXT("Com_Model"), (CComponent**)&mComModel));

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Add_ColliderDesc(COLLIDER_DESC desc)
{
	mListColliderDesc.push_back(desc);
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Add_ColliderDesc(COLLIDER_DESC * desc, int size)
{
	for (int i =0; i<size;++i)
	{
		Add_ColliderDesc(desc[i]);
	}

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::CollisionFunc(_float3 PickPosition, _float dist, _uint ColliderIndex)
{
	// 잘된다.


	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXANIMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));

	// 모델 타입에 따라 정적모델 동적모델 처리
	if (mComModel == nullptr)
	{
		// 동적 모델은 자동으로 동적으로 컴포넌트가 적용된다.
		string strModel = mModelDesc.mModelName;
		wstring ModelName = CHelperClass::Convert_str2wstr(strModel);
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, ModelName.c_str(), TEXT("Com_Model"), (CComponent**)&mComModel));
	}

	if (mComNaviMesh == nullptr)
	{
		// 현재 Terrain의 네비 메시를 가져와야한다.
		CGameObject_MyTerrain* terrain =  (CGameObject_MyTerrain*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN));
		if (terrain != nullptr)
		{
			mComNaviMesh = (CNavigation*)terrain->Get_ComNavimesh()->Clone(nullptr);
			mComNaviMesh->Set_NaviObjType(CNavigation::NAVI_OBJTYPE_PLAYER);
		}
	}
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Update_Collider()
{
	// Desc 정보로 리스트를 새로 정의한다.
	if (mComListCollider == nullptr)
		mComListCollider = NEW list<CCollider*>;
	else
	{
		for (auto& col : *mComListCollider)
		{
			Safe_Release(col);
		}
		mComListCollider->clear();
	}

	// Desc 정보로 복수의 충돌체 정의
	for (auto& desc : mListColliderDesc)
	{
		CCollider* col = nullptr;

		switch (desc.meColliderType)
		{
		case CCollider::COL_AABB:
			col = (CCollider*)Create_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_AABB));
			break;
		case CCollider::COL_OBB:
			col = (CCollider*)Create_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_OBB));
			break;
		case CCollider::COL_SPHERE:
			col = (CCollider*)Create_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_SPHERE));
			break;
		default:
			break;
		}

		if (col == nullptr)
			return E_FAIL;

		col->Set_Offset(desc.mOffset);
		col->Set_Scale(desc.mSize);
		col->Update_Transform(mComTransform->GetWorldFloat4x4());
		mComListCollider->push_back(static_cast<CCollider*>(col));
	}

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Set_Terrain_HeightY(CGameObject_MyTerrain* terrain)
{
	if (terrain == nullptr)
		return E_FAIL;

	mCurrentPosition.y = terrain->Get_HeightY(mCurrentPosition);
	mComTransform->Set_State(CTransform::E_STATE::STATE_POSITION, mCurrentPosition.ToVec4(1));
	return S_OK;
}

CGameObject_3D_Dynamic * CGameObject_3D_Dynamic::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_3D_Dynamic*	pInstance = NEW CGameObject_3D_Dynamic(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_3D_Dynamic* CGameObject_3D_Dynamic::Clone(void* pArg)
{
	CGameObject_3D_Dynamic*	pInstance = NEW CGameObject_3D_Dynamic(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D_Dynamic::Free()
{
	__super::Free();

	Safe_Release(mComModel);
	
	if (mComListCollider != nullptr)
	{
		for (auto& col : *mComListCollider)
		{
			Safe_Release(col);
		}
		mComListCollider->clear();
		Safe_Delete(mComListCollider);
	}

	Safe_Release(mComNaviMesh);
}