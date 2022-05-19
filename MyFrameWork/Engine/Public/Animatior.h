#pragma once

#include "Base.h"

BEGIN(Engine)

// 모델에 애니메이션 클립 데이터를 이용해 제어할 수 있는 애니메이션 상태를 만든다.

class CAnimationClip;
class CMeshContainer;
class CHierarchyNode;

// 블랜드 / 
class ENGINE_DLL CAnimatior final 
	: public CBase
{
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
	HRESULT SetUp_AnimIndex(_uint index);
	HRESULT Set_AniString(string AniName);

	const vector<CAnimationClip*>* Get_VecAnimaions() const
	{
		return &m_Animations;
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

public: 
	HRESULT NativeConstruct(const vector<CMeshContainer*>* meshcon);

public:
	HRESULT Update_CombinedTransformationMatrices(_double timer);

private:
	HRESULT Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* parentNode, _uint iDepth);
	HRESULT Ready_OffsetMatrices(const vector<CMeshContainer*>* meshcon);
	HRESULT Ready_Animation();
	HRESULT Link_ChannelToNode();

private:
	CHierarchyNode* Find_HierarchyNode(const char* pName, _uint* pOut = nullptr) const;
	_int Find_Animation(string aniname) const;



private: // 애니메이터 기능
	HRESULT AniMationBlend(int startindex, int endindex, _double timer);

private: // 애니메이션 클립
	const aiScene*							m_pScene;

	// 계층 // 영향을 주는 뼈정보 세팅 
	vector<CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<CHierarchyNode*>	HIERARCHYNODES;

	_uint									m_iCurrentAniIndex = 0;
	_uint									m_iNewAniIndex = 0;
	_uint									m_iNumAnimations;
	vector<CAnimationClip*>					m_Animations;

private: // 애니메이터
	bool									misBlend = false;
	_double									mBlendMaxTime = 0.2f;
	_double									mBlendTimer = 0;


public:
	static CAnimatior* Create(const aiScene* scene, const vector<class CMeshContainer*>* con);
	virtual void Free() override;
};

END