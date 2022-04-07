#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "Shader.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, mNumMeshContainer(rhs.mNumMeshContainer)
	, mpVectorMeshContainers(rhs.mpVectorMeshContainers)
	, meModelType(rhs.meModelType)
	, mNumMaterials(rhs.mNumMaterials)
	, mVectorMaterials(rhs.mVectorMaterials)

{
	// ���� ���� ����
	for (auto& mmatrial : mVectorMaterials)
	{
		for (auto& tex : mmatrial->pTexture)
		{
			Safe_AddRef(tex);
		}
	}

	// �޽� ���͵� ����
	if(mpVectorMeshContainers != nullptr)
	{
		for (_uint i = 0; i<mNumMaterials;++i)
		{
			for (auto& mesh : mpVectorMeshContainers[i])
			{
				Safe_AddRef(mesh);
			}
		}

	}

}



HRESULT CModel::NativeConstruct_Prototype(E_MODEL_TYPE type, const char * ModelPath, const char * ModelName, _fmatrix defaultMatrix)
{
	char	szFullPath[MAX_PATH] = "";

	if (type == CModel::MODEL_END)
		return E_FAIL;

	meModelType = type;
	XMStoreFloat4x4(&mTransformMatrix, defaultMatrix);

	strcpy_s(szFullPath, ModelPath);
	strcat_s(szFullPath, ModelName);

	// ���� �ҷ��� �� �ִϸ��̼� / �ִϰ� ���� ���� �и��ؼ� �ҷ��´�.
	_uint		iFlag = 0;

	// ��ǥ�� ������Ʈ / �ﰢ������ �׸� / ź��Ʈ���� ��� / 
	// �ִϸ��̼��� �ƴϸ� ������ ���� ���� �ٿ���
	
	if (meModelType == CModel::MODEL_NOANI)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;

	
	// aiScene ��ü�� ��� �޽� �����͸� �����Ѵ�. 
	// �� �����ڴ� aiScene�� �ʱ�ȭ�ϴ� �Ͱ� �ٸ� ����.
	mScene = mImporter.ReadFile(szFullPath, iFlag);
	if (mScene == nullptr)
		return E_FAIL;

	// Scene ������ �� ���� �޽� ������ �����Ѵ�.
	FAILED_CHECK(Ready_Materials(ModelPath));
	FAILED_CHECK(Ready_MeshContainers());

	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	return S_OK;
}



HRESULT CModel::Bind_OnShader(CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char * pValueName)
{
	if (iMaterialIndex >= mNumMaterials)
		return E_FAIL;

	// �� �ؽ�ó ���ε�
	CTexture*		pTexture = mVectorMaterials[iMaterialIndex]->pTexture[eTextureType];
	if (nullptr == pTexture)
		return E_FAIL;
	
	return pTexture->SetUp_OnShader(pShader, pValueName);
}

// �������� �ٸ� ���̴��� �׷��� �� ����.
HRESULT CModel::Render(CShader* shader,_uint pass, _uint iMaterialIndex)
{
	// ���ε��� ������ �޽��� ������ �ƴ� ������ ������ ������ ����.
	if (iMaterialIndex >= mNumMaterials ||
		mpVectorMeshContainers == nullptr)
		return E_FAIL;


	// �޽������̳ʵ��� �����ϴ� �����̳ʸ� ���� ������ŭ �����
	// �޽������̳ʸ� ���� ������ŭ ȣ���Ų��.
	for (auto& pMeshContainer : mpVectorMeshContainers[iMaterialIndex])
	{
		// ���� ���� �޽ù��� ������
		if (pMeshContainer != nullptr)
			pMeshContainer->Render(shader, pass);
	}
	return S_OK;
}



HRESULT CModel::Ready_MeshContainers()
{
	// �޽� ���� �ʱ�ȭ

	if (nullptr == mScene)
		return E_FAIL;

	// �޽� �����̳� ���͸� ������ �� ������ŭ �����Ҵ�.
	mNumMeshContainer = mScene->mNumMeshes;
	mpVectorMeshContainers = NEW MESHCONTAINERS[mNumMaterials];

	// �޽� ���� ��ŭ
	for (_uint i = 0; i < mNumMeshContainer; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, meModelType, mScene->mMeshes[i], XMLoadFloat4x4(&mTransformMatrix));
		if (nullptr == pMeshContainer)
			return E_FAIL;

		// �� �޽ð� ������ �ִ� ���� ��ȣ�� �޽ø� �ִ´�.
		// �� ���� ��ȣ�� �ؽ�ó�� ����� �׸��� ��ü�� ���� ����[ID]�� ����ȴ�.
		mpVectorMeshContainers[mScene->mMeshes[i]->mMaterialIndex].push_back(pMeshContainer);
	}
	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	// ���� �ʱ�ȭ
	if (mScene == nullptr)
		return E_FAIL;
	
	mNumMaterials = mScene->mNumMaterials;
	for (_uint i = 0; i < mNumMaterials; ++i)
	{
		MESHMATERIALDESC* meshDesc = NEW MESHMATERIALDESC;
		ZeroMemory(meshDesc, sizeof(MESHMATERIALDESC));

		// ���� �� Ÿ���� ��ȸ�ϸ鼭 �´� ���� ������Ʈ
		for (_uint k = 0; k < AI_TEXTURE_TYPE_MAX; ++k)
		{
			// mseh�� �ؽ�ó ��� ��ȯ
			aiString texturepath;
			if (FAILED(mScene->mMaterials[i]->GetTexture(aiTextureType(k), 0, &texturepath)))
				continue;

			// �ؽ�ó �̸� ����
			char szFullPath[MAX_PATH] = "";
			char szFileName[MAX_PATH] = "";
			char szEXE[MAX_PATH] = "";

			_splitpath_s(texturepath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXE, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXE);

			// �����ڵ� ����
			_tchar		szTextureFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szTextureFullPath, MAX_PATH);

			meshDesc->pTexture[k] = CTexture::Create(m_pDevice, m_pDeviceContext,szTextureFullPath,1);
			if (nullptr == meshDesc->pTexture[k])
				return E_FAIL;

		}
		mVectorMaterials.push_back(meshDesc);
	}

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, E_MODEL_TYPE eModelType, const char * modelPath, const char * modelName, _fmatrix defaultMatrix)
{
	CModel*	pInstance = NEW CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eModelType, modelPath, modelName, defaultMatrix)))
	{
		MSGBOX("Failed to Creating CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CModel * CModel::Clone(void * pArg)
{
	CModel*	pInstance = NEW CModel(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	// ���� ����
	for (auto& mat : mVectorMaterials)
	{
		for (auto& tex : mat->pTexture)
			Safe_Release(tex);

		if (m_isCloned == false)
			Safe_Delete(mat);	
	}
	mVectorMaterials.clear();


	// �޽� ����
	if (mpVectorMeshContainers != nullptr)
	{
		for (_uint i = 0; i < mNumMaterials; ++i)
		{
			for (auto& mesh : mpVectorMeshContainers[i])
				Safe_Release(mesh);

			if (m_isCloned == false)
				mpVectorMeshContainers[i].clear();
		}
	}

	if (false == m_isCloned)
	{
		Safe_Delete_Array(mpVectorMeshContainers);
	}

	// ���̾��Ű ����

	mImporter.FreeScene();

}
