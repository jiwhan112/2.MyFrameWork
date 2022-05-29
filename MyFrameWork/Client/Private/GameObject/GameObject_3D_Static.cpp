#include "stdafx.h"
#include "GameObject/GameObject_3D_Static.h"
#include "GameObject/GameObject_MyTerrain.h"

CGameObject_3D_Static::CGameObject_3D_Static(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_STATIC;
}

CGameObject_3D_Static::CGameObject_3D_Static(const CGameObject_3D_Static& rhs)
	: CGameObject_Base(rhs)
	, mComModel(rhs.mComModel)
	, mComCollider(rhs.mComCollider)
	, mModelStatic_Desc(rhs.mModelStatic_Desc)
{
	Safe_AddRef(mComModel);
	Safe_AddRef(mComCollider);
}

HRESULT CGameObject_3D_Static::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());


	mCurrentShaderPass = 0;
	return S_OK;
}

HRESULT CGameObject_3D_Static::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	// 데이터 디폴트 세팅
	if (IsName(mModelStatic_Desc.mModelName) == false)
	{
		string str("crea_SnotPickaxe.fbx");
		strcpy_s(mModelStatic_Desc.mModelName, str.c_str());
	}
	Set_Component();

	return S_OK;
}

_int CGameObject_3D_Static::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	if (meUpdateType == CGameObject_3D_Static::E_UPDATETYPE_NONE)
	{
		mCurrentShaderPass = (int)E_SHADERPASS_STATICMODEL_DEFAULT;
	}

	else if (meUpdateType == CGameObject_3D_Static::E_UPDATETYPE_PICK)
	{
		mCurrentShaderPass = (int)E_SHADERPASS_STATICMODEL_DEFAULT;

		// 피킹오브젝트는 해당 지형에 매핑된다.
		CGameObject_MyTerrain* terrain = (CGameObject_MyTerrain*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN_DUNGEON));
		if (terrain != nullptr)
		{
			_float3 worldPickPos = GetSingle(CGameManager)->Get_PickPos();
			int index = terrain->Get_TileIndex(worldPickPos);
			_float3 pickTilePos = terrain->Get_TileWorld(index);
			pickTilePos.y = worldPickPos.y + mOffsetY;

			_float4x4 transmat = _float4x4::CreateTranslation(pickTilePos);
			mComTransform->Set_State(CTransform::STATE_POSITION, transmat.Translation());
		}
	}

	mComCollider->Update_Transform(mComTransform->GetWorldFloat4x4());
	return UPDATENONE;
}

_int CGameObject_3D_Static::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	if (GetSingle(CGameInstance)->IsIn_WorldSpace(Get_WorldPostition(), 2.0f))
		mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);



	return UPDATENONE;
}

HRESULT CGameObject_3D_Static::Render()
{
	FAILED_CHECK(Set_ConstantTable_World());
	FAILED_CHECK(Set_ConstantTable_Light());
	FAILED_CHECK(Set_ConstantTable_Model());

	if (mComModel != nullptr)
	{
		_uint iNumMaterials = mComModel->Get_NumMaterials();

		// 재질 개수만큼 루프
		for (int i = 0; i < iNumMaterials; ++i)
		{
			// 1. Diffuse 텍스처 설정
			mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);
			
			// 2. 랜더링
			// 여기서 뼈를 넘긴다.
			mComModel->Render(mComShader, mCurrentShaderPass, i, "g_BoneMatrices");
		}
	}
#ifdef _DEBUG
	mComCollider->Render();
#endif // _DEBUG
	return S_OK;
}

HRESULT CGameObject_3D_Static::CollisionFunc(_float3 PickPosition, _float dist)
{
	int i = 5;

	return S_OK;
}

HRESULT CGameObject_3D_Static::Set_LoadModelDESC(const MODEL_STATIC_DESC & desc)
{
	memcpy(&mModelStatic_Desc, &desc, sizeof(MODEL_STATIC_DESC));

	// 해당 모델 컴포넌트로 변경
	if (mComModel != nullptr)
	{
		Safe_Release(mComModel);
		mComModel = nullptr;
	}

	string strModel = mModelStatic_Desc.mModelName;
	wstring ModelName = CHelperClass::Convert_str2wstr(strModel);

	FAILED_CHECK(__super::Release_Component(TEXT("Com_Model")));

	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, ModelName.c_str(), TEXT("Com_Model"), (CComponent**)&mComModel));
	

	return S_OK;
}

HRESULT CGameObject_3D_Static::Set_LoadColliderDESC(const COLLIDER_DESC & desc)
{
	memcpy(&mCollider_Desc, &desc, sizeof(COLLIDER_DESC));

	// 해당 모델 컴포넌트로 변경
	if (mComCollider != nullptr)
	{
		Safe_Release(mComCollider);
		mComCollider = nullptr;
	}

	;

	FAILED_CHECK(__super::Release_Component(TEXT("Com_Collider")));

	switch (mCollider_Desc.meColliderType)
	{
	default:
		break;
	case CCollider::COL_AABB:
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_AABB), TEXT("Com_Collider"), (CComponent**)&mComCollider));
		break;
	case CCollider::COL_OBB:
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_OBB), TEXT("Com_Collider"), (CComponent**)&mComCollider));
		break;
	case CCollider::COL_SPHERE:
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_SPHERE), TEXT("Com_Collider"), (CComponent**)&mComCollider));
		break;
	case CCollider::COL_END:
		break;
	}
	mComCollider->Set_Scale(mCollider_Desc.mSize);
	mComCollider->Set_Offset(mCollider_Desc.mOffset);
	mComCollider->Update_Transform(mComTransform->GetWorldFloat4x4());

	return S_OK;
}

void CGameObject_3D_Static::Update_CombinedTransformationMatrix(_fmatrix parentMat)
{
	// 부모 행렬을 넘겨서 자식객체에 업데이트 시켜준다.
	mComTransform->UpdateMatrix(parentMat);
}

HRESULT CGameObject_3D_Static::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	// 모델 타입에 따라 정적모델 동적모델 처리

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));

	if (mComModel == nullptr)
	{
		if (IsName(mModelStatic_Desc.mModelName))
		{
			string strModel = mModelStatic_Desc.mModelName;
			wstring ModelName = CHelperClass::Convert_str2wstr(strModel);
			FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, ModelName.c_str(), TEXT("Com_Model"), (CComponent**)&mComModel));
		}
	}
	if (mComCollider == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_SPHERE), TEXT("Com_Collider"), (CComponent**)&mComCollider));
	}
	//if (mComTexture == nullptr)
	//{
	//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP_FBX), TEXT("Com_Texture"), (CComponent**)&mComTexture));
	//}

	return S_OK;
}

HRESULT CGameObject_3D_Static::Set_ConstantTable_Model()
{
	// 이외 텍스처 바인딩
	if (mComTexture == nullptr)
		return S_OK;

	//auto tex1 = mComTexture->Get_MapTexture(mTexture_Model_DESC.mTextureKey_Normal);
	//auto tex2 = mComTexture->Get_MapTexture(mTexture_Model_DESC.mTextureKey_Hieght);

	//if (tex1)
	//	mComShader->Set_Texture(STR_TEX_NOMAL, tex1);

	//if (tex2)
	//	mComShader->Set_Texture(STR_TEX_HEIGHT, tex2);

	return S_OK;
}

CGameObject_3D_Static * CGameObject_3D_Static::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_3D_Static*	pInstance = NEW CGameObject_3D_Static(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Static");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_3D_Static* CGameObject_3D_Static::Clone(void* pArg)
{
	CGameObject_3D_Static*	pInstance = NEW CGameObject_3D_Static(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Static");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D_Static::Free()
{
	__super::Free();

	Safe_Release(mComModel);
	Safe_Release(mComCollider);
	Safe_Release(mComTexture);
	
}