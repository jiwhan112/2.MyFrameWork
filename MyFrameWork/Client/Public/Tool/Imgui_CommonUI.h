#pragma once

#include "Imgui_Base.h"

BEGIN(Engine)
class CGameObject;
END
BEGIN(Client)


// ���� ���� UI / ������Ʈ ����Ʈ 
// ������ ��������
class CImgui_CommonUI final:
	public CImgui_Base
{
protected:
	explicit CImgui_CommonUI(ID3D11Device* device , ID3D11DeviceContext* context);
	explicit CImgui_CommonUI(const CImgui_CommonUI& rhs) = default;
	virtual ~CImgui_CommonUI() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual HRESULT Update(_double time);
	HRESULT Render_UI();

public:
	CGameObject* Get_SelectObject() const
	{
		return mSelectObject;
	}


protected: 
	virtual HRESULT Update_ObjectList();
	virtual HRESULT Set_SelectObject(CGameObject* obj);


private:
	void Update_ListBox(CGameObject * parent, _uint cnt, _int* selectindex);
	void PATHMODE();

	void Button_PathTxtSave(wstring path, wstring txtpath, wstring txtname);


private:
	CGameObject*			mSelectObject = nullptr;

public:
	static CImgui_CommonUI* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);

	virtual void Free() override;
};

END
