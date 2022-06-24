#pragma once

#include "GameObject_3D_Static.h"

BEGIN(Engine)
class CModel;
class CCollider;
END

// 환경 오브젝트 랜덤으로 뿌림

BEGIN(Client)
class CGameObject_Environment :
	public CGameObject_3D_Static
{
public:
	// 업데이트 모드
	enum E_ENVIORMENT_TYPE
	{
		E_ENVIOR_TREE_DEL,
		E_ENVIOR_TREE_DEAD,
		E_ENVIOR_MUSH,
		E_ENVIOR_WEED,
		E_ENVIOR_ROCK,
		E_ENVIOR_FLOWER,
		E_ENVIOR_END,
	};

protected:
	explicit CGameObject_Environment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Environment(const CGameObject_Environment& rhs);
	virtual ~CGameObject_Environment() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	//virtual _int Tick(_double TimeDelta);
	//virtual _int LateTick(_double TimeDelta);
	//virtual HRESULT Render();


public:
	// 모델 랜덤 세팅
	HRESULT Environment_RandomSetting(E_ENVIORMENT_TYPE type);

protected:
	virtual HRESULT Set_Component()override;



protected: // 3D모델 Com / DESC 추가
	E_ENVIORMENT_TYPE meEnviorType;
	static vector<string>* mVec_TreeDel_EnviorName;
	static vector<string>* mVec_TreeDead_EnviorName;
	static vector<string>* mVec_Weed_EnviorName;
	static vector<string>* mVec_Mush_EnviorName;

	static vector<string>* mVec_Rock_EnviorName;
	static vector<string>* mVec_Flower_Name;



	E_ENVIORMENT_TYPE meType = E_ENVIOR_TREE_DEL;

public:
	static CGameObject_Environment* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_Environment* Clone(void* pArg);
	virtual void Free() override;
};

END