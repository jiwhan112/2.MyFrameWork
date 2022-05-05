#include "stdafx.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_MyTerrain.h"

CGameObject_3D_Dynamic::CGameObject_3D_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC;

}

CGameObject_3D_Dynamic::CGameObject_3D_Dynamic(const CGameObject_3D_Dynamic& rhs)
	: CGameObject_Base(rhs)
	, mComModel(rhs.mComModel)
	, mModelDesc(rhs.mModelDesc)
	, mComNaviMesh(rhs.mComNaviMesh)
	, mComCollider(rhs.mComCollider)

{
	Safe_AddRef(mComModel);
	Safe_AddRef(mComCollider);
	Safe_AddRef(mComNaviMesh);
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
	mComModel->SetUp_AnimIndex(0);
	meAI = CGameObject_3D_Dynamic::BASEAI_IDLE;

	mComNaviMesh->Set_NaviObjType(CNavigation::NAVI_OBJTYPE_PLAYER);

	return S_OK;
}

_int CGameObject_3D_Dynamic::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));
	mComCollider->Update_Transform(mComTransform->GetWorldFloat4x4());
	

	E_LEVEL Getindex = (E_LEVEL)GetSingle(CGameInstance)->Get_CurrentLevelIndex();

	// 이동 
	if (Getindex == LEVEL_MYGAMEPLAY)
		GOMOVE(TimeDelta);


	return UPDATENONE;
}

_int CGameObject_3D_Dynamic::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	CGameObject_MyTerrain* terrain = (CGameObject_MyTerrain*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN));
	if (terrain != nullptr)
	{
		if (terrain->Get_isPick())
		{

			_float3 worldPos = terrain->Get_PickWorldPos();
			int index = terrain->Get_TileIndex(worldPos);
			mGoalPosition = terrain->Get_TileWorld(index);
			mStartPosition = mComTransform->GetState(CTransform::STATE_POSITION);

			mTimer = 0;
			mTimeMax = _float3::Distance(mStartPosition, mGoalPosition);

			meAI = CGameObject_3D_Dynamic::BASEAI_MOVE;

		}
		//	mComTransform->Set_State(CTransform::STATE_POSITION, pos);


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
	mComCollider->Render();

	CTransform* terraintrans =  GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN))->Get_TransformCom();
	mComNaviMesh->Render(terraintrans);
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

	if (mComCollider == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_SPHERE), TEXT("Com_Collider"), (CComponent**)&mComCollider));

	if (mComNaviMesh == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_NAVIMESH), TEXT("Com_Navimesh"), (CComponent**)&mComNaviMesh));


	return S_OK;
}

void CGameObject_3D_Dynamic::GOMOVE(_double delta)
{
	switch (meAI)
	{
	case Client::CGameObject_3D_Dynamic::BASEAI_IDLE:
		mComModel->SetUp_AnimIndex(25);
		break;
	case Client::CGameObject_3D_Dynamic::BASEAI_MOVE:
	{
		mComTransform->LookAt(mGoalPosition);

		mComModel->SetUp_AnimIndex(29);
		mTimer += delta;
		_float3 CurrentPos = _float3::Lerp(mStartPosition, mGoalPosition, mTimer/ mTimeMax);
		_float4 CurrentPos4;
		CurrentPos4 = CurrentPos;
		CurrentPos4.w = 1;

		if(mComNaviMesh->Move_OnNavigation(CurrentPos4))
		{
			mComTransform->Set_State(CTransform::STATE_POSITION, CurrentPos4);
		}
		else
			meAI = CGameObject_3D_Dynamic::BASEAI_IDLE;



	//	_float distance =  _float3::Distance(CurrentPos, mGoalPosition);

		if (mTimer > mTimeMax)
			meAI = CGameObject_3D_Dynamic::BASEAI_IDLE;

	}

	break;
	case Client::CGameObject_3D_Dynamic::BASEAI_END:
		break;
	default:
		break;
	}

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
	Safe_Release(mComCollider);
	Safe_Release(mComNaviMesh);

}
