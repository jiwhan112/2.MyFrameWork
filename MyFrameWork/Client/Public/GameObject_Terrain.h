#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer;
class CTexture;
END

BEGIN(Client)
class CGameObject_Terrain final :
	public CGameObject
{
public:
	enum TEXTURETYPE { TYPE_DIFFUSE, TYPE_FILTER, TYPE_BRUSH, TYPE_END };

protected:
	explicit CGameObject_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Terrain(const CGameObject_Terrain& rhs);
	virtual ~CGameObject_Terrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Set_Component() override;
	HRESULT Set_ConstantTable();
	bool PickObject();

private:
	CShader*			mComShader = nullptr;
	CRenderer*			mComRenderer = nullptr;
	CVIBuffer*			mComVIBuffer = nullptr;
	CTexture*			mComTexture[TYPE_END] = { nullptr };

	// 제작한 텍스처
	ID3D11ShaderResourceView*	mRSV = nullptr;

public:
	HRESULT Create_FilterTexture();


public:
	static CGameObject_Terrain* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_Terrain* Clone(void* pArg);
	virtual void Free() override;
};

END