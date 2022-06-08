#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum E_STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
	enum E_ROTTYPE
	{
		ROTTYPE_X, ROTTYPE_Y, ROTTYPE_Z, ROTTYPE_END
	};

public:
	typedef struct tag_TransformDESC
	{
		tag_TransformDESC()
		{
			SpeedPersec = 1.0f;
			RotPersec=1.0f;
			ScalePersec=1.0f;
		}
		_float SpeedPersec;
		_float RotPersec;
		_float ScalePersec;
}TRANSFORMDESC;

public:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector GetState(E_STATE state) const;
	_vector GetScale(E_STATE state) const;
	_float3 Get_Scale();

	_float3 GetScaleXYZ() const;
	// 연산용 데이터
	_matrix GetWorldMatrix() const { return XMLoadFloat4x4(&mWorldMatrix); }

	// 저장용 데이터
	_float4x4 GetWorldFloat4x4() const { return mWorldMatrix; }
	_float4x4 GetInvert() const
	{
		_float4x4 invmat;
		mWorldMatrix.Invert(invmat);
		return invmat;
	}

	_float4x4 GetWorldLocalFloat4x4() const { return mLocalMatrix; }


public:
	// 인자로 들어올때의 타입 1~3 fvecotr
	void Set_State(E_STATE state, _fvector vec);

	void Set_WorldMat(_fmatrix worldmat)
	{
		mWorldMatrix = worldmat;
	}

	void Set_State_Local(E_STATE state, _fvector vec);

	void Set_WorldMat_Local(_fmatrix worldmat)
	{
		mLocalMatrix = worldmat;
	}
	void Set_LoadTransDesc(_float speed, _float rot, _float scale)
	{
		mDesc.SpeedPersec = speed;
		mDesc.RotPersec = rot;
		mDesc.ScalePersec = scale;
	}
	void Set_MoveSpeed(_float fMoveSpeed) { mDesc.SpeedPersec = fMoveSpeed; };
	void Set_RotSpeed(_float fMoveSpeed) { mDesc.RotPersec= fMoveSpeed; };
	void Set_ScaleSpeed(_float fMoveSpeed) { mDesc.ScalePersec = fMoveSpeed; };




public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	// 바인딩
	HRESULT Bind_OnShader(class CShader* pShader, const char* pValueName);
	void SetTransformDesc(const TRANSFORMDESC& desc);

	HRESULT UpdateMatrix(_float4x4 ParentMat); // 부모 행렬을 곱할 때 사용


public:

	// Move
	HRESULT GO_Straight(_double deltatime);
	HRESULT GO_Left(_double deltatime);
	HRESULT GO_Right(_double deltatime);
	HRESULT GO_Backward(_double deltatime);
	HRESULT GO_Up(_double deltatime);
	HRESULT GO_Down(_double deltatime);
	HRESULT GO_WorldVec(_float3 vec, _double deltatime);
	HRESULT GO_WorldVec(_float3 vec, _float Angle, E_ROTTYPE type, _double deltatime);

	void MovetoDir(_float3 vDir, _double fDeltaTime);
	void MovetoTarget(_float3 vTarget, _double fDeltaTime);


	// Rot
	HRESULT Turn(_fvector vAxis,_double time);
	void Turn_CW(_float3 vAxis, _float fDeltaTime  ,_float speed);
	void Turn_CCW(_float3 vAxis, _float fDeltaTime ,_float speed);

	HRESULT Rotation(_fvector vAxis, _float fRadian);
	HRESULT Rotation_Add(_fvector vAxis, _float fRadian);

	//void Turn_CW(_float3 vAxis, _double fDeltaTime);
	//void Turn_CCW(_float3 vAxis, _double fDeltaTime);
	//void Rotation_CW(_float3 vAxis, _double fRadian);
	//void Rotation_CCW(_float3 vAxis, _double fRadian);


	HRESULT Chase(_fvector TargetPos, _double time);
	HRESULT LookAt(_fvector TargetPos, _double time, _float speed = 1);
	HRESULT LookAtY(_fvector TargetPos, _double time, _float speed = 1);
	HRESULT LookAt(_fvector TargetPos);
	HRESULT LookAtDir(_float3 Dir);

	// scaled
	HRESULT Scaled(_fvector scale);
	void Scaling(_float3 vScale, _double fDeltaTime);


private:
	TRANSFORMDESC	mDesc;

	_float4x4		mLocalMatrix; // 자식 행렬

	_float4x4		mWorldMatrix; // 최종 행렬


public:
	static CTransform* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END