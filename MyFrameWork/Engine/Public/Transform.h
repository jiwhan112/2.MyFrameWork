#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
	typedef struct tag_TransformDESC
	{
		_double mSpeedPersec;
		_double mRotPersec;
		_double mScalePersec;

}TRANSFORMDESC;

public:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

public:
	// ¹ÙÀÎµù

public:

	// Move
	HRESULT GO_Straight(_double deltatime);
	HRESULT GO_Left(_double deltatime);
	HRESULT GO_Right(_double deltatime);
	HRESULT GO_Backward(_double deltatime);

	// Rot
	//HRESULT Turn(_double time);
	//HRESULT Rotation();

	// Scale
	//HRESULT Scaled();
	//HRESULT Scaling();


private:
	TRANSFORMDESC	mDesc;
	_float4x4		mWorldMatrix;



public:
	static CTransform* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END