#include "stdafx.h"
#include "GameObject/GameObject_3D_Tiles.h"
#include "GameObject/GameObject_3D_Static.h"

CGameObject_3D_Tiles::CGameObject_3D_Tiles(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_STATIC_TILES;
}

CGameObject_3D_Tiles::CGameObject_3D_Tiles(const CGameObject_3D_Tiles& rhs)
	: CGameObject_Base(rhs)
{
	// 깊은 복사 처리
	//for (auto& obj : rhs.mVecChildObject)
	//{
	//	CGameObject_3D_Static* newchildobj = obj->Clone(nullptr);
	//	Add_StaticObejct(newchildobj);
	//}
}

HRESULT CGameObject_3D_Tiles::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	// 데이터 디폴트 세팅

	mCurrentShaderPass = 0;
	return S_OK;
}

HRESULT CGameObject_3D_Tiles::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	
	for (int i = 0; i < TILETYPE_END; i++)
	{
		if (mTileObject[i] == nullptr)
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

_int CGameObject_3D_Tiles::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));


	//if (meUpdateType == CGameObject_3D_Tiles::E_UPDATETYPE_NONE)
	//{
	//	mCurrentShaderPass = (int)E_SHADERPASS_STATICMODEL_DEFAULT;
	//}

	//else if (meUpdateType == CGameObject_3D_Tiles::E_UPDATETYPE_PICK)
	//{
	//	mCurrentShaderPass = (int)E_SHADERPASS_STATICMODEL_RED;

	//	// 피킹오브젝트는 해당 지형에 매핑된다.
	//	CGameObject_MyTerrain* terrain = (CGameObject_MyTerrain*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN));
	//	if (terrain != nullptr)
	//	{
	//		if (terrain->Get_isPick())
	//		{
	//			_float3 worldPos = terrain->Get_PickWorldPos();
	//			int index = terrain->Get_TileIndex(worldPos);

	//			_float3 pickTilePos = terrain->Get_TileWorld(index);			

	//			_float4x4 transmat = _float4x4::CreateTranslation(pickTilePos);
	//			mComTransform->Set_State(CTransform::STATE_POSITION, transmat.Translation());
	//			
	//		}
	//	}
	//}
	// FAILED_CHECK_NONERETURN(Tick_Child(TimeDelta));

	return UPDATENONE;
}

_int CGameObject_3D_Tiles::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	return UPDATENONE;
}

HRESULT CGameObject_3D_Tiles::Render()
{
	FAILED_CHECK(Set_ConstantTable_World());
	FAILED_CHECK(Set_ConstantTable_Light());


	return S_OK;
}


HRESULT CGameObject_3D_Tiles::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	// 모델 타입에 따라 정적모델 동적모델 처리

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));


	return S_OK;
}

void CGameObject_3D_Tiles::RenderOnOff(int index,bool b)
{
	
	mTileObject[index]->Set_isVisible(b);	
}

CGameObject_3D_Tiles * CGameObject_3D_Tiles::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_3D_Tiles*	pInstance = NEW CGameObject_3D_Tiles(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Tiles");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_3D_Tiles* CGameObject_3D_Tiles::Clone(void* pArg)
{
	CGameObject_3D_Tiles*	pInstance = NEW CGameObject_3D_Tiles(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Tiles");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D_Tiles::Free()
{
	__super::Free();

	
}