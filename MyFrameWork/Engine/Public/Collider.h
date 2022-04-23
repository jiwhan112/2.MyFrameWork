#pragma once

#include "Component.h"

BEGIN(Engine)

// �浹ü ������Ʈ

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum E_COLLIDER_TYPE { COL_AABB, COL_OBB, COL_SPHERE, COL_END };

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

public:
	void SetScale(_float3 size);


#ifdef _DEBUG
public:
	virtual HRESULT Render(class CTransform* trnas);
#endif // _DEBUG

	
private: // �浹ü ���� 
	// �浹ü�� ���� ó��
	BoundingBox*				mAABB = nullptr;
	BoundingOrientedBox*		mOBB = nullptr;
	BoundingSphere*				mSphere = nullptr;
	
	// Ÿ�԰� DESC
	E_COLLIDER_TYPE				meType = COL_END;
	COLLIDERDESC				mColliderDesc;
	

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