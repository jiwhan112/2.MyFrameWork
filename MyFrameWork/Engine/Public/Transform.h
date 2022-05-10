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
	_float3 GetScaleXYZ() const;
	// ����� ������
	_matrix GetWorldMatrix() const { return XMLoadFloat4x4(&mWorldMatrix); }

	// ����� ������
	_float4x4 GetWorldFloat4x4() const { return mWorldMatrix; }
	_float4x4 GetInvert() const { 
		_float4x4 invmat;  
		mWorldMatrix.Invert(invmat);
		return invmat;
	}

public:
	// ���ڷ� ���ö��� Ÿ�� 1~3 fvecotr
	void Set_State(E_STATE state, _fvector vec);

	void Set_WorldMat(_fmatrix worldmat)
	{
		mWorldMatrix = worldmat;
	}

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	// ���ε�
	HRESULT Bind_OnShader(class CShader* pShader, const char* pValueName);
	void SetTransformDesc(const TRANSFORMDESC& desc);

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

	// Rot
	HRESULT Turn(_fvector vAxis,_double time);
	HRESULT Rotation(_fvector vAxis,_float fRadian);

	HRESULT Chase(_fvector TargetPos, _double time);
	HRESULT LookAt(_fvector TargetPos, _double time);
	HRESULT LookAt(_fvector TargetPos);

	// Scale
//	HRESULT Scaling(_fvector scale);
	HRESULT Scaled(_fvector scale);

private:
	TRANSFORMDESC	mDesc;
	_float4x4		mWorldMatrix;

public:
	static CTransform* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END