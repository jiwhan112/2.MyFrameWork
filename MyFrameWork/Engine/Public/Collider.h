#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

public:
	typedef struct tagColliderDesc
	{
		_float3 vScale; /* ��� */
		_float4 vRotation;
		_float4 vTranslation;
	} COLLIDERDESC;
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg);

#ifdef _DEBUG
public:
	virtual HRESULT Render();
#endif // _DEBUG

private:
	BoundingBox*				m_pAABB = nullptr;
	BoundingOrientedBox*		m_pOBB = nullptr;
	BoundingSphere*				m_pSphere = nullptr;
	TYPE						m_eType = TYPE_END;
	COLLIDERDESC				m_ColliderDesc;
	

#ifdef _DEBUG
private:
	/* �������� �׸��� �ִ� ���̴��� �����Ѵ�. */
	BasicEffect*									m_pBasicEffect = nullptr;

	ID3D11InputLayout*								m_pInputLayout = nullptr;

	/* �⺻���� ������ �׷������� �ִ� ���� ����, �ε��� ���۸� �����ϳ�. */
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pBatch = nullptr;
#endif // _DEBUG




public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END