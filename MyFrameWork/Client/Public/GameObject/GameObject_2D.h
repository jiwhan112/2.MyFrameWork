#pragma once

#include "GameObject.h"

// 2D에 해당하는 UI / 파티클을 만들기 위한 오브젝트

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)


class CGameObject_2D final:
	public CGameObject
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

protected:
	virtual HRESULT Set_Component()override;

private:
	HRESULT Set_ConstantTable_UI();
	HRESULT Set_ConstantTable_World();
	HRESULT Set_ConstantTable_Tex(_uint texid);


private:
	UIDESC			mUiDesc;


	// 컴포넌트만 갈아끼우면 이미지가 바뀌게 수정
	CShader*		mComShader		= nullptr;
	CRenderer*		mComRenderer	= nullptr;
	CVIBuffer_Rect*	mComVIBuffer	= nullptr;
	CTexture*		mComTexture		= nullptr;

public:
	static CGameObject_2D* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_2D* Clone(void* pArg);
	virtual void Free() override;
};

END