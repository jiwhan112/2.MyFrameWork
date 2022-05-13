#pragma once

#include "GameObject_3D_Static.h"

BEGIN(Client)
// 자식이 있는 오브젝트
class CGameObject_3D_Static2  final:
	public CGameObject_Base
{
public:
	// 업데이트 모드
	enum E_UPDATETYPE
	{
		// 기본
		E_UPDATETYPE_NONE,

		// 마우스 피킹시 
		E_UPDATETYPE_PICK,
		E_UPDATETYPE_END
	};

protected:
	explicit CGameObject_3D_Static2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Static2(const CGameObject_3D_Static2& rhs);
	virtual ~CGameObject_3D_Static2() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Add_StaticObejct(CGameObject_3D_Static* obj);
	HRESULT Delefe_StaticObejct(int index);
	void	Set_UpdateMode(E_UPDATETYPE mode) { meUpdateType = mode; }


public:
	CGameObject_3D_Static* Get_ChildOfIndex(_uint index) const;
	const vector<CGameObject_3D_Static*>* Get_ChildList() const { return  &mVecChildObject; };

	HRESULT ReleaseModelVec();

protected:
	virtual HRESULT Set_Component()override;
	HRESULT Render_Child();
	HRESULT Tick_Child(_double time);
	HRESULT LateTick_Child(_double time);

private:
	// 자식객체로 사용
	vector<CGameObject_3D_Static*>		mVecChildObject;
	E_UPDATETYPE						meUpdateType = E_UPDATETYPE_NONE;


public:
	static CGameObject_3D_Static2* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Static2* Clone(void* pArg);
	virtual void Free() override;
};

END

