#pragma once

#include "Base.h"

BEGIN(Client)

// Ŭ���̾�Ʈ�� DESC ���� ���� �ҷ����� Ŭ����
// ���̳ʸ��� DESC���� ���� �� �ҷ����� ����
class CGameObject_Base;

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
	HRESULT SaverObject(E_OBJECT_TYPE type, wstring FolderPath, wstring filename, CGameObject_Base* obj);


	// �����͸� �ҷ��ͼ� �����⿡ �ѱ�
	HRESULT LoadObject_Create(wstring FolderPath, wstring filename);

	// Creater �ʿ� ���� ����
	bool Create_CreateMap_ProtoType(HANDLE& hFile, wstring keyname);
	
private:
	// ������Ʈ�� descŸ�� ���� ������ �����Ѵ�.
	HRESULT Save_DESC(E_DESC_DATA descid, wstring FolderPath, wstring filename, void* desc, _uint size);


private:
	// ���� ������ �ɹ� ����
	E_OBJECT_TYPE					mObjectType;
	UI_DESC							mUIDesc;
	TEXTURE_UI_DESC					mTexDesc;

public:
	static CObjectIO* Create();
	virtual void Free() override;
};

END