#pragma once

#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	class CComponent* Get_Component(const _tchar* pComponentTag);

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	class CTransform*	Get_TransformCom() const { return mComTransform; }

	bool Get_IsLife() const { return mIsLife; }
	bool Get_IsRenderer() const { return mIsRenderer; }
	void Set_Dead() { mIsLife = false; }

	void Set_isVisible(bool b) { mIsRenderer = b; }

	const _uint& Get_ObjectTypeID() const
	{
		return mObjectTypeid;
	}

	virtual const _int& Get_Depth() { return 0; };

	//CGameObject* Get_Parent()const
	//{
	//	if (mParrent)
	//		return mParrent;
	//	return nullptr;
	//}

	//void Set_Parent(CGameObject* obj)
	//{
	//	Safe_Release(mParrent);
	//	mParrent = obj;
	//	Safe_AddRef(mParrent);

	//}

	//const list<CGameObject*>* Get_Children() const
	//{
	//	if (mChildren)
	//		return mChildren;
	//	return nullptr;
	//}

	//void Set_AddChild(CGameObject* obj,bool bAddDepth = true)
	//{
	//	if (mChildren == nullptr)
	//		mChildren = NEW list<CGameObject *>;

	//	if (bAddDepth)
	//		obj->Set_Depth(mDepth + 1);

	//	else
	//		obj->Set_Depth(mDepth);

	//	mChildren->push_back(obj);

	//}

	//_uint Get_Depth() const { return mDepth; }
	//void Set_Depth(_uint depth) { mDepth = depth; }

protected:
	virtual HRESULT Set_Component()PURE;

public:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg = nullptr);
	HRESULT Release_Component(const _tchar* pComponentTag);

private:
	//	HRESULT Add_Component_This(const _tchar* pComponentTag, CComponent** ppOut, void* pArg = nullptr);

	protected:
		ID3D11Device*			m_pDevice = nullptr;
		ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	protected:
		class CTransform*		mComTransform = nullptr;
		static const _tchar*	mComTag_Transform;

		// Á×À½ Ã¼Å©¿Í ·»´õ¸µ ¿©ºÎ
		bool				mIsLife = true;
		bool				mIsRenderer = true;
		bool				mIsClone = false;
		_uint				mObjectTypeid = 99;

	protected:
		CComponent*	Find_Component(const _tchar* pComponentTag);

	protected:
		map<const _tchar*, CComponent*>			m_Components;
		typedef map<const _tchar*, CComponent*>	COMPONENTS;

	public:
		virtual CGameObject* Clone(void* pArg) = 0;
		virtual void Free() override;
};

END