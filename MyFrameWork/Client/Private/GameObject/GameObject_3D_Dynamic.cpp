#include "stdafx.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "AI/AI_Action.h"

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

	// 유닛별 초기화
	Init_Unit();
	
	// test
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_DUNGEON);
//	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_WORLD);
	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());

	return S_OK;
}

_int CGameObject_3D_Dynamic::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	// 충돌
	if (mComListCollider != nullptr)
	{
		for (auto& col : *mComListCollider)
		{
			col->Update_Transform(mComTransform->GetWorldFloat4x4());
			GetSingle(CGameManager)->Add_ColliderObject(CColliderManager::E_COLLIDEROBJ_TYPE::COLLIDEROBJ_DYNAMIC, this);
		}
	}


	mComBehavior->Tick(TimeDelta);
	return UPDATENONE;
}

_int CGameObject_3D_Dynamic::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	if (mCurrentMap == nullptr)
		return UPDATEERROR;

	//if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON)& DIS_Down)
	//{

	//}

	mComBehavior->LateTick(TimeDelta);
	mComModel->Update_CombinedTransformationMatrices(TimeDelta);

	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());
	if (GetSingle(CGameInstance)->IsIn_WorldSpace(Get_WorldPostition(), 2.f))
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

	if (mCurrentMap)
	{
		CTransform* terraintrans = mCurrentMap->Get_ComTransform();
		mCurrentNavi->Render(terraintrans);
	}
	
#endif // _DEBUG

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Init_Unit()
{
	// 상속받아서 사용

	mComTransform->Scaled(_float3(0.5f, 0.5f, 0.5f));

	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_AABB;
	desc.mSize = _float3(0.5f, 0.5f, 0.5f);
	Add_ColliderDesc(&desc, 1);
	Update_Collider();

	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE));

	// AI 세팅
	Create_Sequnce();
	
	return S_OK;
}

HRESULT  CGameObject_3D_Dynamic::Set_MapSetting(E_MAPTYPE type)
{
	if (type == CGameObject_3D_Dynamic::MAPTYPE_END)
		return E_FAIL;

	meCurrentMap = type;
	mCurrentMap = mTerrain_Maps[type];
	mCurrentNavi = mComNavi[type];

	// 위치도 정해두기

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

HRESULT CGameObject_3D_Dynamic::FindPathForCurrentNavi(_float3 GoalPosition)
{
	// 현재 네비메시로 경로 탐색
	if (mCurrentNavi == nullptr)
		return E_FAIL;

	if (mCurrentPathList.empty() == false)
		mCurrentPathList.clear();

	_uint StartIndex = mCurrentNavi->Get_CurrentCellIndex();
	_uint GoalIndex = StartIndex;

	// 해당 위치의 네비메시 셀 인덱스 반환
	if (mCurrentNavi->Get_PickPosForIndex(GoalPosition, &GoalIndex))
	{
		// 경로 탐색
		mCurrentPathList = mCurrentNavi->AstartPathFind(StartIndex, GoalIndex);
		return S_OK;
	}

	return S_FALSE;
}

_bool CGameObject_3D_Dynamic::Get_PathList_Frontpop(_float3 * NextPosition)
{
	if (mCurrentPathList.empty())
		return false;

	*NextPosition = mCurrentPathList.front()->Get_CenterPoint();
	mCurrentPathList.pop_front();

	return true;
}


HRESULT CGameObject_3D_Dynamic::Create_Sequnce()
{
	// #TEST 딜레이 루틴 2개 만들기
	// A루틴 : 0.5초 딜레이 2번
	// B루틴 : 0.3초 딜레이 10번


	CNode_Seqeunce* Seq_DealyA = CNode_Seqeunce::Create();

	// 클론 만들기
	CAction_DEALY* dealy5 = CAction_DEALY::Create("Dealy0.5", this, 0.5f);
	CAction_DEALY* dealyidle = CAction_DEALY::Create("dealyidle", this, 0.0);
	CAction_DEALY* dealydig = CAction_DEALY::Create("dealydig", this, 0.0);
	dealyidle->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);
	dealydig->Set_Animation(CAnimatior::E_COMMON_ANINAME_DIG);

	// CAction_MOVE* MoveRun = CAction_MOVE::Create("run", this, _float3(0, 0, 0), 0.2f);
	// MoveRun->Set_AniType(CAction_MOVE::MOVE_RUN_ANI);
	CAction_MOVE* MoveWalk = CAction_MOVE::Create("walk", this, _float3(0, 0, 0), 0.6f);
	MoveWalk->Set_AniType(CAction_MOVE::MOVE_WALK_ANI);

//	Seq_DealyA->PushBack_LeafNode(dealyidle->Clone());
	Seq_DealyA->PushBack_LeafNode(dealydig->Clone());

	// 이동
//	Seq_DealyA->PushBack_LeafNode(MoveRun->Clone());
	Seq_DealyA->PushBack_LeafNode(MoveWalk->Clone());


	//for (int i = 0; i < 2; ++i)
	//{
	//	Seq_DealyA->PushBack_LeafNode(dealy5->Clone());
	//}

	//Seq_DealyA->PushBack_LeafNode(MoveWalk->Clone());

	mComBehavior->Add_Seqeunce("IDLE1",Seq_DealyA);
	mComBehavior->Select_Sequnce("IDLE1");

	Safe_Release(dealy5);
	//Safe_Release(MoveRun);
	Safe_Release(MoveWalk);
	Safe_Release(dealyidle);
	Safe_Release(dealydig);
	
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
	if (mComBehavior == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_BEHAVIORTREE), TEXT("Com_Behavior"), (CComponent**)&mComBehavior));
	}

	if (mTerrain_Maps[MAPTYPE_DUNGEON] == nullptr || mTerrain_Maps[MAPTYPE_WORLD] == nullptr)
	{
		// 현재 Terrain의 네비 메시 복사.
		mTerrain_Maps[MAPTYPE_DUNGEON] = GetSingle(CGameManager)->Get_LevelObject_DUNGEONMAP();
		mTerrain_Maps[MAPTYPE_WORLD] = GetSingle(CGameManager)->Get_LevelObject_WORLDMAP();
		Safe_AddRef(mTerrain_Maps[MAPTYPE_DUNGEON]);
		Safe_AddRef(mTerrain_Maps[MAPTYPE_WORLD]);

		mComNavi[MAPTYPE_DUNGEON] = (CNavigation*)mTerrain_Maps[MAPTYPE_DUNGEON]->Get_ComNavimesh()->Clone(nullptr);
		mComNavi[MAPTYPE_WORLD] = (CNavigation*)mTerrain_Maps[MAPTYPE_WORLD]->Get_ComNavimesh()->Clone(nullptr);
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

HRESULT CGameObject_3D_Dynamic::Set_AniEnum(CAnimatior::E_COMMON_ANINAME name)
{
	if (mComModel == nullptr)
		return E_FAIL;

	return mComModel->Get_Animaitor()->Set_AniEnum(name);
}

HRESULT CGameObject_3D_Dynamic::Set_AniIndex(_uint AniIndex)
{
	if (mComModel == nullptr)
		return E_FAIL;
	return mComModel->Get_Animaitor()->SetUp_AnimIndex(AniIndex);

}
HRESULT CGameObject_3D_Dynamic::Set_AniString(string str)
{
	if (mComModel == nullptr)
		return E_FAIL;
	return mComModel->Get_Animaitor()->Set_AniString(str);
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
	Safe_Release(mComBehavior);
	
	if (mComListCollider != nullptr)
	{
		for (auto& col : *mComListCollider)
		{
			Safe_Release(col);
		}
		mComListCollider->clear();
		Safe_Delete(mComListCollider);
	}

	for (int i =0; i<MAPTYPE_END;++i)
	{
		Safe_Release(mTerrain_Maps[i]);
		Safe_Release(mComNavi[i]);
	}
	mCurrentMap = nullptr;
	mCurrentNavi = nullptr;



}