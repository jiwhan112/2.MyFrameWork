#pragma once

#include "Base.h"

BEGIN(Engine)

// �𵨿� �ִϸ��̼� Ŭ�� �����͸� �̿��� ������ �� �ִ� �ִϸ��̼� ���¸� �����.

class CAnimation;
class CMeshContainer;
class CHierarchyNode;

// ���� / 
class ENGINE_DLL CAnimatior final 
	: public CBase
{
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

	const vector<CAnimation*>* Get_VecAnimaions() const
	{
		return &m_Animations;
	}
	void Set_BlendMaxTime(_float time)
	{
		mBlendMaxTime = time;
	}

	_float Get_BlendMaxTime() const
	{
		return mBlendMaxTime;
	}

public: // ������ �ִϸ��̼��� �ִ´�.
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



private: // �ִϸ����� ���
	HRESULT AniMationBlend(int startindex, int endindex, _double timer);

private: // �ִϸ��̼� Ŭ��
	const aiScene*							m_pScene;

	// ���� // ������ �ִ� ������ ���� 
	vector<CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<CHierarchyNode*>	HIERARCHYNODES;

	_uint									m_iCurrentAniIndex = 0;
	_uint									m_iNewAniIndex = 0;
	_uint									m_iNumAnimations;
	vector<CAnimation*>				m_Animations;
	typedef vector<CAnimation*>		ANIMATIONS;



private: // �ִϸ�����
	bool									misBlend = false;
	_double									mBlendMaxTime = 0.5f;
	_double									mBlendTimer = 0;


public:
	static CAnimatior* Create(const aiScene* scene, const vector<class CMeshContainer*>* con);
	virtual void Free() override;
};

END