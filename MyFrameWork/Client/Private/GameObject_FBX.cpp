#include "stdafx.h"
#include "GameObject_FBX.h"
#include "GameObject_Terrain.h"

CGameObject_FBX::CGameObject_FBX(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CGameObject_FBX::CGameObject_FBX(const CGameObject_FBX& rhs)
	: CGameObject(rhs)
	, mComShader(rhs.mComShader)
	, mComRenderer(rhs.mComRenderer)
	, mComModel(rhs.mComModel)
	, mComCollider(rhs.mComCollider)
{
	Safe_AddRef(mComShader);
	Safe_AddRef(mComRenderer);
	Safe_AddRef(mComModel);
	Safe_AddRef(mComCollider);
}

HRESULT CGameObject_FBX::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_FBX::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	CTransform::TRANSFORMDESC desc;
	desc.RotPersec = XMConvertToRadians(180);
	desc.SpeedPersec = 5.0f;
	mComTransform->SetTransformDesc(desc);

	return S_OK;
}

_int CGameObject_FBX::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	CGameInstance* pGameInstance = GetSingle(CGameInstance);
	if (pGameInstance->Get_DIKeyState(DIK_UPARROW) & DIS_Press)
	{
		mComTransform->GO_Straight(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_DOWNARROW) & DIS_Press)
	{
		mComTransform->GO_Backward(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_LEFTARROW) & DIS_Press)
	{
		mComTransform->Turn(XMVectorSet(0,1,0,0), TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_RIGHTARROW) & DIS_Press)
	{
		mComTransform->Turn(XMVectorSet(0, -1, 0, 0), TimeDelta);
	}
	mComCollider->Update_Transform(mComTransform->GetWorldFloat4x4());

	return UPDATENONE;
}

_int CGameObject_FBX::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	Set_Height();
	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);
	return UPDATENONE;
}

HRESULT CGameObject_FBX::Render()
{
	FAILED_CHECK(__super::Render());
	FAILED_CHECK(Set_ConstantTable());

	_uint iNumMaterials = mComModel->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);
	//	mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);
		mComModel->Render(mComShader, 0, i);
	}

	// 해당 위치에 충돌체 렌더링

#ifdef _DEBUG
	mComCollider->Render();
#endif // _DEBUG

	return S_OK;
}


CGameObject_FBX * CGameObject_FBX::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_FBX*	pInstance = NEW CGameObject_FBX(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_FBX");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_FBX* CGameObject_FBX::Clone(void* pArg)
{
	CGameObject_FBX*	pInstance = NEW CGameObject_FBX(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_FBX");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CGameObject_FBX::Set_Component()
{
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_MODEL), TEXT("Com_Model"), (CComponent**)&mComModel));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_SPHERE), TEXT("Com_Collider"), (CComponent**)&mComCollider));
	return S_OK;
}

HRESULT CGameObject_FBX::Set_ConstantTable()
{
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	FAILED_CHECK(mComTransform->Bind_OnShader(mComShader, STR_MAT_WORLD));
	FAILED_CHECK(mComShader->Set_RawValue(STR_MAT_VIEW, &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(mComShader->Set_RawValue(STR_MAT_PROJ, &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_PROJ), sizeof(_float4x4)));

	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (pLightDesc == nullptr)
		return E_FAIL;

	//	FAILED_CHECK(mComShader->Set_RawValue("g_vLightPos", &pLightDesc->vDiffuse, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_DIR, &pLightDesc->vDirection, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_DIFFUSE, &pLightDesc->vDiffuse, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_AMBIENT, &pLightDesc->vAmbient, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_SPECULAR, &pLightDesc->vSpecular, sizeof(_float4)));

	FAILED_CHECK(mComShader->Set_RawValue(STR_CAMPOS, &pGameInstance->GetCameraPosition_vec(), sizeof(_float4)));


	return S_OK;
}

HRESULT CGameObject_FBX::Set_Height()
{
	// 1. 각 컴포넌트 구하기
	if (mTerrain == nullptr)
	{
		int index = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
		auto listdata = GetSingle(CGameInstance)->Get_GameObjectLayerList(index, TAGLAY(LAY_TERRAIN));
		
		NULL_CHECK_BREAK(listdata);

		mTerrain = (CGameObject_Terrain*)*listdata->begin();

		NULL_CHECK_BREAK(mTerrain);
	}

	CVIBuffer_Terrain*		pVIBuffer = (CVIBuffer_Terrain*)mTerrain->Get_Component(TEXT("Com_VIBuffer"));
	if (nullptr == pVIBuffer)
		return E_FAIL;

	// 2.위치 가져오기
	_vector		vPos = mComTransform->GetState(CTransform::STATE_POSITION);
	_float4x4	WorldMatrix = mComTransform->GetWorldFloat4x4();
	_float4x4	WorldMatrixInverse = WorldMatrix.Invert();

	_float4 vPosition;
	XMStoreFloat4(&vPosition, vPos);

	// 3.공간 맞춰서 연산하고 다시 공간 변환
	// 월드 -> 로컬 -> 연산 -> 로컬 -> 월드
	_float4::Transform(vPosition, WorldMatrixInverse, vPosition);
	vPosition = pVIBuffer->Get_Height(vPosition);	
	_float4::Transform(vPosition, WorldMatrix, vPosition); 

	vPosition.y = 5;
	// 세팅
	mComTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}


void CGameObject_FBX::Free()
{
	Safe_Release(mComShader);
	Safe_Release(mComRenderer);
	Safe_Release(mComModel);
	Safe_Release(mComCollider);
	
	__super::Free();

}

