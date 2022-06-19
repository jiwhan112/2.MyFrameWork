#pragma once

#include "Base.h"

BEGIN(Engine)
class CCoillider;
class CNavigation;
END
BEGIN(Client)

class CGameObject_Base;
// 콜라이더를 넣어서 관리한다. 각 충돌 옵션에 맞게 처리한다.
// 
class CColliderManager final :
	public CBase
{
public:
	enum E_COLLIDEROBJ_TYPE
	{
		COLLIDEROBJ_STATIC, COLLIDEROBJ_DYNAMIC, COLLIDEROBJ_TERRAIN, COLLIDEROBJ_UI,COLLIDEROBJ_END
	};

private:
	explicit CColliderManager();
	virtual ~CColliderManager() = default;

public:
	HRESULT NativeConstruct_Prototype();
	HRESULT Tick_ColliderCheck(_double Timer);

public:
	const _float3& Get_PickPos()const { return mWorldPickPos; }
	const _bool& Get_IsMousePick_Object()const { return mIsMousePick_Object; }
	const _bool& Get_IsMousePick_Terrain()const { return mIsMousePick_Terrain; }
	const _ray& Get_MouseRay()const { return mWorldRay; }

public:
	HRESULT Add_ColliderObject(E_COLLIDEROBJ_TYPE e, CGameObject_Base* col);
	HRESULT ReleaseObjects();
	
private:	
	HRESULT ColCheck_OBJECTS(_double timer);
	bool	ColCheck_MOUSE_Object_UI();

	bool	ColCheck_MOUSE_Object();
	bool	ColCheck_MOUSE_Terrain();

	bool Check_Navi(_float4x4 worldinv,CNavigation* navi);

	bool Check_Mouse_Terrain();
	bool Check_Mouse_Object(_ray MouseWorldRay);

	bool ColCheck_Dynamic(CCollider* a, CCollider* b);

private:
	list<CGameObject_Base*> mListColliders[E_COLLIDEROBJ_TYPE::COLLIDEROBJ_END];

	_float3		mWorldPickPos;
	_ray		mWorldRay;
	_float2		mWindowPos;

	_bool		mIsMousePick_Terrain = false;
	_bool		mIsMousePick_Object = false;


public:
	static CColliderManager* Create();
	virtual void Free() override;
};
END
