#include "stdafx.h"
#include "GameObject/GameObject_Socket.h"

CGameObject_3D_Socket::CGameObject_3D_Socket(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Static(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_SOCKET;
}

CGameObject_3D_Socket::CGameObject_3D_Socket(const CGameObject_3D_Socket& rhs)
	: CGameObject_3D_Static(rhs)

{
	
}

HRESULT CGameObject_3D_Socket::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());


	mCurrentShaderPass = 0;
	return S_OK;
}

HRESULT CGameObject_3D_Socket::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	FAILED_CHECK(Set_Component());

	mCurrentShaderPass = 2;
	
	return S_OK;
}

_int CGameObject_3D_Socket::Tick(_double TimeDelta)
{
	mMatSocket = (*mBoneMatrixPtr.pCombinedMatrix) * mSocketTransformMatrix * mSocketDESC.mTransform->GetWorldMatrix();
	mComCollider->Update_Transform(mMatSocket);
	return UPDATENONE;
}

HRESULT CGameObject_3D_Socket::Render()
{
	if (mComModel == nullptr)
		return E_FAIL;
	if (mComShader == nullptr)
		return E_FAIL;

	

	//_matrix TransformMatrix = XMLoadFloat4x4(mBoneMatrixPtr.pOffsetMatrix)
	//	* XMLoadFloat4x4(mBoneMatrixPtr.pCombinedMatrix) * XMLoadFloat4x4(&mSocketTransformMatrix) 
	//	* mSocketDESC.mTransform->GetWorldMatrix();

	
	//mMatSocket.Right().Normalize();
	//mMatSocket.Up().Normalize();
	//mMatSocket.Backward().Normalize();
	
	//mMatSocket = mComTransform->GetWorldMatrix() * XMLoadFloat4x4(&mMatSocket) *  mSocketDESC.mTransform->GetWorldMatrix();
	
	mComShader->Set_RawValue("g_SocketMatrix", &XMMatrixTranspose(mMatSocket), sizeof(_float4x4));

	__super::Render();
	return S_OK;
}

HRESULT CGameObject_3D_Socket::Set_LoadSocketDESC(const char* MyFbxname, const SOCKETDESC & desc)
{
	if (MyFbxname)
	{
		strcpy_s(mModelStatic_Desc.mModelName, MyFbxname);
		FAILED_CHECK(Set_LoadModelDESC(mModelStatic_Desc));
	}

	memcpy(&mSocketDESC, &desc, sizeof(SOCKETDESC));

	if (mSocketDESC.mTargetModel == nullptr)
		return E_FAIL;

	mBoneMatrixPtr = mSocketDESC.mTargetModel->Get_BoneMatrixPtr(mSocketDESC.mSocketName);
	mSocketTransformMatrix = mSocketDESC.mTargetModel->Get_DefaultMatrix();


	return S_OK;
}

HRESULT CGameObject_3D_Socket::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	// 모델 타입에 따라 정적모델 동적모델 처리

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));

	mComModel = nullptr;

	if (mComCollider == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_SPHERE), TEXT("Com_Collider"), (CComponent**)&mComCollider));
	}

	/*if (mComCollider == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_SPHERE), TEXT("Com_Collider"), (CComponent**)&mComCollider));
	}*/

	return S_OK;
}

//_int CGameObject_3D_Socket::Tick(_double TimeDelta)
//{
//	FAILED_UPDATE(__super::Tick(TimeDelta));
//	mComCollider->Update_Transform(mComTransform->GetWorldFloat4x4());
//
//	return UPDATENONE;

//}
//
//_int CGameObject_3D_Socket::LateTick(_double TimeDelta)
//{
//	FAILED_UPDATE(__super::LateTick(TimeDelta));
//	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
//
//	return UPDATENONE;
//}
//
//HRESULT CGameObject_3D_Socket::Render()
//{
//	
//	return S_OK;
//}

CGameObject_3D_Socket * CGameObject_3D_Socket::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_3D_Socket*	pInstance = NEW CGameObject_3D_Socket(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Socket");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_3D_Socket* CGameObject_3D_Socket::Clone(void* pArg)
{
	CGameObject_3D_Socket*	pInstance = NEW CGameObject_3D_Socket(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Socket");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D_Socket::Free()
{
	__super::Free();


}