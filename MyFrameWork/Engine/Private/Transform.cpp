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

_float3 CTransform::Get_Scale()
{
	_float3 r = mWorldMatrix.Right();
	_float3 u = mWorldMatrix.Up();
	_float3 l = mWorldMatrix.Backward();
	_float3 length = _float3(r.Length(), u.Length(), l.Length());
	return length;
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

HRESULT CTransform::UpdateMatrix(_float4x4 ParentMat)
{
	// 무보 행렬이 있다면 연산을 해서 최종 상태 행렬을 만든다.
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

void CTransform::MovetoDir(_float3 vDir, _double fDeltaTime)
{
	if (vDir == _float3(0, 0, 0))
		return;

	_float3 vPos = GetState(STATE_POSITION);

	vDir.Normalize();
	vPos += vDir * mDesc.SpeedPersec * fDeltaTime;

	Set_State(STATE_POSITION, vPos);
}

void CTransform::MovetoDir(_float3 vDir, _float Speed, _double fDeltaTime)
{
	if (vDir == _float3(0, 0, 0))
		return;

	_float3 vPos = GetState(STATE_POSITION);

	vDir.Normalize();
	vPos += vDir * Speed * fDeltaTime;

	Set_State(STATE_POSITION, vPos);
}



void CTransform::MovetoTarget(_float3 vTarget, _double fDeltaTime)
{
	_float3 vPos = GetState(STATE_POSITION);
	_float3 vTargetNormal = vTarget - vPos;
	vTargetNormal.Normalize();
	vPos += vTargetNormal * mDesc.SpeedPersec * fDeltaTime;

	Set_State(STATE_POSITION, vPos);
}

void CTransform::Set_Rotate(_float3 rot)
{
	_quaterion quat_x, quat_y, quat_z, quat;
	_float4x4 matrix;
	_float3 axis_x(1.0f, 0.0f, 0.0f);
	_float3 axis_y(0.0f, 1.0f, 0.0f);
	_float3 axis_z(0.0f, 0.0f, 1.0f);

	// XMQuaternionRotationAxis();
	quat_x = _quaterion::CreateFromAxisAngle(axis_x, rot.x);
	quat_y = _quaterion::CreateFromAxisAngle(axis_y, rot.y);
	quat_z = _quaterion::CreateFromAxisAngle(axis_z, rot.z);

	quat = quat_y * quat_x * quat_z;
	quat.Normalize();
	quat.CreateFromRotationMatrix(matrix);
	matrix = _float4x4::CreateFromQuaternion(quat);

	Set_State(CTransform::STATE_RIGHT, *(_float3*)&matrix.m[0]);
	Set_State(CTransform::STATE_UP, *(_float3*)&matrix.m[1]);
	Set_State(CTransform::STATE_LOOK, *(_float3*)&matrix.m[2]);
}
void CTransform::Set_Rotate(const _float4x4& rotmat)
{
	Set_State(CTransform::STATE_RIGHT, *(_float3*)&rotmat.m[0]);
	Set_State(CTransform::STATE_UP, *(_float3*)&rotmat.m[1]);
	Set_State(CTransform::STATE_LOOK, *(_float3*)&rotmat.m[2]);
}



_quaterion CTransform::Get_Quaterion()
{
	_quaterion quter = _quaterion::CreateFromRotationMatrix(mWorldMatrix);
	return quter;

}

_quaterion CTransform::Get_DegreeFormQuater(_float3 rot)
{
	_quaterion quat_x, quat_y, quat_z, quat;
	_float4x4 matrix;
	_float3 axis_x(1.0f, 0.0f, 0.0f);
	_float3 axis_y(0.0f, 1.0f, 0.0f);
	_float3 axis_z(0.0f, 0.0f, 1.0f);

	// XMQuaternionRotationAxis();
	quat_x = _quaterion::CreateFromAxisAngle(axis_x, rot.x);
	quat_y = _quaterion::CreateFromAxisAngle(axis_y, rot.y);
	quat_z = _quaterion::CreateFromAxisAngle(axis_z, rot.z);

	quat = quat_y * quat_x * quat_z;
	quat.Normalize();
	return quat;

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

void CTransform::Turn_CW(_float3 vAxis, _float fDeltaTime,_float Speed)
{
	_vector		vRight = GetState(CTransform::STATE_RIGHT);
	_vector		vUp = GetState(CTransform::STATE_UP);
	_vector		vLook = GetState(CTransform::STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, Speed * fDeltaTime);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Turn_CCW(_float3 vAxis, _float fDeltaTime,_float speed)
{
	Turn_CW(vAxis * -1, fDeltaTime, speed);
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

HRESULT CTransform::LookAt(_fvector  targetPos, _double time,_float speed)
{
	_float3 TargetPos = targetPos;
	_float3 vPos = mWorldMatrix.Translation();
	_float3 vScale = GetScaleXYZ();

	_float3 CurrentLook = GetState(CTransform::STATE_LOOK);
	CurrentLook.Normalize();
	_float3 TargetLook = TargetPos - vPos;
	TargetLook.Normalize();

	_float3 vLook, vRight, vUp;
	vLook = _float3::Lerp(CurrentLook, TargetLook, time*speed);
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

HRESULT CTransform::LookAt_Tick(_double Deltatime, _float speed)
{
	//if (mIsRotating)
	//{
	//	
	//	
	//	// 타겟과의 앵글을 구해서 Y축 회전 수행
	//	_float3 vPos = mWorldMatrix.Translation();
	//	vPos.y = 0;
	//	mTargetPos.y = 0;


	//	_float3 TargetDir = mTargetPos - vPos;
	//	TargetDir.Normalize();

	//	Turn_CW(_float3::Up, Deltatime,10);

	//	_float3 look =  GetState(CTransform::STATE_LOOK);
	//	look.y = 0;
	//	look.Normalize();

	//	float dot = look.Dot(TargetDir);
	//	if(dot>0.8f)
	//		mIsRotating = false;




	//	//TargetDir.Normalize(); // Look
	//	//_float3 MyLook = GetState(CTransform::STATE_LOOK);
	//	//_float theta = MyLook.Dot(TargetDir);
	//	//if (theta > 0.8f)
	//	//{
	//	//	mIsRotating = false;
	//	//	return S_OK;
	//	//}

	//	//_double angle =  GetAngleXZ(vPos, mTargetPos);

	//	//Set_Rotate(_float3(0, angle, 0));


	//	

	//	//_float angle = XMConvertToDegrees(theta);
	//	//mRotTimer += Deltatime;
	//	//if (mRotTimer >= 1.f)
	//	//{
	//	//	mIsRotating = false;
	//	//	mRotTimer = 0;
	//	//	return S_OK;
	//	//}

	//}
	//else
	//{
	//	mRotTimer = 0;
	//}

	//return S_OK;









	


	_float3 TargetPos = mTargetPos;
	TargetPos.y = 0;

	_float3 vPos = mWorldMatrix.Translation();
	vPos.y = 0;

	if (_float3::Distance(vPos, mTargetPos) < 0.2f)
	{
		mIsRotating = false;
		return S_OK;
	}

	_float3 TargetLook = TargetPos - vPos;
	TargetLook.Normalize();

	_float3 CurrentLook = GetState(CTransform::STATE_LOOK);
	CurrentLook.Normalize();
	
	_float lookDot = CurrentLook.Dot(TargetLook);
	if (0.9f < lookDot)
	{
		// 회전 한계
		mIsRotating = false;
		return S_OK;
	}
	else
	{
		// 회전방향
		_float3 CurrentRight = GetState(CTransform::STATE_RIGHT);
		CurrentRight.Normalize();
		_float rightLeft = CurrentRight.Dot(TargetLook);
		
		if (rightLeft > 0)
			Turn_CW(_float3::Up, Deltatime, 10);
		else
			Turn_CCW(_float3::Up, Deltatime, 10);

	//	mIsRotating = false;

		return S_OK;
	}
	return S_OK;
}

double CTransform::GetAngleXZ(_float3 start, _float3 end)
{

	double dz = end.y - start.z;
	double dx = end.x - start.x;

	return atan2(dz, dx) * (180.0 / 3.1415);
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

HRESULT CTransform::LookAtDir(_fvector  Dir)
{
	_float3 vLook, vRight, vUp;
	_float3 vScale = GetScaleXYZ();

	vLook = Dir;
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

HRESULT CTransform::Scaled(_fvector scale)
{
	_vector vRight = GetState(STATE_RIGHT);
	_vector vUp = GetState(STATE_UP);
	_vector vLook = GetState(STATE_LOOK);

	// 크기 재설정
	vRight = XMVector3Normalize(vRight)* XMVectorGetX(scale);
	vUp = XMVector3Normalize(vUp)* XMVectorGetY(scale);
	vLook = XMVector3Normalize(vLook)* XMVectorGetZ(scale);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
	return S_OK;
}

void CTransform::Scaling(_float3 vScale, _double fDeltaTime)
{
	_float3 ScaleXYZ = Get_Scale();
	_float3 FinalScale = (ScaleXYZ + (vScale * fDeltaTime));

	_float3 rNormal = mWorldMatrix.Right();
	rNormal.Normalize();
	_float3 uNormal = mWorldMatrix.Up();
	uNormal.Normalize();
	_float3 lNormal = mWorldMatrix.Backward();
	lNormal.Normalize();

	Set_State(CTransform::STATE_RIGHT, rNormal* FinalScale.x);
	Set_State(CTransform::STATE_UP, uNormal * FinalScale.y);
	Set_State(CTransform::STATE_LOOK, lNormal * FinalScale.z);
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