#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:

	enum E_MODEL_TYPE { MODEL_NOANI, MODEL_ANI, MODEL_END };
private:
	explicit CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMaterials() const;
	_uint Get_NumAnimations()const { return m_iNumAnimations; }

	const vector<class CAnimation*>* Get_VecAnimations()const { return &m_Animations; }
	const vector<class CHierarchyNode*>* Get_VecHierarchy()const { return &m_HierarchyNodes; }
	HRESULT Set_AniString(string AniName);
	void	Set_BlendMaxTime(_double time) { mBlendMaxTime = time; }
	_double	Get_BlendMaxTime() { return mBlendMaxTime; }

public:
	virtual HRESULT NativeConstruct_Prototype(E_MODEL_TYPE eModelType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT SetUp_AnimIndex(_uint iAnimIndex);
	HRESULT Update_CombinedTransformationMatrices(_double TimeDelta);
	HRESULT Bind_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pValueName);
	HRESULT Render(class CShader* pShader, _uint iPassIndex, _uint iMaterialIndex, const char* pBoneValueName = nullptr);

public: // Animaion


private:
	const aiScene*				m_pScene = nullptr;
	Importer					m_Importer;

private: // 모델
	_uint									m_iNumMeshContainers = 0;
	vector<class CMeshContainer*>*			m_pMeshContainers = nullptr;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;
	E_MODEL_TYPE							m_eModelType = MODEL_END;
	_float4x4								m_TransformMatrix;

private: // 텍스처
	_uint									m_iNumMaterials = 0;
	vector<MESHMATERIALDESC*>				m_Materials;
	typedef vector<MESHMATERIALDESC*>		MATERIALS;

private: // 애니메이션
	
	// 계층
	vector<class CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<class CHierarchyNode*>	HIERARCHYNODES;

	// 애니메이션 클립
	_uint									m_iCurrentAniIndex = 0;
	_uint									m_iNewAniIndex = 0;
	_uint									m_iNumAnimations;
	vector<class CAnimation*>				m_Animations;
	typedef vector<class CAnimation*>		ANIMATIONS;

	// 애니메이션 제어
	bool									misBlend = false;
	_double									mBlendMaxTime = 0.5f;
	_double									mBlendTimer = 0;

	// 에니메이터 

private:
	HRESULT Ready_MeshContainers();
	HRESULT Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth);
	HRESULT Ready_OffsetMatrices();
	HRESULT Ready_Animation();
	HRESULT Link_ChannelToNode();

	HRESULT AniMationBlend(int startindex, int endindex, _double timer);
	HRESULT Set_BlendFrame(int startindex, int endindex);

private:
	CHierarchyNode* Find_HierarchyNode(const char* pName, _uint* pOut = nullptr);
	_uint Find_Animation(string AniName)const; // 애니메이션 찾기

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, E_MODEL_TYPE eModelType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END