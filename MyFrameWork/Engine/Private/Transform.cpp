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

void CTransform::Set_State(E_STATE state, _fvector vec)
{

	XMStoreFloat4((_float4*)&mWorldMatrix.m[state][0], vec);
	if (state == CTransform::STATE_POSITION)
		mWorldMatrix.m[state][3] = 1;
}

void CTransform::Set_State_Local(E_STATE state, _fvector vec)
{
	XMStoreFloat4((_float4*)&mLocalMatrix.m[state][0], vec);
	if (state == CTransform::STATE_POSITION)
		mLocalMatrix.m[state][3] = 1;
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
	// ���̴��� �ѱ������ ��ġ ����� �����.
	// ����� ������ ������ �ϱ� ������ SIMD ���� �������� �ٲ��ش�.

	_float4x4		WorldMatrix;

	// SIMD ����� �ڷ����� �ٽ� �Ϲ� �ڷ������� ��ȯ�ؼ� ����
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&mWorldMatrix)));

	return pShader->Set_RawValue(pValueName, &WorldMatrix, sizeof(_float4x4));
}

void CTransform::SetTransformDesc(const TRANSFORMDESC& desc)
{
	mDesc = desc;
}

HRESULT CTransform::UpdateMatrix(_float4x4 ParentMat)
{
	// ���� ����� �ִٸ� ������ �ؼ� ���� ���� ����� �����.
	mWorldMatrix = mLocalMatrix *  ParentMat;

	return S_OK;
}

HRESULT CTransform::GO_Straight(_double deltatime)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vLook = GetState(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * mDesc.SpeedPersec * (_float)deltatime;

	Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_Backward(_double deltatime)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vLook = GetState(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * mDesc.SpeedPersec * (_float)deltatime;

	Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_Up(_double deltatime)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vUp = GetState(CTransform::STATE_UP);

	vPosition += XMVector3Normalize(vUp) * mDesc.SpeedPersec * (_float)deltatime;

	Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_Down(_double deltatime)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vUp = GetState(CTransform::STATE_UP);

	vPosition -= XMVector3Normalize(vUp) * mDesc.SpeedPersec * (_float)deltatime;

	Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_Right(_double deltatime)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vRight = GetState(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * mDesc.SpeedPersec * (_float)deltatime;

	Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_Left(_double deltatime)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vLeft = GetState(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vLeft) * mDesc.SpeedPersec * (_float)deltatime;

	Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::GO_WorldVec(_float3 vec, _double deltatime)
{
	_float3		vPosition = GetState(CTransform::STATE_POSITION);

	vec.Normalize();
	vPosition += vec * mDesc.SpeedPersec *(_float)deltatime;

	_float4 vPos = vPosition.ToVec4(1);

	Set_State(CTransform::STATE_POSITION, vPos);

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

	_float4 newVec = vec.ToVec4(1);
	_float4::Transform(newVec, RotMat, newVec);

	vPosition += newVec.ToVec3() * mDesc.SpeedPersec *(_float)deltatime;

	Set_State(CTransform::STATE_POSITION, vPosition.ToVec4(1));

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

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
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

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Rotation_Add(_fvector vAxis, _float fRadian)
{
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(GetState(CTransform::STATE_RIGHT)));
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(GetState(CTransform::STATE_UP)));
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(GetState(CTransform::STATE_LOOK)));

	_float4x4		NewMat = mWorldMatrix;

	_float4x4		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);
	NewMat = NewMat * RotationMatrix;

	vRight = XMVector4Transform(vRight, NewMat);
	vUp = XMVector4Transform(vUp, NewMat);
	vLook = XMVector4Transform(vLook, NewMat);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

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

HRESULT CTransform::LookAt(_fvector targetPos)
{
	_float3 TargetPos = targetPos;
	_float3 vPos = mWorldMatrix.Translation();
	_float3 vScale = GetScaleXYZ();

	_float3 vLook, vRight, vUp;

	vLook = TargetPos - vPos;
	vLook.Normalize();
	vLook *= vScale.z;

	if (vLook == _float3(0, 1, 0))
	{
		return S_OK;
	}
	else
	{
		vRight = _float3::Up.Cross(vLook);
		vRight.Normalize();
		vRight *= vScale.x;
	}

	vUp = vLook.Cross(vRight);
	vUp.Normalize();
	vUp *= vScale.y;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::LookAtDir(_float3 Dir)
{
	_float3 vLook, vRight, vUp;
	vLook = Dir;
	vLook.Normalize();

	if (vLook == _float3(0, 1, 0))
	{
		return S_OK;
	}
	else
	{
		vRight = _float3::Up.Cross(vLook);
		vRight.Normalize();
	}

	vUp = vLook.Cross(vRight);
	vUp.Normalize();

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Scaled(_fvector scale)
{
	_vector vRight = GetState(STATE_RIGHT);
	_vector vUp = GetState(STATE_UP);
	_vector vLook = GetState(STATE_LOOK);

	// ũ�� �缳��
	vRight = XMVector3Normalize(vRight)* XMVectorGetX(scale);
	vUp = XMVector3Normalize(vUp)* XMVectorGetY(scale);
	vLook = XMVector3Normalize(vLook)* XMVectorGetZ(scale);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
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