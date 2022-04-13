#pragma once

#include "GameObject_Base.h"

BEGIN(Client)

// 2D 오브젝트용 부모클래스
class CGameObject_2D final:
	public CGameObject_Base
{
protected:
	explicit CGameObject_2D(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_2D(const CGameObject_2D& rhs);
	virtual ~CGameObject_2D() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	void Set_LoadUIDesc(const UIDESC& desc)
	{
		memcpy(&mUiDesc, &desc, sizeof(UIDESC));
	}
	const UIDESC& Get_UIDesc() const { return mUiDesc; }

protected:
	virtual HRESULT Set_Component()override;

private:
	virtual HRESULT Set_ConstantTable_UI();


private:
	UIDESC			mUiDesc;

public:
	static CGameObject_2D* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_2D* Clone(void* pArg);
	virtual void Free() override;
};

END