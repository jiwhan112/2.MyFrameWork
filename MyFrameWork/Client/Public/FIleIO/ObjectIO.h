#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

// 클라이언트의 DESC 정보 저장 불러오기 클래스
// 바이너리로 DESC파일 저장 및 불러오기 수행

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
	// 오브젝트 타입별로 저장 수행
	HRESULT SaverObject(E_OBJECT_TYPE type, wstring FolderPath, wstring filename, CGameObject* obj);

	// 연 파일을 넘겨받고 데이터 저장
	HRESULT SaverData(ofstream* fwrite, E_OBJECT_DATA_TYPE type, const void * desc);

	// 데이터를 불러와서 생성기에 넘김
	HRESULT LoadObject(wstring FolderPath, wstring filename, char** pData, E_OBJECT_TYPE* type);


private:
	// 각 데이터별로 저장
//	void SaveData(ofstream * fwrite, const void* desc, _uint size);
	HRESULT SaveOBJECT(ofstream* fwrite, E_OBJECT_TYPE* desc);
	HRESULT SaveUIDESC(ofstream* fwrite, UIDESC* desc);
	HRESULT SaveTEXTUREDESC(ofstream* fwrite, TEXTUREDESC* desc);
	HRESULT SaveMODELSTATICDESC(ofstream* fwrite, MODEL_STATIC_DESC* desc);
	HRESULT SaveMODELDYNAMICDESC(ofstream* fwrite, MODEL_DYNAMIC_DESC* desc);

private:
	// 임의 데이터 맴버 저장
	E_OBJECT_TYPE		mObjectType;
	UIDESC				mUIDesc;
	TEXTUREDESC			mTexDesc;

public:
	static CObjectIO* Create();
	virtual void Free() override;

};


END