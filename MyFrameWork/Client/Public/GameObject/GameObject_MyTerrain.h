#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CVIBuffer;
END

BEGIN(Client)

// 3D 오브젝트 
class CGameObject_MyTerrain final :
	public CGameObject_Base
{
protected:
	explicit CGameObject_MyTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_MyTerrain(const CGameObject_MyTerrain& rhs);
	virtual ~CGameObject_MyTerrain() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Set_Component()override;

	virtual HRESULT Set_ConstantTable_Tex(); // 텍스처

private:
	CVIBuffer*		mComVIBuffer = nullptr;


public:
	static CGameObject_MyTerrain* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_MyTerrain* Clone(void* pArg);
	virtual void Free() override;
};

END