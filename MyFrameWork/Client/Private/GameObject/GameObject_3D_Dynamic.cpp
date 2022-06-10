#include "stdafx.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "GameObject/Dungeon_Manager.h"
#include "GameObject/Dungeon_Objects.h"
#include "GameObject/GameObject_Socket.h"

#include "AI/AI_Sequnce.h"

CGameObject_3D_Dynamic::CGameObject_3D_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC;
}

CGameObject_3D_Dynamic::CGameObject_3D_Dynamic(const CGameObject_3D_Dynamic& rhs)
	: CGameObject_Base(rhs)
	, mModelDesc(rhs.mModelDesc)
	, mIsTerrainHeight(false)
	, meTickType(rhs.meTickType)

{
	// Dynamic TOOLTEST
	// meTickType = CGameObject_3D_Dynamic::TICK_TOOL;

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

	if (meTickType == TICK_TOOL)
		return UPDATENONE;

	// 생성시 유닛별 초기화 함수 따로 만들기..
	Init_Unit();
	Init_AI();
	
	return S_OK;
}

_int CGameObject_3D_Dynamic::Tick(_double TimeDelta)
{

	FAILED_UPDATE(__super::Tick(TimeDelta));

	// 충돌
	if (meTickType == TICK_TOOL)
	{
		if (mComListCollider != nullptr)
		{
			for (auto& col : *mComListCollider)
			{
				col->Update_Transform(mComTransform->GetWorldFloat4x4());
				GetSingle(CGameManager)->Add_ColliderObject(CColliderManager::E_COLLIDEROBJ_TYPE::COLLIDEROBJ_DYNAMIC, this);
			}
		}
	}

	else
	{
		if (mComBehavior)
			mComBehavior->Tick(TimeDelta);

		if (meCurrentMap == CGameObject_3D_Dynamic::MAPTYPE_DUNGEON)
		{
			FAILED_CHECK_NONERETURN(Tick_Dungeon(TimeDelta));

		}
		else if (meCurrentMap == CGameObject_3D_Dynamic::MAPTYPE_WORLD)
		{
			FAILED_CHECK_NONERETURN(Tick_World(TimeDelta));

		}


		if (mComListCollider)
		{
			for (auto& col : *mComListCollider)
			{
				col->Update_Transform(mComTransform->GetWorldFloat4x4());
				GetSingle(CGameManager)->Add_ColliderObject(CColliderManager::E_COLLIDEROBJ_TYPE::COLLIDEROBJ_DYNAMIC, this);
			}
		}
	}

	
	// 버그 코드
	// Tick_LookUpdate(TimeDelta);
	Tick_Socket(TimeDelta);

//	Tick_DEBUG(TimeDelta);


	return UPDATENONE;
}


_int CGameObject_3D_Dynamic::LateTick(_double TimeDelta)
{
	
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	if (meTickType == TICK_TOOL)
	{
		mComModel->Update_CombinedTransformationMatrices(TimeDelta);
		if (GetSingle(CGameInstance)->IsIn_WorldSpace(Get_WorldPostition(), 2.f))
			mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	}

	else
	{
		if (mCurrentMap == nullptr)
			return UPDATEERROR;

		if (meCurrentMap == CGameObject_3D_Dynamic::MAPTYPE_DUNGEON)
		{
			FAILED_CHECK_NONERETURN(LateTick_Dungeon(TimeDelta));

		}
		else
		{
			FAILED_CHECK_NONERETURN(LateTick_World(TimeDelta));

		}
		
	}

	// 공통 업데이트
	if (mComBehavior)
		mComBehavior->LateTick(TimeDelta);
	if (mComModel)
		mComModel->Update_CombinedTransformationMatrices(TimeDelta);
	if (mCurrentNavi)
		mCurrentNavi->Move_OnNavigation(Get_WorldPostition());

	if (GetSingle(CGameInstance)->IsIn_WorldSpace(Get_WorldPostition(), 2.f))
		mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	return UPDATENONE;
}

HRESULT CGameObject_3D_Dynamic::Tick_Dungeon(_double TimeDelta)
{
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
		_float3 CamDir = CamTrans->GetWorldFloat4x4().Backward();
		CamDir.y = 0;
		CamDir.Normalize();
		mComTransform->LookAtDir(CamDir);
		Set_Position(newPos);

	}

	return UPDATENONE;
}

HRESULT CGameObject_3D_Dynamic::LateTick_Dungeon(_double TimeDelta)
{

	return UPDATENONE;
}

HRESULT CGameObject_3D_Dynamic::Tick_World(_double TimeDelta)
{
	if (meTickType == CGameObject_3D_Dynamic::TICK_TYPE_NONE)
	{
		if (mIsTerrainHeight)
			Set_Terrain_HeightY(mCurrentMap);
	}

	return UPDATENONE;
}

HRESULT CGameObject_3D_Dynamic::LateTick_World(_double TimeDelta)
{
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

	Render_Socket();
	return S_OK;
}



HRESULT CGameObject_3D_Dynamic::Init_Unit()
{
	// 기본 모델
	string str("crea_Snot_a.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);
	
	// 상속으로 초기화

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Init_AI()
{
	// 유닛마다 상속해서 초기화
	FAILED_CHECK(Init_AI_CommonDynamic());
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Init_AI_CommonDynamic()
{
	// 다이나믹 객체 공통 정의 AI

	// 내려오기 / PICK / OpenDoor /

	CSequnce_MOVETARGET* Seq_CreateFall = CSequnce_MOVETARGET::Create(this);
	CSequnce_MOVETARGET::SEQMOVETARGET DefaultCreateFallDesc;
	DefaultCreateFallDesc.Dealytime = 0.1f;
	DefaultCreateFallDesc.TimeMax = 2.0f;
	DefaultCreateFallDesc.StartPosition = Get_WorldPostition();
	DefaultCreateFallDesc.EndPosition = Get_TerrainHeightPostition();
	DefaultCreateFallDesc.EasingID = TYPE_SinIn;
	DefaultCreateFallDesc.AniType = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_UP;
	DefaultCreateFallDesc.eExitFunc = CAction_Function::E_FUNCION::FUNCION_NONE;


	Seq_CreateFall->Restart(&DefaultCreateFallDesc);
	mComBehavior->Add_Seqeunce("CREATE_FALL", Seq_CreateFall);

	CSequnce_PICK* Seq_Pick = CSequnce_PICK::Create(this);
	CSequnce_PICK::SEQPICK DefaultPickDesc;
	DefaultPickDesc.AniType = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_DRAG;
	Seq_Pick->Restart(&DefaultPickDesc);
	mComBehavior->Add_Seqeunce("PICK", Seq_Pick);

	// 상태전환시 데이터 추가
	CSequnce_MOVETARGET* Seq_Fall = CSequnce_MOVETARGET::Create(this);
	CSequnce_MOVETARGET::SEQMOVETARGET DefaultFallDesc;	
	Seq_Fall->Restart(&DefaultFallDesc);
	mComBehavior->Add_Seqeunce("FALL", Seq_Fall);

	CSequnce_MOVETARGET* Seq_Door = CSequnce_MOVETARGET::Create(this);
	CSequnce_MOVETARGET::SEQMOVETARGET DefaultDoorDesc;
	Seq_Door->Restart(&DefaultDoorDesc);
	mComBehavior->Add_Seqeunce("DOOR", Seq_Door);
	
	mComBehavior->Select_Sequnce("CREATE_FALL");


	// WorldIdle / WorldMove
	CSequnce_WorldIdle* Seq_WorldIdle = CSequnce_WorldIdle::Create(this);
	// CSequnce_WorldIdle::SEQWORLDIDLE worldIdle;
	// Seq_Fall->Restart(&worldIdle);
	mComBehavior->Add_Seqeunce("WORLDIDLE", Seq_WorldIdle);

	CSequnce_WorldMove* Seq_WorldMove = CSequnce_WorldMove::Create(this);
	//	CSequnce_WorldMove::tag_SeqWorldMove DefaultDoorDesc;
	//	Seq_Door->Restart(&DefaultDoorDesc);
	mComBehavior->Add_Seqeunce("WORLDMOVE", Seq_WorldMove);


	//CNode_Seqeunce* Seq_Create_Fall = CNode_Seqeunce::Create();
	//CNode_Seqeunce* Seq_Fall = CNode_Seqeunce::Create();
	//CNode_Seqeunce* Seq_Pick = CNode_Seqeunce::Create();
	//CNode_Seqeunce* Seq_OpenDoor = CNode_Seqeunce::Create();
	//// CloneAction
	//CAction_DEALY* dealyTime = (CAction_DEALY*)mComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	//dealyTime->SetUp_Target(this);
	//CAction_DEALY* dealyAnimation = (CAction_DEALY*)mComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	//dealyAnimation->SetUp_Target(this);
	//CAction_DEALY* dealyAniIdle = (CAction_DEALY*)mComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	//dealyAniIdle->SetUp_Target(this);
	//CAction_MOVE_TARGET* fallMove = (CAction_MOVE_TARGET*)mComBehavior->Clone_Leaf(TAGAI(AI_MOVETARGET));
	//fallMove->SetUp_Target(this);
	//// Set_Fall
	//dealyTime->Set_TimeMax(0.2f);
	//fallMove->Set_MoveTargetFlag(CAction_MOVE_TARGET::MOVETARGETFALG_CREATE_FALL);
	//dealyAnimation->Set_Animation(CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_UP);
	//dealyAniIdle->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);
	//Seq_Create_Fall->PushBack_LeafNode(fallMove->Clone());
	//Seq_Create_Fall->PushBack_LeafNode(dealyAnimation->Clone());
	//Seq_Create_Fall->PushBack_LeafNode(dealyAniIdle->Clone());
	//fallMove->Set_MoveTargetFlag(CAction_MOVE_TARGET::MOVETARGETFALG_MOUSEPOS_FALL);
	//Seq_Fall->PushBack_LeafNode(fallMove->Clone());
	//Seq_Fall->PushBack_LeafNode(dealyAnimation->Clone());
	//Seq_Fall->PushBack_LeafNode(dealyAniIdle->Clone());
	//Seq_Create_Fall->Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);
	//mComBehavior->Add_Seqeunce("CREATE_FALL", Seq_Create_Fall);
	//Seq_Fall->Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);
	//mComBehavior->Add_Seqeunce("FALL", Seq_Fall);
	//// Set_Pick
	//dealyAnimation->Set_Animation(CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_DRAG);
	//Seq_Pick->PushBack_LeafNode(dealyAnimation->Clone());
	//// LOOP 타입은 시퀀스를 반복한다.
	//Seq_Pick->Set_SeqType(CNode_Seqeunce::SEQTYPE_LOOP);
	//mComBehavior->Add_Seqeunce("DRAG", Seq_Pick);
	//// Set_Door
	//dealyTime->Set_TimeMax(5.0f);
	//Seq_OpenDoor->PushBack_LeafNode(dealyTime->Clone());
	//fallMove->Set_MoveTargetFlag(CAction_MOVE_TARGET::MOVETARGETFALG_OBJECTTARGET);
	//Seq_OpenDoor->PushBack_LeafNode(fallMove->Clone());
	//Seq_OpenDoor->Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);
	//mComBehavior->Add_Seqeunce("DOOR", Seq_OpenDoor);
	//Safe_Release(dealyTime);
	//Safe_Release(dealyAnimation);
	//Safe_Release(fallMove);
	//Safe_Release(dealyAniIdle);
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

	mIsTerrainHeight = false;
	Set_AniEnum(CAnimatior::E_COMMON_ANINAME_RUN, 0);

	Select_Door();

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
				mComBehavior->Select_Sequnce("PICK");
				meTickType = CGameObject_3D_Dynamic::TICK_TYPE_DUNGION_PICK;
			}
		}

		// Drag->Fall
		if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON)& DIS_Down)
		{
			if (meCurrentMap == CGameObject_3D_Dynamic::MAPTYPE_DUNGEON && meTickType == CGameObject_3D_Dynamic::TICK_TYPE_DUNGION_PICK)
			{
				meTickType = CGameObject_3D_Dynamic::TICK_TYPE_NONE;
				Select_Fall();
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
	// Set_Component
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXANIMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));

	// 각 INIT 함수에서 재정의
	if (mComModel == nullptr)
	{
		// string strModel = mModelDesc.mModelName;
		// wstring ModelName = CHelperClass::Convert_str2wstr(strModel);		
		// FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, ModelName.c_str(), TEXT("Com_Model"), (CComponent**)&mComModel));
	}
	if (mComBehavior == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_BEHAVIORTREE), TEXT("Com_Behavior"), (CComponent**)&mComBehavior));
	}

	if (mTerrain_Maps[MAPTYPE_DUNGEON] == nullptr || mTerrain_Maps[MAPTYPE_WORLD] == nullptr)
	{
		if (meTickType == TICK_TOOL)
			return S_OK;

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

HRESULT CGameObject_3D_Dynamic::Set_Sequnce(const char * statename, void * desc)
{
	// 각 상태마다 void 포인터로 넘겨서 정보전달..
	FAILED_CHECK(mComBehavior->Select_Sequnce(statename, desc));
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Select_Door()
{
	CSequnce_MOVETARGET::SEQMOVETARGET Desc;
	Desc.Dealytime = 5;
	Desc.TimeMax = 2;
	Desc.EasingID = TYPE_Linear;
	Desc.AniType = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_RUN;

	switch (meCurrentMap)
	{
	case CGameObject_3D_Dynamic::MAPTYPE_DUNGEON:
		// 던전으로 전환
		Desc.StartPosition = GetSingle(CGameManager)->Get_DaungonManager()->mDungeonDoorStartPos;;
		Desc.EndPosition =GetSingle(CGameManager)->Get_DaungonManager()->mDungeonDoorGoalPos;;
		break;
	case CGameObject_3D_Dynamic::MAPTYPE_WORLD:
		// 월드로 전환
		Desc.StartPosition = GetSingle(CGameManager)->Get_DaungonManager()->mWorldDoorStartPos;;
		Desc.EndPosition = GetSingle(CGameManager)->Get_DaungonManager()->mWorldDoorGoalPos;;
		break;
	default:
		return E_FAIL;
	}
	Desc.eExitFunc = CAction_Function::E_FUNCION::FUNCION_SETGAMEMODE;
	Set_Position(Desc.StartPosition);
	mComTransform->LookAt(Desc.EndPosition);
	mComBehavior->Select_Sequnce("DOOR",&Desc);
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Select_Fall()
{
	// 상태전환시 데이터 추가
	CSequnce_MOVETARGET::SEQMOVETARGET Desc;
	Desc.Dealytime = 0.2f;
	Desc.TimeMax = 2.0f;
	Desc.EasingID = TYPE_SinIn;
	Desc.AniType = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_UP;

	_float3 mousePos = GetSingle(CGameManager)->Get_PickPos();
	mousePos.y += 10;
	Set_Position(mousePos);
	Desc.StartPosition = mousePos;
	Desc.EndPosition = Get_TerrainHeightPostition();
	Set_IsTerrainHeight(false);

	mComBehavior->Select_Sequnce("FALL", &Desc);
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Select_WorldGo(_float3 pos)
{
	FAILED_CHECK(FindPathForCurrentNavi(pos));
	// CSequnce_WorldMove::SEQWORLDMOVE seq;
	// seq.GoalPosition = pos;
	FAILED_CHECK(mComBehavior->Select_Sequnce("WORLDMOVE"));


	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Set_BehaviorMode()
{
	int modeindex = mComBehavior->Get_Mode();
	switch (modeindex)
	{
	case 0:
		mComBehavior->Set_Mode(1);
		break;
	case 1:
		mComBehavior->Set_Mode(0);
		break;
	}
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Add_Socket(string modelName,string boneName)
{
	auto Create_Manager =  GetSingle(CGameManager)->Get_CreaterManager();

	CGameObject_3D_Socket::SOCKETDESC socketDesc;
	socketDesc.mTargetModel = Get_ComModel();
	socketDesc.mTransform = Get_ComTransform();
	socketDesc.mSocketName = boneName.c_str();
	
	// "crea_SnotPickaxe.fbx"
	CGameObject_3D_Socket* Socket = (CGameObject_3D_Socket*)Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_SOCKET);
	Socket->Set_LoadSocketDESC(modelName.c_str(), socketDesc);
	mListSocket.push_front(Socket);
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Tick_Socket(_double timer)
{

	for (auto& sock : mListSocket)
	{
		sock->Tick(timer);
	}

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Render_Socket()
{
	if (mListSocket.empty() == false)
	{
		for (auto& sock : mListSocket)
		{
			sock->Render();
		}
	}

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Tick_DEBUG(_double timer)
{
	if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_G)& DIS_Down)
	{
		mComBehavior->Clear_Sequnce();

	}
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

	for (auto& socket: mListSocket)
	{
		Safe_Release(socket);
	}
	mListSocket.clear();


}