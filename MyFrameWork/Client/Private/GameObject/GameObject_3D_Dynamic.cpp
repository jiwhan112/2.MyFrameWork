#include "stdafx.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "GameObject/Dungeon_Manager.h"
#include "GameObject/Dungeon_Objects.h"
#include "AI/AI_Action.h"

CGameObject_3D_Dynamic::CGameObject_3D_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC;
}

CGameObject_3D_Dynamic::CGameObject_3D_Dynamic(const CGameObject_3D_Dynamic& rhs)
	: CGameObject_Base(rhs)
	, mModelDesc(rhs.mModelDesc)
	, mIsTerrainHeight(false)

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

	// 생성시 유닛별 초기화 함수 따로 만들기..

	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_DUNGEON);
//	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_WORLD);
	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());

	Init_Unit();
	Init_AI();
	
	Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE);
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
	Tick_LookUpdate(TimeDelta);


	if (meTickType == CGameObject_3D_Dynamic::TICK_TYPE_NONE)
	{
		if (mIsTerrainHeight)
			Set_Terrain_HeightY(mCurrentMap);

	}
	else if (meTickType == CGameObject_3D_Dynamic::TICK_TYPE_DUNGION_PICK)
	{
		_ray	ray = GetSingle(CGameManager)->Get_WorldRay();
		_float3	newPos = ray.position;
		newPos += ray.direction * mMouseOffset;
		CTransform* CamTrans = GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_CAMERA))->Get_ComTransform();
		_float3 CamDir =  CamTrans->GetWorldFloat4x4().Backward();
		CamDir.y = 0;
		CamDir.Normalize();
		mComTransform->LookAtDir(CamDir);
		Set_Position(newPos);

	}

	return UPDATENONE;
}


_int CGameObject_3D_Dynamic::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	if (mCurrentMap == nullptr)
		return UPDATEERROR;

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
	// 유닛마다 상속해서 초기화
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Init_AI()
{
	// 유닛마다 상속해서 초기화
	FAILED_CHECK(Init_Create());
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Init_Create()
{
	// 시퀀스 정의
	// 내려오기 PICK OpenDoor

	CNode_Seqeunce* Seq_Create_Fall = CNode_Seqeunce::Create();
	CNode_Seqeunce* Seq_Fall = CNode_Seqeunce::Create();
	CNode_Seqeunce* Seq_Pick = CNode_Seqeunce::Create();
	CNode_Seqeunce* Seq_OpenDoor = CNode_Seqeunce::Create();

	// CloneAction
	CAction_DEALY* dealyTime = (CAction_DEALY*)mComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	dealyTime->SetUp_Target(this);
	CAction_DEALY* dealyAnimation = (CAction_DEALY*)mComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	dealyAnimation->SetUp_Target(this);
	CAction_DEALY* dealyAniIdle = (CAction_DEALY*)mComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	dealyAniIdle->SetUp_Target(this);
	CAction_MOVE_TARGET* fallMove = (CAction_MOVE_TARGET*)mComBehavior->Clone_Leaf(TAGAI(AI_MOVETARGET));
	fallMove->SetUp_Target(this);

	// Set_Fall
	dealyTime->Set_TimeMax(0.2f);
	fallMove->Set_MoveTargetFlag(CAction_MOVE_TARGET::MOVETARGETFALG_CREATE_FALL);
	dealyAnimation->Set_Animation(CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_UP);
	dealyAniIdle->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);



	Seq_Create_Fall->PushBack_LeafNode(fallMove->Clone());
	Seq_Create_Fall->PushBack_LeafNode(dealyAnimation->Clone());
	Seq_Create_Fall->PushBack_LeafNode(dealyAniIdle->Clone());

	fallMove->Set_MoveTargetFlag(CAction_MOVE_TARGET::MOVETARGETFALG_MOUSEPOS_FALL);
	Seq_Fall->PushBack_LeafNode(fallMove->Clone());
	Seq_Fall->PushBack_LeafNode(dealyAnimation->Clone());
	Seq_Fall->PushBack_LeafNode(dealyAniIdle->Clone());


	Seq_Create_Fall->Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);
	mComBehavior->Add_Seqeunce("CREATE_FALL", Seq_Create_Fall);

	Seq_Fall->Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);
	mComBehavior->Add_Seqeunce("FALL", Seq_Fall);

	// Set_Pick
	dealyAnimation->Set_Animation(CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_DRAG);
	Seq_Pick->PushBack_LeafNode(dealyAnimation->Clone());
	// LOOP 타입은 시퀀스를 반복한다.
	Seq_Pick->Set_SeqType(CNode_Seqeunce::SEQTYPE_LOOP);
	mComBehavior->Add_Seqeunce("DRAG", Seq_Pick);

	// Set_Door
	dealyTime->Set_TimeMax(5.0f);
	Seq_OpenDoor->PushBack_LeafNode(dealyTime->Clone());
	fallMove->Set_MoveTargetFlag(CAction_MOVE_TARGET::MOVETARGETFALG_OBJECTTARGET);
	Seq_OpenDoor->PushBack_LeafNode(fallMove->Clone());
	Seq_OpenDoor->Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);
	mComBehavior->Add_Seqeunce("DOOR", Seq_OpenDoor);



	mComBehavior->Select_Sequnce("CREATE_FALL");

	Safe_Release(dealyTime);
	Safe_Release(dealyAnimation);
	Safe_Release(fallMove);
	Safe_Release(dealyAniIdle);
	return S_OK;
}

HRESULT  CGameObject_3D_Dynamic::Set_MapSetting(E_MAPTYPE type)
{
	if (type == CGameObject_3D_Dynamic::MAPTYPE_END)
		return E_FAIL;
	meCurrentMap = type;
	mCurrentMap = mTerrain_Maps[type];
	mCurrentNavi = mComNavi[type];




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


HRESULT CGameObject_3D_Dynamic::Switch_MapType()
{

	// 맵 / 네비메시 변경
	if (meCurrentMap == CGameObject_3D_Dynamic::MAPTYPE_DUNGEON)
		Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_WORLD);
	else
		Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_DUNGEON);


	meTickType = CGameObject_3D_Dynamic::TICK_TYPE_NONE;

	// 초기위치 // 
	// 초기상태 // 문 나오는 연출
	_float3 GoalPos = _float3();										

	switch (meCurrentMap)
	{
	case CGameObject_3D_Dynamic::MAPTYPE_DUNGEON:
		// 던전으로 전환
		GoalPos = mCustomMovePostition = GetSingle(CGameManager)->Get_DaungonManager()->mDungeonDoorStartPos;
		mCustomMovePostition = GetSingle(CGameManager)->Get_DaungonManager()->mDungeonDoorGoalPos;
		break;
	case CGameObject_3D_Dynamic::MAPTYPE_WORLD:
		// 월드로 전환
		GoalPos = mCustomMovePostition = GetSingle(CGameManager)->Get_DaungonManager()->mWorldDoorStartPos;
		mCustomMovePostition = GetSingle(CGameManager)->Get_DaungonManager()->mWorldDoorGoalPos;
		break;
	default:
		return E_FAIL;
	}
	Set_Position(GoalPos);
	mIsTerrainHeight = false;
	Set_AniEnum(CAnimatior::E_COMMON_ANINAME_RUN, 0);

	FAILED_CHECK(mComBehavior->Select_Sequnce("DOOR"));

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::CollisionFunc(_float3 PickPosition, _float dist, _uint ColliderIndex)
{
	// 잘된다.

	if (meUnitType == CGameObject_3D_Dynamic::UNIT_PLAYER)
	{
		// Drag
		if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON)& DIS_Down)
		{
			if (meCurrentMap == CGameObject_3D_Dynamic::MAPTYPE_DUNGEON && meTickType == CGameObject_3D_Dynamic::TICK_TYPE_NONE)
			{
				mComBehavior->Select_Sequnce("DRAG");
				meTickType = CGameObject_3D_Dynamic::TICK_TYPE_DUNGION_PICK;
			}
		}

		// Drag->Fall
		if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON)& DIS_Down)
		{
			if (meCurrentMap == CGameObject_3D_Dynamic::MAPTYPE_DUNGEON && meTickType == CGameObject_3D_Dynamic::TICK_TYPE_DUNGION_PICK)
			{
				mComBehavior->Select_Sequnce("FALL");
				meTickType = CGameObject_3D_Dynamic::TICK_TYPE_NONE;
			}
		}
		if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_WHEEL)& DIS_Down)
		{
			Switch_MapType();
		}
	}

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
		if (!mCurrentPathList.empty())
			mGoalPosition = mCurrentPathList.back()->Get_CenterPoint();
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

_bool CGameObject_3D_Dynamic::FindPathRandAblePostition(_int Range,_float3* GoalPos)
{
	// 범위내에서 갈 수 있는 타일 
	
	int count = 0;
	while (true)
	{
		_float3 CurrentPos = Get_WorldPostition();
		_int RangeX = CHelperClass::RandomInt(-Range, Range);
		_int RangeZ = CHelperClass::RandomInt(-Range, Range);

		CurrentPos.x += RangeX;
		CurrentPos.z += RangeZ;
		FAILED_CHECK_NONERETURN(FindPathForCurrentNavi(CurrentPos));
		if (mCurrentPathList.empty() == false)
		{
			*GoalPos = CurrentPos;
			return true;
		}
		count++;
		if (count > 30)
			return false;
	}

	return true;
}

_float3 CGameObject_3D_Dynamic::Get_PathGoalPostition() const
{
	return mGoalPosition;
}

_float3 CGameObject_3D_Dynamic::Get_TerrainHeightPostition()
{
	// 현재위치에서 지형을 탄 위치 반환
	_float3 TargetPos = Get_WorldPostition();

	if (mCurrentMap == nullptr)
		return TargetPos;

	TargetPos.y = mCurrentMap->Get_HeightY(TargetPos);
	return TargetPos;
}

void CGameObject_3D_Dynamic::Tick_LookUpdate(_double time)
{
	if (mLookPostiton != _float3())
		mComTransform->LookAtY(mLookPostiton, time, mRotSpeed);
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
	_float3 CurrentPostiton = Get_WorldPostition();;
	CurrentPostiton.y = terrain->Get_HeightY(CurrentPostiton);
	mComTransform->Set_State(CTransform::E_STATE::STATE_POSITION, CurrentPostiton.ToVec4(1));
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Set_AniEnum(CAnimatior::E_COMMON_ANINAME name,_int index)
{
	if (mComModel == nullptr)
		return E_FAIL;

	return mComModel->Get_Animaitor()->Set_AniEnum(name, index);
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