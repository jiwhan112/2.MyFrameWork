#include "stdafx.h"
#include "GameObject/GameObject_Environment.h"

vector<string>* CGameObject_Environment::mVec_TreeDel_EnviorName = nullptr;
vector<string>* CGameObject_Environment::mVec_TreeDead_EnviorName = nullptr;
vector<string>* CGameObject_Environment::mVec_Weed_EnviorName = nullptr;
vector<string>* CGameObject_Environment::mVec_Mush_EnviorName = nullptr;
vector<string>* CGameObject_Environment::mVec_Rock_EnviorName = nullptr;
vector<string>* CGameObject_Environment::mVec_Flower_Name = nullptr;

CGameObject_Environment::CGameObject_Environment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Static(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_STATIC;
}

CGameObject_Environment::CGameObject_Environment(const CGameObject_Environment& rhs)
	: CGameObject_3D_Static(rhs)
{


}

HRESULT CGameObject_Environment::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());
	mCurrentShaderPass = 0;
	return S_OK;
}

HRESULT CGameObject_Environment::NativeConstruct(void* pArg)
{

	// 모델과 충돌 위치만 랜덤 세팅


	if (IsName(mModelStatic_Desc.mModelName) == false)
	{
		string str("vege_Weed_good_a_00.fbx");
		strcpy_s(mModelStatic_Desc.mModelName, str.c_str());
	}
	Set_Component();

	FAILED_CHECK(__super::NativeConstruct(pArg));
	return S_OK;
}



HRESULT CGameObject_Environment::Environment_RandomSetting(E_ENVIORMENT_TYPE type)
{
	// 랜덤 모델 지점
	

	// vege_DeciduousTree_	// tree
	// vege_Weed_			// weed
	// vege_Mushroom_		// mushroom
	// terr_Rock_			// rock
	// ehub_Fountain_		// ehub

	// 환경 오브젝트 전용 Prefab 3개를 만든다음에 위치 지정해서 사용하자.


	if (mVec_TreeDel_EnviorName == nullptr)
	{
	const list<MYFILEPATH*>* listFBXpath_Static = GetSingle(CGameManager)->Get_PathList(CGameManager::PATHTYPE_FBX_STATIC);
		mVec_TreeDel_EnviorName = NEW vector<string>;
		mVec_TreeDead_EnviorName = NEW vector<string>;
		mVec_Weed_EnviorName = NEW vector<string>;
		mVec_Mush_EnviorName = NEW vector<string>;
		mVec_Rock_EnviorName = NEW vector<string>;
		mVec_Flower_Name = NEW vector<string>;

		
		for (auto& path : *listFBXpath_Static)
		{
			bool ismatch = false;
			wstring wName = path->FileName;
			string namestr = CHelperClass::Convert_Wstr2str(wName);

			if (ismatch = CHelperClass::in_quote(namestr, "vege_DeciduousTree_"))
			{
				mVec_TreeDel_EnviorName->push_back(namestr);
			}
			else if (ismatch = CHelperClass::in_quote(namestr, "vege_DeadTree_"))
			{
				mVec_TreeDead_EnviorName->push_back(namestr);
			}
			else if (ismatch = CHelperClass::in_quote(namestr, "vege_Weed_"))
			{
				mVec_Weed_EnviorName->push_back(namestr);
			}
			else if (ismatch = CHelperClass::in_quote(namestr, "vege_Mushroom_"))
			{
				mVec_Mush_EnviorName->push_back(namestr);
			}

			else if (ismatch = CHelperClass::in_quote(namestr, "terr_Rock_"))
			{
				mVec_Rock_EnviorName->push_back(namestr);

			}
			else if (ismatch = CHelperClass::in_quote(namestr, "ehub_Fountain_"))
			{
				mVec_Flower_Name->push_back(namestr);

			}

		}
	}
	int RandInt = 0;

	switch (type)
	{
	case Client::CGameObject_Environment::E_ENVIOR_TREE_DEL:
		RandInt = CHelperClass::RandomInt(0, mVec_TreeDel_EnviorName->size());
		strcpy_s(mModelStatic_Desc.mModelName, (*mVec_TreeDel_EnviorName)[RandInt].c_str());

		break;
	case Client::CGameObject_Environment::E_ENVIOR_TREE_DEAD:
		RandInt = CHelperClass::RandomInt(0, mVec_TreeDead_EnviorName->size());
		strcpy_s(mModelStatic_Desc.mModelName, (*mVec_TreeDead_EnviorName)[RandInt].c_str());

		break;
	case Client::CGameObject_Environment::E_ENVIOR_MUSH:
		RandInt = CHelperClass::RandomInt(0, mVec_Mush_EnviorName->size());
		strcpy_s(mModelStatic_Desc.mModelName, (*mVec_Mush_EnviorName)[RandInt].c_str());
		break;
	case Client::CGameObject_Environment::E_ENVIOR_WEED:
		RandInt = CHelperClass::RandomInt(0, mVec_Weed_EnviorName->size());
		strcpy_s(mModelStatic_Desc.mModelName, (*mVec_Weed_EnviorName)[RandInt].c_str());
		break;
	case Client::CGameObject_Environment::E_ENVIOR_ROCK:
		RandInt = CHelperClass::RandomInt(0, mVec_Rock_EnviorName->size());
		strcpy_s(mModelStatic_Desc.mModelName, (*mVec_Rock_EnviorName)[RandInt].c_str());
		break;
	case Client::CGameObject_Environment::E_ENVIOR_FLOWER:
		RandInt = CHelperClass::RandomInt(0, mVec_Flower_Name->size());
		strcpy_s(mModelStatic_Desc.mModelName, (*mVec_Flower_Name)[RandInt].c_str());
		break;
	case Client::CGameObject_Environment::E_ENVIOR_END:
		break;
	default:
		break;
	}

	// 여기서 모델 넣어줌
	Set_LoadModelDESC(mModelStatic_Desc);

	return S_OK;
}

HRESULT CGameObject_Environment::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	// 모델 타입에 따라 정적모델 동적모델 처리

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));

	if (mComModel == nullptr)
	{
		mComModel = nullptr;
	}
	if (mComCollider == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_SPHERE), TEXT("Com_Collider"), (CComponent**)&mComCollider));
	}
	return S_OK;
}


CGameObject_Environment * CGameObject_Environment::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Environment*	pInstance = NEW CGameObject_Environment(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Environment");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Environment* CGameObject_Environment::Clone(void* pArg)
{
	CGameObject_Environment*	pInstance = NEW CGameObject_Environment(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Environment");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Environment::Free()
{
	__super::Free();

	if (mVec_TreeDel_EnviorName)
	{
		mVec_TreeDel_EnviorName->clear();
		Safe_Delete(mVec_TreeDel_EnviorName);

	}

	if (mVec_TreeDead_EnviorName)
	{
		mVec_TreeDead_EnviorName->clear();
		Safe_Delete(mVec_TreeDead_EnviorName);

	}
	if (mVec_Weed_EnviorName)
	{
		mVec_Weed_EnviorName->clear();

		Safe_Delete(mVec_Weed_EnviorName);

	}
	if (mVec_Mush_EnviorName)
	{
		mVec_Mush_EnviorName->clear();
		Safe_Delete(mVec_Mush_EnviorName);

	}
	if (mVec_Rock_EnviorName)
	{
		mVec_Rock_EnviorName->clear();
		Safe_Delete(mVec_Rock_EnviorName);

	}
	if (mVec_Flower_Name)
	{
		mVec_Flower_Name->clear();
		Safe_Delete(mVec_Flower_Name);
	}
}
