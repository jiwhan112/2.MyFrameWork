#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CModel;
END

// 애니메이션 3D 오브젝트 출력
BEGIN(Client)

class CGameObject_3D_Dynamic final :
	public CGameObject_Base
{
protected:
	explicit CGameObject_3D_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Dynamic(const CGameObject_3D_Dynamic& rhs);
	virtual ~CGameObject_3D_Dynamic() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:

protected:
	virtual HRESULT Set_Component()override;

private:
	HRESULT Set_ConstantTable_World();
	HRESULT Set_ConstantTable_Tex(_uint texid = 0);

private:
	CModel*						mComModel = nullptr;

protected: // DESC


public:
	static CGameObject_3D_Dynamic* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Dynamic* Clone(void* pArg);
	virtual void Free() override;
};

END