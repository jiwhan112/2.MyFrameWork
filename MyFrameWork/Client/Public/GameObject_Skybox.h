#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer;
class CTexture;
END

BEGIN(Client)
class CGameObject_Skybox final :
	public CGameObject
{
protected:
	explicit CGameObject_Skybox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Skybox(const CGameObject_Skybox& rhs);
	virtual ~CGameObject_Skybox() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Set_Component() override;
	HRESULT Set_ConstantTable();

private:
	CShader*			mComShader = nullptr;
	CRenderer*			mComRenderer = nullptr;
	CVIBuffer*			mComVIBuffer = nullptr;
	CTexture*			mComTexture = nullptr;

public:
	static CGameObject_Skybox* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_Skybox* Clone(void* pArg);
	virtual void Free() override;
};

END