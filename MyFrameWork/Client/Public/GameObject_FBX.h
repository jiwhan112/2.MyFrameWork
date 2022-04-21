#pragma once

#include "GameObject.h"
BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CTexture;
END

BEGIN(Client)
class CGameObject_FBX final:
	public CGameObject
{
protected:
	explicit CGameObject_FBX(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_FBX(const CGameObject_FBX& rhs);
	virtual ~CGameObject_FBX() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Set_Component() override;
	HRESULT Set_ConstantTable();

private:
	// 모델 텍스처
	CShader*			mComShader = nullptr;
	CRenderer*			mComRenderer = nullptr;
	CModel*				mComModel = nullptr;
	
public:
	static CGameObject_FBX* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_FBX* Clone(void* pArg);
	virtual void Free() override;

};

END