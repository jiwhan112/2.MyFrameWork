#include "stdafx.h"
#include "GameObject/GameObject_Mine.h"
#include "GameObject/GameObject_3D_Tile.h"
#include "AI/AI_Sequnce.h"

CGameObject_Mine::CGameObject_Mine(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_MINE;
}

CGameObject_Mine::CGameObject_Mine(const CGameObject_Mine& rhs)
	: CGameObject_3D_Dynamic(rhs)
{
	
}

HRESULT CGameObject_Mine::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());
	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_Mine::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	return S_OK;
}

HRESULT CGameObject_Mine::Tick_Dungeon(_double TimeDelta)
{
	if (UPDATEERROR == __super::Tick_Dungeon(TimeDelta))
		return UPDATEERROR;

	// 던전
	// Weapon 테스트
	if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_3)& DIS_Down)
	{
		_bool isSocketVisible = Get_SocketObj(TAGSOCKET(SOCKET_WEAPON_1))->Get_IsRenderer();
		Set_SocketVisible(TAGSOCKET(SOCKET_WEAPON_1), !isSocketVisible);
	}

	return UPDATENONE;
}

HRESULT CGameObject_Mine::LateTick_Dungeon(_double TimeDelta)
{
	if (UPDATEERROR == __super::LateTick_Dungeon(TimeDelta))
		return UPDATEERROR;

	return UPDATENONE;
}

HRESULT CGameObject_Mine::Tick_World(_double TimeDelta)
{
	if (UPDATEERROR == __super::Tick_World(TimeDelta))
		return UPDATEERROR;

	return UPDATENONE;
}

HRESULT CGameObject_Mine::LateTick_World(_double TimeDelta)
{
	if (UPDATEERROR == __super::LateTick_World(TimeDelta))
		return UPDATEERROR;

	return UPDATENONE;
}

HRESULT CGameObject_Mine::Init_Unit()
{
	// 모델 결정
	string str("crea_Snot_a.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);

	// Transform
	_float3 SpawnPos = mSpawnPostitionDAUNGEON;
	SpawnPos.y += 10;
	Set_Position(SpawnPos);

	Set_LookDir(_float3(-1, 0, -1));


	_float size = 0.6f;
	mComTransform->Scaled(_float3(size, size, size));

	// 유닛 타입
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_DUNGEON);
	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());

	meUnitType = UNIT_PLAYER;
	meTickType = CGameObject_3D_Dynamic::TICK_TYPE_NONE;
	mTimeForSpeed = 0.5f;

	// 충돌 정보
	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc.mOffset = _float3(0, size, 0);
	size = size * 0.5f;
	desc.mSize = _float3(size, size, size);
	Add_ColliderDesc(&desc, 1);
	Init_Collider();

	// 애니메이션
	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_SPAWNPOS));

	// 소켓
	Add_Socket_Model(STR_TAYSOCKET(SOCKET_WEAPON_1),"crea_SnotPickaxe.fbx","RArmDigit31");

	return S_OK;
}

HRESULT CGameObject_Mine::Init_AI()
{
	FAILED_CHECK(__super::Init_AI());

	FAILED_CHECK(Init_AI_MINE());
	return S_OK;
}

HRESULT CGameObject_Mine::Init_AI_MINE()
{
	// IDLE 
	CSequnce_IDLE* Seq_IDLE = CSequnce_IDLE::Create(this);
	CSequnce_IDLE::SEQIDLE DefaultIdleDesc;

	DefaultIdleDesc.mMoveEasingId = TYPE_Linear;
	DefaultIdleDesc.AniType = CAnimatior::E_COMMON_ANINAME_IDLE;

	Seq_IDLE->Restart(&DefaultIdleDesc);
	mComBehavior->Add_Seqeunce("IDLE", Seq_IDLE);

	// TILE
	CSequnce_TILE* Seq_TILE= CSequnce_TILE::Create(this);
	CSequnce_TILE::tag_SeqTILE DefaultTileDesc;
	DefaultTileDesc.DigAniIndex = -1;
	Seq_TILE->Restart(&DefaultTileDesc);
	mComBehavior->Add_Seqeunce("DIG", Seq_TILE);

	// PICK
	CSequnce_PICK* Seq_Pick = CSequnce_PICK::Create(this);
	CSequnce_PICK::SEQPICK DefaultPickDesc;
	DefaultPickDesc.AniType = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_DRAG;
	Seq_Pick->Restart(&DefaultPickDesc);
	mComBehavior->Add_Seqeunce("PICK", Seq_Pick);



	return S_OK;
}

void CGameObject_Mine::Set_Dig_Tile(CGameObject_3D_Tile * tile)
{
	// 못가는 타일 뒤에 넣기
	if (tile->Get_IsBlocked())
	{
		tile->Add_TileTask_this();
		return;
	}
	// 이미지 타일이 있다면 리턴
	if (mSearchTile)
	{
		tile->Add_TileTask_this();
		return;
	}


	// 뚫을 타일 위치 찾기
	_float3 GoalPos =  tile->Get_AbleTilePos(0.5f);
	
	// GoalPos;

	/*
	_uint StartIndex = mCurrentNavi->Get_CurrentCellIndex();
	_uint GoalIndex = StartIndex;
	*/

	// 해당 위치의 네비메시 셀 인덱스 반환
	//if (mCurrentNavi->Get_PickPosForIndex(GoalPos, &GoalIndex))
	//{
	//	// 경로 탐색
	//	mCurrentPathList = mCurrentNavi->AstartPathFind(StartIndex, GoalIndex);
	//	if (mCurrentPathList.empty() == false)
	//		mGoalPosition = mCurrentPathList.back()->Get_CenterPoint();
	//}

	//else
	//{
	//	tile->Add_TileTask_this();
	//	return;
	//}

	// 타일 채굴
	CSequnce_TILE::SEQTILE desc;
	desc.GoalPos = GoalPos;
	desc.DigAniIndex = 1;
	mSearchTile = tile;
	mComBehavior->Select_Sequnce("DIG",&desc);
}

void CGameObject_Mine::Set_Dig_Gold(CGameObject_3D_Tile * tile)
{


}

void CGameObject_Mine::Set_Room(_float3 Pos)
{
	// 해당 위치에 룸 객체 설치


}

void CGameObject_Mine::Set_Build(_float3 Pos)
{
	// 해당 위치에 침대 설치


}

HRESULT CGameObject_Mine::RemoveTile()
{
	if (mSearchTile)
	{
		

		static int stcSoundCount = 0;
		if (stcSoundCount % 2)
		{
			PLAYGAMESOUND(L"attack_trollrockthrower_dig_01.wav", CHANNEL_PLAYER, SOUNDVOL_EFFECT);

		}
		else
		{
			PLAYGAMESOUND(L"attack_trollrockthrower_dig_02.wav", CHANNEL_PLAYER, SOUNDVOL_EFFECT);

		}
		stcSoundCount++;

		mSearchTile->RemoveThisTile();
		mSearchTile = nullptr;

		// 이펙트 추가
	}

	return S_OK;
}

HRESULT CGameObject_Mine::LookTile()
{
	if (mSearchTile)
	{
		_float3 dirPos = mSearchTile->Get_WorldPostition();
		mComTransform->LookAt(dirPos);
	}
	return S_OK;
}


CGameObject_Mine * CGameObject_Mine::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Mine*	pInstance = NEW CGameObject_Mine(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Mine");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Mine* CGameObject_Mine::Clone(void* pArg)
{
	CGameObject_Mine*	pInstance = NEW CGameObject_Mine(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Mine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Mine::Free()
{
	__super::Free();
}
