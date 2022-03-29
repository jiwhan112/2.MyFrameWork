#pragma once

#include "GameObject.h"
BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CGameObject_BackGround final:
	public CGameObject
{
protected:
	explicit CGameObject_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_BackGround(const CGameObject_BackGround& rhs);
	virtual ~CGameObject_BackGround() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Set_Component();

private:
	_float4x4			mProjMatrix;
	_float2				mPos;
	_float2				mSize;

private:
	CShader*			mComShader = nullptr;
	CRenderer*			mComRenderer = nullptr;
	CVIBuffer_Rect*		mComVIBuffer = nullptr;
	CTexture*			mComTexture = nullptr;

public:
	static CGameObject_BackGround* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_BackGround* Clone(void* pArg);
	virtual void Free() override;
};

END