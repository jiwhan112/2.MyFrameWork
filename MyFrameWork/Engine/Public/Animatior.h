#pragma once

#include "Base.h"

BEGIN(Engine)

// �𵨿� �ִϸ��̼� Ŭ�� �����͸� �̿��� ������ �� �ִ� �ִϸ��̼� ���¸� �����.

class CAnimationClip;
class CMeshContainer;
class CHierarchyNode;

// ���� / �ִϸ��̼� ��Ȳ�� ���� �±�ȭ
class ENGINE_DLL CAnimatior final
	: public CBase
{
	/*
// boss
skill
sleeping
att
spawnpose

// enemy
angry
carried
drink
eat
up
talk

// mine
miniJob // �̴���
brew_beer
build // �Ǽ�
carried
dance
transport
spawnpose
spawnDrop
workshop // �ٴ�


// orc
resing_in
resing_loop
angry


*/

public:
	static enum E_COMMON_ANINAME
	{
		// AI �̸� �߶� ���� �� ����

		// ���� AI
		E_COMMON_ANINAME_SKINPOSE,
		E_COMMON_ANINAME_WALK,
		E_COMMON_ANINAME_IDLE,
		E_COMMON_ANINAME_RUN,
		E_COMMON_ANINAME_SPAWNPOS,
		E_COMMON_ANINAME_SLEEPING,
		E_COMMON_ANINAME_MELEE,
		E_COMMON_ANINAME_MELEE0,
		E_COMMON_ANINAME_MELEE1,
		E_COMMON_ANINAME_MELEE2,
		E_COMMON_ANINAME_ANGRY,	// ȭ
		E_COMMON_ANINAME_UP,
		E_COMMON_ANINAME_CARRIED, // ���콺

		// MINE ĳ����
		E_COMMON_ANINAME_DIG, // ä��
		E_COMMON_ANINAME_DANCE,
		E_COMMON_ANINAME_DROPSPAWN,
		E_COMMON_ANINAME_DRAG,
		E_COMMON_ANINAME_MINI, // ���ϰŸ�
		E_COMMON_ANINAME_BUILD, // �Ǽ�
		E_COMMON_ANINAME_WORKSHOP, // ���ı�
		E_COMMON_ANINAME_TRANSPORT, // ���ı�

		// PLAYER
		E_COMMON_ANINAME_RESTING,

		// BOSS ĳ����
		E_COMMON_ANINAME_SKILL,
		E_COMMON_ANINAME_ATT,

		E_COMMON_ANINAME_END,
	};


	const char* STR_CommonAniName(E_COMMON_ANINAME e)
	{
		switch (e)
		{
		case E_COMMON_ANINAME_SKINPOSE:
			return "skinpose";
		case E_COMMON_ANINAME_WALK:
			return "walk";
		case E_COMMON_ANINAME_IDLE:
			return "idle";
		case E_COMMON_ANINAME_RUN:
			return "run";
		case E_COMMON_ANINAME_DIG:
			return "dig";
		case E_COMMON_ANINAME_DANCE:
			return "dance";
		case E_COMMON_ANINAME_SLEEPING:
			return "sleeping";
		case E_COMMON_ANINAME_DRAG:
			return "drag";
		case E_COMMON_ANINAME_MELEE:
			return "melee";

		case E_COMMON_ANINAME_MELEE0:
			return "melee0";
		case E_COMMON_ANINAME_MELEE1:
			return "melee1";
		case E_COMMON_ANINAME_MELEE2:
			return "melee2";

		case E_COMMON_ANINAME_UP:
			return "up";
		case E_COMMON_ANINAME_SPAWNPOS:
			return "spawnpose";
			break;
		case E_COMMON_ANINAME_ANGRY:
			return "angry";
			break;
		case E_COMMON_ANINAME_CARRIED:
			return "carried";
			break;
		case E_COMMON_ANINAME_DROPSPAWN:
			return "spawnDrop";
			break;

		case E_COMMON_ANINAME_MINI:
			return "miniJob";
			break;
		case E_COMMON_ANINAME_BUILD:
			return "build";
			break;
		case E_COMMON_ANINAME_WORKSHOP:
			return "workshop";
			break;
		case E_COMMON_ANINAME_TRANSPORT:
			return "transport";
			break;
		case E_COMMON_ANINAME_RESTING:
			return "resing";
			break;
		case E_COMMON_ANINAME_SKILL:
			return "skill";
			break;
		case E_COMMON_ANINAME_ATT:
			return "att";
			break;

		default:
			return "";

		}
	}


public:
	typedef struct tag_AniMationNODE
	{
		CAnimationClip* clip;

		tag_AniMationNODE* aniLeft = nullptr;
		tag_AniMationNODE* aniRight = nullptr;

	}ANIMATIONNODE;

private:
	CAnimatior(const aiScene* _aiscene);
	virtual ~CAnimatior() = default;

public: // Get Set
	const vector<CHierarchyNode*>& Get_HierarchyNode()const
	{
		return m_HierarchyNodes;
	}
	//const vector<CHierarchyNode*>& Get_BoneMatrix()const
	//{
	//	return m_HierarchyNodes;
	//}

	HRESULT SetUp_AnimIndex(_uint index);
	HRESULT Set_AniString(string AniName);
	HRESULT Set_AniEnum(E_COMMON_ANINAME AniName, _int index = -1, _bool isFoward = true); // enum �� ���� Ȥ�� index
	_int	Get_AniEnum2Index(E_COMMON_ANINAME AniName,_int index = -1);

	_double	Get_AniIndex2AniMaxTime(_int index);

	_uint	Get_NewAniIndex()const
	{
		return  m_iNewAniIndex;
	}
	_double	Get_NewIndexAniMaxTime()const;

	const vector<CAnimationClip*>* Get_VecAnimaions() const
	{
		return &mVecAnimations;
	}
	void Set_BlendMaxTime(_float time)
	{
		mBlendMaxTime = time;
	}

	_double Get_BlendMaxTime() const
	{
		return mBlendMaxTime;
	}

	void Set_CurrentAnimaionTime(_float time);
	CAnimationClip* Get_CurrentAnimaion()const;
	_double Get_CurrentAnimationTimeRatio() const;

	_bool Get_CurrentAniEnd() const
	{
		return mIsCurrentAniEnd;
	}

public:
	HRESULT NativeConstruct(const vector<CMeshContainer*>* meshcon);

public:
	HRESULT Update_CombinedTransformationMatrices(_double timer);
	HRESULT Update_CombinedTransformationMatrices_OnlyTime(_double TimeDelta);

	// �ִϸ��̼� �� ã��
	CHierarchyNode* Find_HierarchyNode(const char* pName, _uint* pOut = nullptr) const;
private:
	HRESULT Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* parentNode, _uint iDepth);
	HRESULT Ready_OffsetMatrices(const vector<CMeshContainer*>* meshcon);
	HRESULT Ready_Animation();
	HRESULT Link_ChannelToNode();

private:
	_int Find_Animation(string aniname) const;



private: // �ִϸ����� ���
	HRESULT AniMationBlend(int startindex, int endindex, _double timer);

private: // �ִϸ��̼� Ŭ��
	const aiScene*							m_pScene;

	// ���� // ������ �ִ� ������ ���� 
	vector<CHierarchyNode*>					m_HierarchyNodes;
	typedef vector<CHierarchyNode*>			HIERARCHYNODES;

	_uint									m_iCurrentAniIndex = 0;
	_uint									m_iNewAniIndex = 0;
	_uint									m_iNumAnimations;

	vector<CAnimationClip*>					mVecAnimations;

	// ��Ƽ �ִϸ��̼����� �ϵ��ڵ�


private: // �ִϸ�����
	bool									misBlend = false;
	_double									mBlendMaxTime = 0.2f;
	_double									mBlendTimer = 0;
	_bool									mIsCurrentAniEnd = false;

	// �ִϿ���� �÷���
	_bool									mIsFoward = true;


public:
	static CAnimatior* Create(const aiScene* scene, const vector<class CMeshContainer*>* con);
	virtual void Free() override;
};

END