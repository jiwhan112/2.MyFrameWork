#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

// Ŭ���̾�Ʈ�� DESC ���� ���� �ҷ����� Ŭ����
// ���̳ʸ��� DESC���� ���� �� �ҷ����� ����

class CObjectIO final
	:public CBase
{

private:
	explicit	CObjectIO() = default;
	explicit	CObjectIO(const CObjectIO& rhs) = default;
	virtual		~CObjectIO() = default;
	
public:
	HRESULT NativeConstruct();

public:
	// ������Ʈ Ÿ�Ժ��� ���� ����
	HRESULT SaverObject(E_OBJECT_TYPE type, wstring FolderPath, wstring filename, CGameObject* obj);

	// �� ������ �Ѱܹް� ������ ����
	HRESULT SaverData(ofstream* fwrite, E_OBJECT_DATA_TYPE type, const void * desc);

	// �����͸� �ҷ��ͼ� �����⿡ �ѱ�
	HRESULT LoadObject(wstring FolderPath, wstring filename, char** pData, E_OBJECT_TYPE* type);


private:
	// �� �����ͺ��� ����
//	void SaveData(ofstream * fwrite, const void* desc, _uint size);
	HRESULT SaveOBJECT(ofstream* fwrite, E_OBJECT_TYPE* desc);
	HRESULT SaveUIDESC(ofstream* fwrite, UIDESC* desc);
	HRESULT SaveTEXTUREDESC(ofstream* fwrite, TEXTUREDESC* desc);
	HRESULT SaveMODELSTATICDESC(ofstream* fwrite, MODEL_STATIC_DESC* desc);
	HRESULT SaveMODELDYNAMICDESC(ofstream* fwrite, MODEL_DYNAMIC_DESC* desc);

private:
	// ���� ������ �ɹ� ����
	E_OBJECT_TYPE		mObjectType;
	UIDESC				mUIDesc;
	TEXTUREDESC			mTexDesc;

public:
	static CObjectIO* Create();
	virtual void Free() override;

};


END