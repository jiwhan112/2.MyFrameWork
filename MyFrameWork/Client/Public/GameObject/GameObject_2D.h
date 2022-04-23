#pragma once

#include "GameObject_Base.h"

BEGIN(Client)

// 2D 오브젝트용 부모클래스
class CGameObject_2D :
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
	CTexture_map* Get_TextureMap() const { return mComTexture; }	
	CVIBuffer* Get_VIBuffer() const { return mComVIBuffer; }

	void Set_LoadUIDesc(const UIDESC& desc)
	{
		memcpy(&mUiDesc, &desc, sizeof(UIDESC));
	}
	const UIDESC& Get_UIDesc() const { return mUiDesc; }

	void Set_LoadTexDesc(const TEXTUREDESC& desc);	
	const TEXTUREDESC& Get_TextureDesc() const { return mTexStrDESC; }

	// 오름차순으로 정렬 큰값을 나중에 그린다.
	virtual const _int& Get_Depth() override { return mUiDesc.mDepth; }

protected:
	virtual HRESULT Set_Component()override;

	// 셰이더 넘기기
	virtual HRESULT Set_ConstantTable_UI(); // UI Cam
	virtual HRESULT Set_ConstantTable_Tex(); // 텍스처 설정


protected: // UI에서 Com / DESC 추가

	CVIBuffer*		mComVIBuffer = nullptr;
	CTexture_map*	mComTexture = nullptr;

	UIDESC			mUiDesc;
	TEXTUREDESC		mTexStrDESC;

public:

	static CGameObject_2D* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_2D* Clone(void* pArg);
	virtual void Free() override;
};

END