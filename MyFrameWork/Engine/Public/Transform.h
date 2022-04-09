#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum E_STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

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
	// 연산용 데이터
	_matrix GetWorldMatrix() const { return XMLoadFloat4x4(&mWorldMatrix); }

	// 저장용 데이터
	_float4x4 GetWorldFloat4x4() const { return mWorldMatrix; }

public:
	// 인자로 들어올때의 타입 1~3 fvecotr
	void SetState(E_STATE state, _fvector vec);

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	// 바인딩
	HRESULT Bind_OnShader(class CShader* pShader, const char* pValueName);
	void SetTransformDesc(const TRANSFORMDESC& desc);

public:

	// Move
	HRESULT GO_Straight(_double deltatime);
	HRESULT GO_Left(_double deltatime);
	HRESULT GO_Right(_double deltatime);
	HRESULT GO_Backward(_double deltatime);

	// Rot
	HRESULT Turn(_fvector vAxis,_double time);
	HRESULT Rotation(_fvector vAxis,_float fRadian);

	HRESULT Chase(_fvector TargetPos, _double time);
	HRESULT LookAt(_fvector TargetPos, _double time);

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