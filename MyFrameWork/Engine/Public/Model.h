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

	//	������ ���� ������Ʈ �� ������Ʈ�� ����� �ؽ�ó ���ε�
	HRESULT Bind_OnShader(class CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char * pValueName);
	//	�޽� �����̳� ������
	HRESULT Render(class CShader* shader,_uint pass, _uint iMaterialIndex);

public:
	_uint Get_Materials() const { return mNumMaterials; }
	_uint Get_Meshs()const { return mNumMeshContainer; }

private:
	/*

	�޽��� �����ϱ� ���� aiScene ��ü assimp ���� Ŭ�����̴�.
	mScene->mNumMeshes; // ���� ����
	mScene->mMeshes[0]->mNumVertices[]; // �޽� ���� ���ٿ���
	*/

	const aiScene*				mScene = nullptr;
	Importer					mImporter;

private:
	// �޽��� ����
	_uint									mNumMeshContainer = 0;
	// �޽� �����̳��� ����
	vector<class CMeshContainer*>*			mpVectorMeshContainers = nullptr;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;

	E_MODEL_TYPE							meModelType = MODEL_END;
	_float4x4								mTransformMatrix;

	// ����(�ؽ�ó)�� ����
	_uint									mNumMaterials = 0;
	vector<MESHMATERIALDESC*>				mVectorMaterials;
	typedef vector<MESHMATERIALDESC*>		MATERIALS;

	// ���� ��� 
	vector<CHierarchyNode*>					mVectorHierarchyNodes;
	typedef vector<CHierarchyNode*>			HIERARCHYNODES;

	// �ִϸ��̼� 
	_uint									miCurrentAnim=0;
	_uint									mNumAnimations;
	vector<CAnimation*>						mVectorAnimations;
	typedef vector<CAnimation*>				ANIMATIONS;


private:
	// �� �޽� �غ�
	HRESULT Ready_MeshContainers();
	HRESULT Ready_Materials(const char* pModelFilePath);

	// �ִϸ��̼� �غ�
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