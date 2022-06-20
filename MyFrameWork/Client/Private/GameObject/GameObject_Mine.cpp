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

	// ����
	// Weapon �׽�Ʈ
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
	// �� ����
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

	// ���� Ÿ��
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_DUNGEON);
	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());

	meUnitType = UNIT_PLAYER;
	meTickType = CGameObject_3D_Dynamic::TICK_TYPE_NONE;
	mTimeForSpeed = 0.5f;

	// �浹 ����
	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc.mOffset = _float3(0, size, 0);
	size = size * 0.5f;
	desc.mSize = _float3(size, size, size);
	Add_ColliderDesc(&desc, 1);
	Init_Collider();

	// �ִϸ��̼�
	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_SPAWNPOS));

	// ����
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
	// ������ Ÿ�� �ڿ� �ֱ�
	if (tile->Get_IsBlocked())
	{
		tile->Add_TileTask_this();
		return;
	}
	// �̹��� Ÿ���� �ִٸ� ����
	if (mSearchTile)
	{
		tile->Add_TileTask_this();
		return;
	}


	// ���� Ÿ�� ��ġ ã��
	_float3 GoalPos =  tile->Get_AbleTilePos(0.5f);
	
	// GoalPos;

	/*
	_uint StartIndex = mCurrentNavi->Get_CurrentCellIndex();
	_uint GoalIndex = StartIndex;
	*/

	// �ش� ��ġ�� �׺�޽� �� �ε��� ��ȯ
	//if (mCurrentNavi->Get_PickPosForIndex(GoalPos, &GoalIndex))
	//{
	//	// ��� Ž��
	//	mCurrentPathList = mCurrentNavi->AstartPathFind(StartIndex, GoalIndex);
	//	if (mCurrentPathList.empty() == false)
	//		mGoalPosition = mCurrentPathList.back()->Get_CenterPoint();
	//}

	//else
	//{
	//	tile->Add_TileTask_this();
	//	return;
	//}

	// Ÿ�� ä��
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
	// �ش� ��ġ�� �� ��ü ��ġ


}

void CGameObject_Mine::Set_Build(_float3 Pos)
{
	// �ش� ��ġ�� ħ�� ��ġ


}

HRESULT CGameObject_Mine::RemoveTile()
{
	if (mSearchTile)
	{
		mSearchTile->RemoveThisTile();
		mSearchTile = nullptr;

		// ����Ʈ �߰�
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
