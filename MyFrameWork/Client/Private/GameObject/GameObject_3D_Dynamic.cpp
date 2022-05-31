#include "stdafx.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "Animatior.h"
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

	// ������ ����Ʈ ����
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

	// ���ֺ� �ʱ�ȭ
	Init_Unit();
	
	// test
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_DUNGEON);
//	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_WORLD);

	return S_OK;
}

_int CGameObject_3D_Dynamic::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());
	mCurrentNavi->Move_OnNavigation(mCurrentPosition);

	// �浹
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

		// ���� ������ŭ ����
		for (int i = 0; i < iNumMaterials; ++i)
		{
			// 1. �ؽ�ó ����
			mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);
			// 2. ������
			// ���⼭ ���� �ѱ��.
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
	// ��ӹ޾Ƽ� ���

	mComTransform->Scaled(_float3(0.5f, 0.5f, 0.5f));
	mComModel->SetUp_AnimIndex(0);

	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_AABB;
	desc.mSize = _float3(0.5f, 0.5f, 0.5f);
	Add_ColliderDesc(&desc, 1);
	Update_Collider();
	mIsNaviPath = false;

	mComModel->Get_Animaitor()->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE);

	// AI ����
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
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Set_LoadModelDynamicDESC(const MODEL_DYNAMIC_DESC & desc)
{
	memcpy(&mModelDesc, &desc, sizeof(MODEL_DYNAMIC_DESC));

	// �ش� �� ������Ʈ�� ����
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
	// �ߵȴ�.


	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::PathTrigger(CNavigation* MyNaviMesh, _float3 TargetXZ)
{
	// ��ġ ã��
	if (mIsNaviPath)
		return S_FALSE;
	
	_uint StartIndex = MyNaviMesh->Get_CurrentCellIndex();
	_uint GoalIndex = StartIndex;

	if (mIsNaviPath == false && mCurrentPathList.empty())
	{
		// �ش� ��ġ�� �׺�޽� �� �ε��� ��ȯ
		if (MyNaviMesh->Get_PickPosForIndex(TargetXZ, &GoalIndex))
		{
			// ��� Ž��
			mCurrentPathList = MyNaviMesh->AstartPathFind(StartIndex, GoalIndex);
			mIsNaviPath = true;
		}
	}

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Update_Move(_double TimeDelta)
{
	// ������ �Լ�

	if (mIsNaviPath)
	{
		mComModel->SetUp_AnimIndex(29);

		if (mIsMoveCell)
		{
			// �� �̵�
			mGoalPosition = mMoveCell->Get_CenterPoint();
			_float3 newLook = _float3(mGoalPosition.x, mStartPosition.y, mGoalPosition.z);
			mComTransform->LookAt(newLook);
			mTimer += TimeDelta;
			mCurrentPosition = GetSingle(CGameInstance)->Easing3(TYPE_Linear, mStartPosition, mGoalPosition, mTimer, mTimeMax);
			Set_Position(mCurrentPosition);

			if (mTimer > mTimeMax)
			{
				mIsMoveCell = false;
			}
			else if (_float3::Distance(mCurrentPosition, mGoalPosition) < 0.5f)
			{
				mIsMoveCell = false;
			}
		}

		else
		{
			// �� ����
			if (mCurrentPathList.empty())
			{
				mIsNaviPath = false;
				mIsMoveCell = false;
				Set_Position(mGoalPosition);
			}
			else
			{
				// Enter To Move
				mMoveCell = mCurrentPathList.front();
				mCurrentPathList.pop_front();
				mIsMoveCell = true;
				mStartPosition = Get_WorldPostition();
				mTimer = 0;
			}
		}
	}

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Create_Sequnce()
{
	// #TEST ������ ��ƾ 2�� �����
	// A��ƾ : 0.5�� ������ 2��
	// B��ƾ : 0.3�� ������ 10��


	CNode_Seqeunce* Seq_DealyA = CNode_Seqeunce::Create();

	// Ŭ�� �����
	CAction_DEALY* dealy3 = CAction_DEALY::Create("Dealy0.3", this, 0.3f);
	CAction_DEALY* dealy5 = CAction_DEALY::Create("Dealy0.5", this, 0.5f);

	Seq_DealyA->PushBack_LeafNode(dealy5->Clone());
	Seq_DealyA->PushBack_LeafNode(dealy5->Clone());

	CNode_Seqeunce* Seq_DealyB = CNode_Seqeunce::Create();
	for (int i =0; i<10 ;++i)
	{
		Seq_DealyB->PushBack_LeafNode(dealy3->Clone());
	}

	mComBehavior->Add_Seqeunce("DealyA",Seq_DealyA);
	mComBehavior->Add_Seqeunce("DealyB",Seq_DealyB);
	mComBehavior->Select_Sequnce("DealyA");
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXANIMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));

	// �� Ÿ�Կ� ���� ������ ������ ó��
	if (mComModel == nullptr)
	{
		// ���� ���� �ڵ����� �������� ������Ʈ�� ����ȴ�.
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
		// ���� Terrain�� �׺� �޽� ����.
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
	// Desc ������ ����Ʈ�� ���� �����Ѵ�.
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

	// Desc ������ ������ �浹ü ����
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

	for (int i =0; i<MAPTYPE_END;++i)
	{
		Safe_Release(mTerrain_Maps[i]);
		Safe_Release(mComNavi[i]);
	}
	mCurrentMap = nullptr;
	mCurrentNavi = nullptr;
}