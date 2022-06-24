#pragma once

#include "Component.h"

BEGIN(Engine)

// �浹ü ������Ʈ

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
	// �浹ü�� ��ġ ����
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
	virtual bool ColliderCheck(CCollider* TargetCollider);
	virtual bool ColliderCheck(_ray worldDIr, _float& dist);
	virtual bool ColliderCheck(_float3 point1, _float3 point2, _float3 point3);

	_bool Get_IsCollision()const { return mIsCollision; }
public:
	void Set_Scale(_float3 size);
	void Set_Offset(_float3 offset);
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

	bool Update_AABB(_ray RatDIr, _float& dist);
	bool Update_OBB(_ray RatDIr, _float& dist);
	bool Update_SPHERE(_ray RatDIr, _float& dist);

	bool Update_MY_AABB(CCollider* TargetCollider);
	bool Update_MY_OBB(CCollider* TargetCollider);


private: // �浹ü ����
	// �浹ü�� ���� ó��
	BoundingBox*				mAABB = nullptr;
	BoundingOrientedBox*		mOBB = nullptr;
	BoundingSphere*				mSphere = nullptr;
	
	// Ÿ�԰� DESC
	E_COLLIDER_TYPE				meType = COL_END;
	COLLIDERDESC				mColliderDesc;

	_float3						mOffset = _float3(0,0,0);
	_bool						mIsCollision = false;

	// Draw ����
#ifdef _DEBUG

private:
	// �⺻ ���̴� ����
	BasicEffect*									mBaseEffect = nullptr;
	// ���̴��� �ʿ��� ���̾ƿ�
	ID3D11InputLayout*								mInputLayout = nullptr;

	// �⺻���� <��������> ����
	PrimitiveBatch<DirectX::VertexPositionColor>*	mBatch = nullptr;
#endif // _DEBUG


public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, E_COLLIDER_TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END