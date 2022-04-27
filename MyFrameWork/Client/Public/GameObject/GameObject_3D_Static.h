#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CModel;
END

// �⺻ 3D ������Ʈ ���
BEGIN(Client)
class CGameObject_3D_Static final:
	public CGameObject_Base
{
protected:
	explicit CGameObject_3D_Static(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Static(const CGameObject_3D_Static& rhs);
	virtual ~CGameObject_3D_Static() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:

	CModel*		Get_ComModel() const { return mComModel; }
	const MODEL_STATIC_DESC& Get_ModelDESC() const { return mModelDesc; }

	HRESULT		Set_LoadModelDESC(const MODEL_STATIC_DESC& desc);

protected:
	virtual HRESULT Set_Component()override;

	virtual HRESULT Set_ConstantTable_Model(); // �� ����
//	virtual HRESULT Set_ConstantTable_Light(_uint lightid = 0); // ������



protected: // 3D�� Com / DESC �߰�
	CModel*						mComModel = nullptr;
	MODEL_STATIC_DESC			mModelDesc;

public:
	static CGameObject_3D_Static* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Static* Clone(void* pArg);
	virtual void Free() override;
};

END