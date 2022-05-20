#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CModel;
END

// �ִϸ��̼� 3D ������Ʈ ���
BEGIN(Client)

class CGameObject_3D_Dynamic  :
	public CGameObject_Base
{
public:
	//enum E_BASEAI
	//{
	//	BASEAI_IDLE,
	//	BASEAI_MOVE,
	//	BASEAI_END,
	//};

protected:
	explicit CGameObject_3D_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Dynamic(const CGameObject_3D_Dynamic& rhs);
	virtual ~CGameObject_3D_Dynamic() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	CModel*						Get_ComModel() const { return mComModel; }
	const list<CCollider*>*		Get_ComListCollider() const { return mComListCollider; }
	CNavigation*				Get_ComNaviMesh() const { return mComNaviMesh; }

	const MODEL_DYNAMIC_DESC&	Get_ModelDESC() const { return mModelDesc; }

	HRESULT		Set_LoadModelDynamicDESC(const MODEL_DYNAMIC_DESC& desc);
	HRESULT		Setup_Colluder2(_float4x4 worldmat, _float scale);

	HRESULT		Add_ColliderDesc(COLLIDER_DESC desc);
	HRESULT		Add_ColliderDesc(COLLIDER_DESC* desc,int size);

public:
	virtual HRESULT CollisionFunc(_float3 PickPosition, _float dist, _uint ColliderIndex);


protected:
	virtual HRESULT Set_Component() override;
	HRESULT Update_Collider();


//	void		GOMOVE(_double timer);

protected: // 3D�� Com / DESC �߰�
	CModel*						mComModel = nullptr;

	// �ݶ��̴� ����Ʈ �߰� / �ݶ��̴��� �ʿ� ���� �ʰ� ���⼭�� �����Ѵ�.
	list<CCollider*>*			mComListCollider = nullptr;

	// ������ ������ Terrain ������ �־���Ѵ�.
//	CGameObject_Base*			mGameObjectTerrain = nullptr;
	CNavigation*				mComNaviMesh = nullptr;


	MODEL_DYNAMIC_DESC			mModelDesc;
	list<COLLIDER_DESC>			mListColliderDesc;

	_float3						mGoalPosition;
	_float3						mStartPosition;
	_double						mTimer;
	_double						mTimeMax;
	bool						mIsMove = false;
//	E_BASEAI					meAI;

public:
	static CGameObject_3D_Dynamic* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Dynamic* Clone(void* pArg);
	virtual void Free() override;
};

END