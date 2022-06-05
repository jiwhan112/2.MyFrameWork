#pragma once

#include "GameObject_3D_Dynamic.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

// 마인 AI 설정
class CGameObject_Mine final :
	public CGameObject_3D_Dynamic
{
public:

protected:
	explicit CGameObject_Mine(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Mine(const CGameObject_Mine& rhs);
	virtual ~CGameObject_Mine() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	//virtual _int Tick(_double TimeDelta);
	//virtual _int LateTick(_double TimeDelta);
	//virtual HRESULT Render();

	virtual HRESULT Init_Unit();
	virtual HRESULT Init_AI();

	// 모델 구현 
	void Set_Dig_Tile(class CGameObject_3D_Tile* tile);
	void Set_Dig_Gold(class CGameObject_3D_Tile* tile);



protected:

public:
	static CGameObject_Mine* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_Mine* Clone(void* pArg);
	virtual void Free() override;
};

END