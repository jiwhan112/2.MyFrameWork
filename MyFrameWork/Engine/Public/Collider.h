#pragma once

#include "Component.h"

BEGIN(Engine)

// 충돌체 컴포넌트

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum E_COLLIDER_TYPE { COL_AABB, COL_OBB, COL_SPHERE, COL_END };

public:
	typedef struct tagOBBDesc
	{
		_float3		vCenterAxis[3];
		_float3		vAlignAxis[3];
	}OBBDESC;

public:
	// 충돌체의 위치 적용
	typedef struct tagColliderDesc
	{
		_float3 vScale; 
		_float4 vRotation;
		_float4 vTranslation;
	} COLLIDERDESC;

private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(E_COLLIDER_TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg);
	virtual HRESULT Update_Transform(_float4x4 TransformMatrix);
	virtual bool Update_Collider(CCollider* TargetCollider);

public:
	void SetScale(_float3 size);
	E_COLLIDER_TYPE Get_Type()
	{
		return meType;
	}

	BoundingBox* Get_Collider_AABB()
	{
		return mAABB;
	}
	BoundingOrientedBox* Get_Collider_OBB()
	{
		return mOBB;
	}
	BoundingSphere* Get_Collider_SPHERE()
	{
		return mSphere;
	}

	OBBDESC Get_Compute_OBBDesc();


#ifdef _DEBUG
public:
	virtual HRESULT Render();
#endif // _DEBUG

private:
	bool Update_AABB(CCollider* TargetCollider);
	bool Update_OBB(CCollider* TargetCollider);
	bool Update_SPHERE(CCollider* TargetCollider);

	bool Update_MY_AABB(CCollider* TargetCollider);
	bool Update_MY_OBB(CCollider* TargetCollider);
	
private: // 충돌체 정보 
	// 충돌체에 따라서 처리
	BoundingBox*				mAABB = nullptr;
	BoundingOrientedBox*		mOBB = nullptr;
	BoundingSphere*				mSphere = nullptr;
	
	// 타입과 DESC
	E_COLLIDER_TYPE				meType = COL_END;
	COLLIDERDESC				mColliderDesc;

	// Draw 관련
#ifdef _DEBUG

private:
	// 기본 셰이더 제공
	BasicEffect*									mBaseEffect = nullptr;
	// 셰이더에 필요한 레이아웃
	ID3D11InputLayout*								mInputLayout = nullptr;

	// 기본적인 <정점버퍼> 제공
	PrimitiveBatch<DirectX::VertexPositionColor>*	mBatch = nullptr;
#endif // _DEBUG

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, E_COLLIDER_TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END