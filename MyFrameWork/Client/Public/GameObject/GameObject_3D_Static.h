#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CModel;
END

// 기본 3D 오브젝트 출력
BEGIN(Client)
class CGameObject_3D_Static final:
	public CGameObject_Base
{
protected:
	explicit CGameObject_3D_Static(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Static(const CGameObject_3D_Static& rhs);
	virtual ~CGameObject_3D_Static() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	CModel*		Get_ComModel() const { return mComModel; }

protected:
	virtual HRESULT Set_Component()override;

private:
	HRESULT Set_ConstantTable_World();
	HRESULT Set_ConstantTable_Tex(_uint texid=0);

private:
	CModel*						mComModel = nullptr;

protected: // DESC

	// 모델 컴포넌트는 FBX 모델 이름으로 생성
//	MODUEL_STATIC_DESC			mModelDesc;



public:
	static CGameObject_3D_Static* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Static* Clone(void* pArg);
	virtual void Free() override;
};

END