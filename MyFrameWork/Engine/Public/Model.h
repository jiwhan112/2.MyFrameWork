#pragma once

#include "Component.h"

BEGIN(Engine)
class CHierarchyNode;
class CAnimation;
class ENGINE_DLL CModel final : public CComponent
{
public:
	enum E_MODEL_TYPE { MODEL_ANI,MODEL_NOANI,MODEL_END };

public:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(E_MODEL_TYPE type, const char* ModelPath, const char* ModelName, _fmatrix defaultMatrix);
	virtual HRESULT NativeConstruct(void* pArg);

	//	렌더링 전에 오브젝트 모델 컴포넌트에 사용할 텍스처 바인딩
	HRESULT Bind_OnShader(class CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char * pValueName);
	//	메시 컨테이너 랜더링
	HRESULT Render(class CShader* shader,_uint pass, _uint iMaterialIndex);

public:
	_uint Get_Materials() const { return mNumMaterials; }
	_uint Get_Meshs()const { return mNumMeshContainer; }

private:
	/*

	메쉬에 접근하기 위한 aiScene 객체 assimp 지원 클래스이다.
	mScene->mNumMeshes; // 재질 개수
	mScene->mMeshes[0]->mNumVertices[]; // 메쉬 내부 접근예시
	*/

	const aiScene*				mScene = nullptr;
	Importer					mImporter;

private:
	// 메쉬의 정보
	_uint									mNumMeshContainer = 0;
	// 메쉬 컨테이너의 집합
	vector<class CMeshContainer*>*			mpVectorMeshContainers = nullptr;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;

	E_MODEL_TYPE							meModelType = MODEL_END;
	_float4x4								mTransformMatrix;

	// 재질(텍스처)의 정보
	_uint									mNumMaterials = 0;
	vector<MESHMATERIALDESC*>				mVectorMaterials;
	typedef vector<MESHMATERIALDESC*>		MATERIALS;

	// 계층 노드 
	vector<CHierarchyNode*>					mVectorHierarchyNodes;
	typedef vector<CHierarchyNode*>			HIERARCHYNODES;

	// 애니메이션 
	_uint									miCurrentAnim=0;
	_uint									mNumAnimations;
	vector<CAnimation*>						mVectorAnimations;
	typedef vector<CAnimation*>				ANIMATIONS;


private:
	// 모델 메시 준비
	HRESULT Ready_MeshContainers();
	HRESULT Ready_Materials(const char* pModelFilePath);

	// 애니메이션 준비
	HRESULT Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth);
	HRESULT Ready_OffsetMatrices();
	HRESULT Ready_Animation();

	CHierarchyNode*		Find_HeirarchyNode(const char* name);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,
		E_MODEL_TYPE eModelType, const char* modelPath, const char* modelName, _fmatrix defaultMatrix);
	virtual CModel* Clone(void* pArg) override;
	virtual void Free() override;
};


END