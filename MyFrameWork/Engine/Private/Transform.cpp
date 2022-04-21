#include "..\Public\Transform.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, mWorldMatrix(rhs.mWorldMatrix)
{
}

_vector CTransform::GetState(E_STATE state) const
{
	return XMLoadFloat4((_float4*)&mWorldMatrix.m[state][0]);
}

_vector CTransform::GetScale(E_STATE state) const
{
	return XMVector3Length(XMLoadFloat3((_float3*)&mWorldMatrix.m[state][0]));
}

_float3 CTransform::GetScaleXYZ() const
{
	_float3 fSizeX, fSizeY, fSizeZ;

	XMStoreFloat3(&fSizeX, XMVector3Length(XMLoadFloat3((_float3*)&mWorldMatrix.m[0][0])));
	XMStoreFloat3(&fSizeY, XMVector3Length(XMLoadFloat3((_float3*)&mWorldMatrix.m[1][0])));
	XMStoreFloat3(&fSizeZ, XMVector3Length(XMLoadFloat3((_float3*)&mWorldMatrix.m[2][0])));

	return _float3(fSizeX.x, fSizeY.x, fSizeZ.x);
}

void CTransform::SetState(E_STATE state, _fvector vec)
{
	XMStoreFloat4((_float4*)&mWorldMatrix.m[state][0], vec);
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	XMStoreFloat4x4(&mWorldMatrix, XMMatrixIdentity());
	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&mDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

HRESULT CTransform::Bind_OnShader(CShader * pShader, const char * pValueName)
{
	// 셰이더로 넘기기위해 전치 행렬을 만든다.
	// 만드는 과정은 연산을 하기 때문에 SIMD 전용 연산으로 바꿔준다.

	_float4x4		WorldMatrix;

	// SIMD 연산용 자료형을 다시 일반 자료형으로 변환해서 대입
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&mWorldMatrix)));

	return pShader->Set_RawValue(pValueName, &WorldMatrix, sizeof(_float4x4));
}

void CTransform::SetTransformDesc(const TRANSFORMDESC& desc)
{
	mDesc = desc;
}

HRESULT CTransform::GO_Straight(_double deltatime)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vLook = GetState(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * mDesc.SpeedPersec * (_float)deltatime;

	SetState(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_Backward(_double deltatime)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vLook = GetState(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * mDesc.SpeedPersec * (_float)deltatime;

	SetState(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_Up(_double deltatime)
{

	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vUp = GetState(CTransform::STATE_UP);

	vPosition += XMVector3Normalize(vUp) * mDesc.SpeedPersec * (_float)deltatime;

	SetState(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_Down(_double deltatime)
{

	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vUp = GetState(CTransform::STATE_UP);

	vPosition -= XMVector3Normalize(vUp) * mDesc.SpeedPersec * (_float)deltatime;

	SetState(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_Right(_double deltatime)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vRight = GetState(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * mDesc.SpeedPersec * (_float)deltatime;

	SetState(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_Left(_double deltatime)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vLeft = GetState(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vLeft) * mDesc.SpeedPersec * (_float)deltatime;

	SetState(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_WorldVec(_float3 vec, _double deltatime)
{

	_float3		vPosition = GetState(CTransform::STATE_POSITION);

	vec.Normalize();
	vPosition += vec * mDesc.SpeedPersec *(_float)deltatime;

	_float4 vPos = _float4(vPosition, 1.f);

	SetState(CTransform::STATE_POSITION, vPos);

	return S_OK;
}

HRESULT CTransform::GO_WorldVec(_float3 vec, _float Angle, E_ROTTYPE type, _double deltatime)
{

	_float3		vPosition = GetState(CTransform::STATE_POSITION);
	vec.Normalize();

	_float4x4 RotMat;

	switch (type)
	{
	case CTransform::ROTTYPE_X:
		RotMat = _float4x4::CreateRotationX(XMConvertToRadians(Angle));
		break;
	case CTransform::ROTTYPE_Y:
		RotMat = _float4x4::CreateRotationY(XMConvertToRadians(Angle));
		break;
	case CTransform::ROTTYPE_Z:
		RotMat = _float4x4::CreateRotationZ(XMConvertToRadians(Angle));
		break;
	default:
		break;

	}

	_float4 newVec = _float4(vec,1.f); 
	_float4::Transform(newVec, RotMat, newVec);

	vPosition += newVec.Get_Float3() * mDesc.SpeedPersec *(_float)deltatime;


	SetState(CTransform::STATE_POSITION, _float4(vPosition, 1.f));

	return S_OK;
}


HRESULT CTransform::Turn(_fvector vAxis, _double time)
{
	_vector		vRight = GetState(CTransform::STATE_RIGHT);
	_vector		vUp = GetState(CTransform::STATE_UP);
	_vector		vLook = GetState(CTransform::STATE_LOOK);

	// vRight = XMVectorSetW(vRight, 0.f);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, mDesc.RotPersec * (_float)time);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	SetState(CTransform::STATE_RIGHT, vRight);
	SetState(CTransform::STATE_UP, vUp);
	SetState(CTransform::STATE_LOOK, vLook);
	return S_OK;
}

HRESULT CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(GetState(CTransform::STATE_RIGHT)));
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(GetState(CTransform::STATE_UP)));
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(GetState(CTransform::STATE_LOOK)));

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	SetState(CTransform::STATE_RIGHT, vRight);
	SetState(CTransform::STATE_UP, vUp);
	SetState(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Chase(_fvector TargetPos, _double time)
{
	return S_OK;
}

HRESULT CTransform::LookAt(_fvector TargetPos, _double time)
{
	return S_OK;
}

HRESULT CTransform::Scaled(_fvector scale)
{
	_vector vRight = GetState(STATE_RIGHT);
	_vector vUp = GetState(STATE_UP);
	_vector vLook = GetState(STATE_LOOK);

	// 크기 재설정
	vRight = XMVector3Normalize(vRight)* XMVectorGetX(scale);
	vUp = XMVector3Normalize(vUp)* XMVectorGetY(scale);
	vLook = XMVector3Normalize(vLook)* XMVectorGetZ(scale);

	SetState(STATE_RIGHT, vRight);
	SetState(STATE_UP, vUp);
	SetState(STATE_LOOK, vLook);
	return S_OK;
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTransform*	pInstance = NEW CTransform(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = NEW CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}