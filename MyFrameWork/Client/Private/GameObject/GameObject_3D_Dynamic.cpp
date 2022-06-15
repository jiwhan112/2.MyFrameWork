#include "stdafx.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "GameObject/Dungeon_Manager.h"
#include "GameObject/Dungeon_Objects.h"
#include "GameObject/GameObject_Socket.h"

#include "AI/AI_Sequnce.h"

#include "DebugDraw.h"


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

	// 목표위치로 회전

	mComTransform->LookAt_Tick(TimeDelta);

	// Tick_LookUpdate(1);

	Tick_Socket(TimeDelta);

	//	Tick_DEBUG(TimeDelta);


	return UPDATENONE;
}


_int CGameObject_3D_Dynamic::LateTick(_double TimeDelta)
{

	FAILED_UPDATE(__super::LateTick(TimeDelta));
	//if (meTickType == TICK_TOOL)
	//{
	//	mComModel->Update_CombinedTransformationMatrices(TimeDelta);
	//	if (GetSingle(CGameInstance)->IsIn_WorldSpace(Get_WorldPostition(), 2.f))
	//		mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	//}


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



	// 공통 업데이트
	if (mComBehavior)
		mComBehavior->LateTick(TimeDelta);

	if (mCurrentNavi)
	{
		mCurrentNavi->Move_OnNavigation(Get_WorldPostition());
		Set_Position(mCurrentNavi->Move_OnNavigation_Able(Get_WorldPostition()));
	}
		
	

	if (GetSingle(CGameInstance)->IsIn_WorldSpace(Get_WorldPostition(), 2.f))
	{
		mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
		if (mComModel)
			mComModel->Update_CombinedTransformationMatrices(TimeDelta);
	}
	else
	{
		if (mComModel)		
			mComModel->Update_CombinedTransformationMatrices_OnlyTime(TimeDelta);
		
	}

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

		_float3 CamDir = _float3();
		if (mIsPickTurn)
			CamDir = CamTrans->GetWorldFloat4x4().Backward();
		else
			CamDir = CamTrans->GetWorldFloat4x4().Forward();

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
	// #AI 공통 AI 세팅 
	// 다이나믹 객체 공통 정의 AI

	// 생성 / 내려오기 / PICK / OpenDoor /

	if (meUnitType == CGameObject_3D_Dynamic::UNIT_PLAYER || 
		meUnitType == CGameObject_3D_Dynamic::UNIT_ENEMY)
	{
		// 내려오기
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

		// 문
		CSequnce_MOVETARGET* Seq_Door = CSequnce_MOVETARGET::Create(this);
		CSequnce_MOVETARGET::SEQMOVETARGET DefaultDoorDesc;
		Seq_Door->Restart(&DefaultDoorDesc);
		mComBehavior->Add_Seqeunce("DOOR", Seq_Door);

		// 상태전환시 데이터 추가
		CSequnce_MOVETARGET* Seq_Fall = CSequnce_MOVETARGET::Create(this);
		CSequnce_MOVETARGET::SEQMOVETARGET DefaultFallDesc;
		Seq_Fall->Restart(&DefaultFallDesc);
		mComBehavior->Add_Seqeunce("FALL", Seq_Fall);

		// World
		// WorldIdle / WorldMove
		CSequnce_WorldIdle* Seq_WorldIdle = CSequnce_WorldIdle::Create(this);
		mComBehavior->Add_Seqeunce("WORLD_IDLE", Seq_WorldIdle);

		mComBehavior->Select_Sequnce("CREATE_FALL");
	}

	// 각각 타입에 따른 공통 AI
	if (meUnitType == CGameObject_3D_Dynamic::UNIT_PLAYER)
	{
		CSequnce_WorldMove_Player* Seq_WorldMove = CSequnce_WorldMove_Player::Create(this);
		CSequnce_WorldMove_Player::SEQWORLDMOVE_PlAYER WorldDesc;
		WorldDesc.GoalPosition = _float3(0, 0, 0);
		Seq_WorldMove->Restart(&WorldDesc);
		mComBehavior->Add_Seqeunce("WORLD_MOVE", Seq_WorldMove);

		CSequnce_WorldAttack_Player* Seq_WorldAttack = CSequnce_WorldAttack_Player::Create(this);
		CSequnce_WorldAttack_Player::SEQWORLDATTACK_PLY attackDesc;
		Seq_WorldAttack->Restart(&attackDesc);
		mComBehavior->Add_Seqeunce("WORLD_ATTACK", Seq_WorldAttack);


	}

	else if (meUnitType == CGameObject_3D_Dynamic::UNIT_ENEMY)
	{

	}

	else if (meUnitType == CGameObject_3D_Dynamic::UNIT_BOSS)
	{
		
	}

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


void CGameObject_3D_Dynamic::Set_LookDir(_float3 dir)
{
	mComTransform->LookAtDir(dir);
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
	Set_BehaviorMode();
	Select_Door();


	return S_OK;
}

void CGameObject_3D_Dynamic::Set_LookAt_RotateXZ(_float4x4 mat)
{
	_float3 scale = mComTransform->Get_Scale();
	_float3 right = mat.Right();
	_float3 look = mat.Backward();

	right.Normalize();
	look.Normalize();

	right *= scale.x;
	look *= scale.z;

	mComTransform->Set_State(CTransform::STATE_RIGHT, -right);
	mComTransform->Set_State(CTransform::STATE_LOOK, -look);

}

void CGameObject_3D_Dynamic::Set_RotateXZ(_float4x4 mat)
{
	_float3 scale = mComTransform->Get_Scale();
	_float3 right = mat.Right();
	_float3 look = mat.Backward();

	right.Normalize();
	look.Normalize();

	right *= scale.x;
	look *= scale.z;

	mComTransform->Set_State(CTransform::STATE_RIGHT, right);
	mComTransform->Set_State(CTransform::STATE_LOOK, look);

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

HRESULT CGameObject_3D_Dynamic::CollisionFunc(CGameObject_3D_Dynamic * dynamic,_double Timer)
{
	if (meCurrentMap == CGameObject_3D_Dynamic::MAPTYPE_WORLD)
	{
		// 서로 밀어내기
		_float3 look = dynamic->Get_WorldPostition() - Get_WorldPostition();
		look.Normalize();
		mComTransform->MovetoDir(-look, Timer);
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
		{
			// 월드맵 경로 보간
			if (meCurrentMap == CGameObject_3D_Dynamic::MAPTYPE_WORLD)
				WorldPathLerp();
			mGoalPosition = mCurrentPathList.back()->Get_CenterPoint();
		}

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

_bool CGameObject_3D_Dynamic::FindPathRandDungeonAblePostition(_uint Mincount,_uint NebiorMaxCount, _float3* GoalPos)
{
	// 던전내에 Count 만큼 이웃하는 타일 찾아서 반환

	int NeiborCount = CHelperClass::RandomInt(Mincount, NebiorMaxCount);

	_float3 CurrentPos = Get_WorldPostition();
	const CCell* StartCell = mCurrentNavi->Get_CurrentCell();
	const CCell* CurrentCell = StartCell;

	list<int> ListPreIndex;
	ListPreIndex.push_front(mCurrentNavi->Get_CurrentCellIndex());

	int cnt_neviorSearch = 0;
	int cnt_while = 0;
	while(cnt_neviorSearch<NeiborCount)
	{
		cnt_while++;

		// 각 셀에 연결된 랜덤위치 반환
		// 이전 인덱스 X
		int NextIndex = CurrentCell->Get_RandomNeighborIndex();
		if (NextIndex == -1)
			continue;
		bool isInListCell = false;
		for (auto& index : ListPreIndex)
		{

			if (index == NextIndex)
			{
				isInListCell = true;
				break;
			}
		}

		if (isInListCell == false)
		{
			ListPreIndex.push_front(NextIndex);
			const CCell* nextCell = mCurrentNavi->Get_CellFromIndex(NextIndex);
			if (nextCell != nullptr && nextCell->Get_CellType() != CCell::CELLTYPE_STOP)
			{
				CurrentCell = nextCell;
				cnt_neviorSearch++;
			}
		}

		if (cnt_while > 500)
			break;
	}

	if (StartCell == CurrentCell)
		return false;

	_float3 TargetPos = CurrentCell->Get_CenterPoint();

	FAILED_CHECK_NONERETURN(FindPathForCurrentNavi(TargetPos));
	if (mCurrentPathList.empty() == false)
	{
		*GoalPos = TargetPos;
		return true;
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

void CGameObject_3D_Dynamic::Set_RotationFlag(_float3 Target)
{
	mComTransform->Set_RotationFlag(Target);
}

HRESULT CGameObject_3D_Dynamic::WorldPathLerp(_float range)
{

	// 월드맵 경로 보간
	
	// 1. 인접 인덱스라면 삭제하는 방법
	// 생각보다 괜찮음..
	list<CCell*> newPath;
	newPath.push_back(*mCurrentPathList.begin());

	for (auto iter = mCurrentPathList.begin(); iter != mCurrentPathList.end();)
	{
		auto nextiter = iter;
		nextiter++;

		for (; nextiter != mCurrentPathList.end(); nextiter++)
		{
			_float3 PosA = (*iter)->Get_CenterPoint();
			_float3 PosB = (*nextiter)->Get_CenterPoint();

			if (_float3::Distance(PosA, PosB) > range)
			{
				// 현재 탐색 
				iter = nextiter;
				newPath.push_back(*iter);
				break;
			}
		}
		iter++;
	}

	mCurrentPathList.clear();
	mCurrentPathList.assign(newPath.begin(), newPath.end());


	return S_OK;
}

//
//void CGameObject_3D_Dynamic::MoveAbleNaviMesh()
//{
//	// 갈 수 있는 타일로 업데이트
//	if (mCurrentNavi->Get_CurrentCell()->Get_CellType() == CCell::CELLTYPE_STOP)
//	{
//		mCurrentNavi
//	}
//	if (mCurrentNavi->Get_CurrentCell()->Get_CellType());
//}


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

HRESULT CGameObject_3D_Dynamic::Init_Collider()
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
//	CurrentPostiton.y = terrain->Get_HeightY(CurrentPostiton);
	CurrentPostiton.y = terrain->Get_HeightY(_float3(0,0,0));
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
	Desc.eExitFunc = CAction_Function::E_FUNCION::FUNCION_NONE;
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

HRESULT CGameObject_3D_Dynamic::Select_WorldPostition(_float3 pos)
{
	FAILED_CHECK(FindPathForCurrentNavi(pos));
	CSequnce_WorldMove_Player::SEQWORLDMOVE_PlAYER seq;
	seq.GoalPosition = pos;
	mTarget_Attack = nullptr;

	FAILED_CHECK(mComBehavior->Select_Sequnce("WORLD_MOVE"));
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Set_BehaviorMode(int index)
{
	if (index != -1)
	{
		mComBehavior->Set_Mode(index);
		return S_OK;
	}

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

HRESULT CGameObject_3D_Dynamic::AttackFunc()
{
	if (mTarget_Attack)
	{
		if (mTarget_Attack->Get_Hp() <= 0)
		{
			mTarget_Attack = nullptr;
		}
		else
		{
			mTarget_Attack->HitFunc(mDamage);

		}

	}
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::HitFunc(_int Damage)
{
	 mHP -= Damage; 
	 if (mHP <= 0)
		 DieFunc(); 
	 return S_OK;
}

HRESULT CGameObject_3D_Dynamic::DieFunc()
{
	 Set_Dead();
	 return S_OK; 
}

HRESULT CGameObject_3D_Dynamic::Add_Socket_Model(string tag, string modelName, string boneName)
{
	auto find = Find_Socket(tag);
	if (find)
		return E_FAIL;

	auto Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
	CGameObject_3D_Socket::SOCKETDESC socketDesc;
	socketDesc.mTargetModel = Get_ComModel();
	socketDesc.mTransform = Get_ComTransform();
	socketDesc.mSocketName = boneName.c_str();

	CGameObject_3D_Socket* Socket = (CGameObject_3D_Socket*)Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_SOCKET);
	NULL_CHECK_BREAK(Socket);

	Socket->Set_LoadSocketDESC(modelName.c_str(), socketDesc);
	Socket->Set_SocketType(CGameObject_3D_Socket::SOCKETTYPE_MODEL);

	mMapListSocket.emplace(tag, Socket);
	return S_OK;
}
HRESULT CGameObject_3D_Dynamic::Add_Socket_NULL(string tag, string boneName)
{
	auto find = Find_Socket(tag);
	if (find)
		return E_FAIL;

	auto Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
	CGameObject_3D_Socket::SOCKETDESC socketDesc;
	socketDesc.mTargetModel = Get_ComModel();
	socketDesc.mTransform = Get_ComTransform();
	socketDesc.mSocketName = boneName.c_str();

	CGameObject_3D_Socket* Socket = (CGameObject_3D_Socket*)Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_SOCKET);
	NULL_CHECK_BREAK(Socket);

	Socket->Set_LoadSocketDESC(nullptr, socketDesc);
	Socket->Set_SocketType(CGameObject_3D_Socket::SOCKETTYPE_NOMODEL);

	mMapListSocket.emplace(tag, Socket);
	return S_OK;
}

CGameObject_3D_Socket * CGameObject_3D_Dynamic::Find_Socket(string tag)
{
	// 소켓 찾기
	auto iter = mMapListSocket.find(tag);
	if (iter == mMapListSocket.end())
		return nullptr;
	return iter->second;
}

HRESULT CGameObject_3D_Dynamic::Set_SocketVisible(string tag, _bool vis)
{
	auto findobj = Get_SocketObj(tag);
	if (findobj == nullptr)
		return E_FAIL;

	findobj->Set_isVisible(vis);

	return S_OK;
}

CGameObject_3D_Socket * CGameObject_3D_Dynamic::Get_SocketObj(string tag)
{
	auto iter = mMapListSocket.find(tag);
	if (iter == mMapListSocket.end())
		return nullptr;
	return iter->second;
}

CTransform * CGameObject_3D_Dynamic::Get_Socket_Trans(string tag)
{
	auto iter = mMapListSocket.find(tag);
	if (iter == mMapListSocket.end())
		return nullptr;
	return iter->second->Get_ComTransform();
}

HRESULT CGameObject_3D_Dynamic::Tick_Socket(_double timer)
{
	for (auto& sock : mMapListSocket)
	{
		if (sock.second->Get_IsRenderer())
			sock.second->Tick(timer);
	}
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Render_Socket()
{
	if (mMapListSocket.empty() == false)
	{
		for (auto& sock : mMapListSocket)
		{
			if (sock.second->Get_IsRenderer())
				sock.second->Render();
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

	for (auto& socket: mMapListSocket)
	{
		Safe_Release(socket.second);
	}
	mMapListSocket.clear();

}