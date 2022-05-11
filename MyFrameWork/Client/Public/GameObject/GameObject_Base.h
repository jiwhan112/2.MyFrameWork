#pragma once

#include "GameObject.h"

// 2D�� �ش��ϴ� UI / ��ƼŬ�� ����� ���� ������Ʈ

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer;
//class CTexture;
class CTexture_map;
END

BEGIN(Client)

// Ŭ���̾�Ʈ���� ����ϴ� �⺻ ���� ������Ʈ
// ������Ʈ ��ȯ / �⺻ ������Ʈ ���� / �⺻ ��� ����
class CGameObject_Base abstract :
	public CGameObject
{
protected:
	explicit CGameObject_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Base(const CGameObject_Base& rhs);
	virtual ~CGameObject_Base() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	bool IsName(const char* name)
	{
		// �̸� �ִ��� üũ
		if (2 < strlen(name))
			return true;
		return false;
	}
	void Set_RenderPass(int i)
	{
		mCurrentShaderPass = i;
	}


public: // ������Ʈ ��ȯ
	CShader*		Get_ComShader() { return mComShader; }
	CRenderer*		Get_ComRenderer() { return mComRenderer; }
	E_OBJECT_TYPE	Get_ObjectTypeID_Client() { return (E_OBJECT_TYPE)mObjectTypeid; }

protected:
	// ���� ���̴� ����
	virtual HRESULT Set_ConstantTable_World();
	virtual HRESULT Set_ConstantTable_Light(_uint lightid = 0);

	//	bool PickObject_3D();

protected:
	_uint			mCurrentShaderPass = 0;

	// ��� Ŭ���̾�Ʈ ������Ʈ �⺻ ������Ʈ
	CShader*		mComShader = nullptr;
	CRenderer*		mComRenderer = nullptr;

public:
	virtual CGameObject_Base* Clone(void* pArg)PURE;
	virtual void Free() override;
};

END