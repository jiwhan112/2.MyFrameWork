#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CVIBuffer_Rect;

END

BEGIN(Client)

// 안쓸듯
class GameObject_2D_UI:
	public CGameObject_Base
{

protected:
	explicit GameObject_2D_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit GameObject_2D_UI(const GameObject_2D_UI& rhs);
	virtual ~GameObject_2D_UI() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Set_Component()override;
	virtual HRESULT Set_ConstantTable_Texture();


protected:
	// 인스턴싱 버퍼
	// 인스터싱 셰이더

	CVIBuffer_Rect*					mComVIBuffer = nullptr;
	CTexture_map*					mComTexture = nullptr;
	string							mStr_UITex;


public:
	static GameObject_2D_UI* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual GameObject_2D_UI* Clone(void* pArg);
	virtual void Free() override;
};

END