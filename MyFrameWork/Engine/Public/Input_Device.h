#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:
	enum MOUSEMOVESTATE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };
	enum MOUSEBUTTONSTATE { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_END };

private:
	CInput_Device();
	virtual ~CInput_Device() = default;

public:
	_byte Get_DIKeyState(_ubyte eKeyID);

	_long Get_DIMouseMoveState(MOUSEMOVESTATE eMouseMoveState) {	return *((_long*)&m_MouseState + eMouseMoveState);  }

	_byte Get_DIMouseButtonState(MOUSEBUTTONSTATE eMouseButtonState);
	

public:
	HRESULT Ready_Input_Device(HINSTANCE hInst, HWND hWnd, _float fDoubleTimeInterver);
	HRESULT SetUp_InputDeviceState(_float fDeltaTime);

private:
	LPDIRECTINPUT8				m_pInput = nullptr;
	LPDIRECTINPUTDEVICE8		m_pKeyboard = nullptr;
	LPDIRECTINPUTDEVICE8		m_pMouse = nullptr;	


	
private:
	_byte					m_byKeyState[256];
	_byte					m_byOldKeyState[256];			//이전 프레임상태를 저장 (다운 체크과 업을 체그하기 위해)
	_float					m_byDoubleKeyState[256];		//더블 키 입력 시간 간격을 체크하기 위한 배열
	_bool					m_bIsKeyPulsDeltaTime[256];		//더블 키 입력을 여러번 받는 것을 방지하기 위한 변수

	DIMOUSESTATE			m_MouseState;
	_byte					m_byOldMouseState[MBS_END];						//이전 프레임상태를 저장 (다운 체크과 업을 체그하기 위해)
	_float					m_byDoubleMouseState[MBS_END];			//더블 키 입력 시간 간격을 체크하기 위한 배열
	_bool					m_bIsMousePulsDeltaTime[MBS_END];		//더블 키 입력을 여러번 받는 것을 방지하기 위한 변수

	_float					m_DoubleInterver;
	_float					m_fDeltaTime;

public:
	virtual void Free() override;
};

END