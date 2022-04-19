#include "..\public\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{
}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd, _float fDoubleTimeInterver)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
		return E_FAIL;

	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->Acquire()))
		return E_FAIL;

	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////
	m_DoubleInterver = fDoubleTimeInterver;

	ZeroMemory(m_byKeyState, sizeof(_byte) * 256);
	ZeroMemory(m_byOldKeyState, sizeof(_byte) * 256);
	ZeroMemory(m_byDoubleKeyState, sizeof(_float) * 256);
	ZeroMemory(m_bIsKeyPulsDeltaTime, sizeof(_bool) * 256);


	ZeroMemory(&m_MouseState, sizeof(MOUSEBUTTONSTATE));
	ZeroMemory(&m_byOldMouseState, sizeof(_byte)* MBS_END);
	ZeroMemory(m_byDoubleMouseState, sizeof(_float) * MBS_END);
	ZeroMemory(m_bIsMousePulsDeltaTime, sizeof(_bool) * MBS_END);


	return S_OK;
}


_byte CInput_Device::Get_DIKeyState(_ubyte eKeyID)
{
	//더블 키입력값을 검사해야하는 상황이고 현재 프레임에 델타타임을 올려준 적이 없다면
	if (m_byDoubleKeyState[eKeyID] && !m_bIsKeyPulsDeltaTime[eKeyID])
	{
		//델타타임을 올리고 이번 프레임에는 해당 키에 대한 프레임 증가를 시켜줬다는것을 표시
		m_byDoubleKeyState[eKeyID] += m_fDeltaTime;
		m_bIsKeyPulsDeltaTime[eKeyID] = true;

		//더블 키입력 검사시간을 초과했다면 0으로 바꿔라
		if (m_byDoubleKeyState[eKeyID] > m_DoubleInterver)
			m_byDoubleKeyState[eKeyID] = 0;
	}

	if (m_byKeyState[eKeyID] & 0x80)
	{
		//지금 눌렸고 이전 프레임에 눌리지 않았을때(DOWN)
		if (!m_byOldKeyState[eKeyID])
		{
			//이전에 눌린적이 없으면 다음프레임부터 델타타임을 검사할 수 있도록 한번 증가시켜주고
			if (!m_byDoubleKeyState[eKeyID])
			{
				m_byDoubleKeyState[eKeyID] += m_fDeltaTime;
				m_bIsKeyPulsDeltaTime[eKeyID] = true;
				return 0b00000010;//다운
			}
			//한 프레임 내에서 여러번 호출 될 때 더블 키입력으로 되는 것을 방지하고 더블 키입력 인터벌 내로 키 입력을 다시 받은 것이라면
			else if (m_byDoubleKeyState[eKeyID] > m_fDeltaTime && m_byDoubleKeyState[eKeyID] <= m_DoubleInterver)
				return 0b00000100;//더블다운

			else
				return 0b00000010;


		}

		//지금 눌렸고 이전 프레임에도 눌렸을 떄(Pressing)
		else
			return 0b00001000;
	}
	else
	{
		//지금 안눌렸고 이전 프레임에 눌리지 않았을때(NoPressing)
		if ((!m_byOldKeyState[eKeyID]))
			return 0b00000000;

		//지금 안눌렸고 이전 프레임에 눌렸을 떄(Up)
		else
			return 0b00000001;

	}
}

_byte CInput_Device::Get_DIMouseButtonState(MOUSEBUTTONSTATE eMouseButtonState)
{
	if (m_byDoubleMouseState[eMouseButtonState] && !m_bIsMousePulsDeltaTime[eMouseButtonState])
	{
		m_byDoubleMouseState[eMouseButtonState] += m_fDeltaTime;
		m_bIsMousePulsDeltaTime[eMouseButtonState] = true;

		if (m_byDoubleMouseState[eMouseButtonState] > m_DoubleInterver)
			m_byDoubleMouseState[eMouseButtonState] = 0;
	}

	if (m_MouseState.rgbButtons[eMouseButtonState] & 0x80)
	{
		if (!m_byOldMouseState[eMouseButtonState])
		{
			if (!m_byDoubleMouseState[eMouseButtonState])
			{
				m_byDoubleMouseState[eMouseButtonState] += m_fDeltaTime;
				m_bIsMousePulsDeltaTime[eMouseButtonState] = true;
				return 0b00000010;
			}
			else if (m_byDoubleMouseState[eMouseButtonState] > m_fDeltaTime &&
				m_byDoubleMouseState[eMouseButtonState] <= m_DoubleInterver)
				return 0b00000100;
			else
				return 0b00000010;
		}
		else
			return 0b00001000;
	}
	else
	{
		if ((!m_byOldMouseState[eMouseButtonState]))
			return 0b00000000;
		else
			return 0b00000001;
	}
}


HRESULT CInput_Device::SetUp_InputDeviceState(_float fDeltaTime)
{
	if (nullptr == m_pKeyboard ||
		nullptr == m_pMouse)
		return E_FAIL;

	//매프레임마다 더블 키입력 검사를 위해 m_byDoubleKeyState(float * 256)개의 
	//모든키의 값을 검사하는 것은 비효율 적인 것 같아서
	//더블 키 입력검사를 키검사 호출되는 곳으로 넘기기 위해 델타타임을 넘기고
	//한프레임 내에서 여러번 호출되더라도 한번만 DeltaTime을 올리기 위해 해당 키에 올려줬는지를 memcpy로 false로 바꿈 
	m_fDeltaTime = fDeltaTime;


	ZeroMemory(m_bIsKeyPulsDeltaTime, sizeof(_bool) * 256);
	memcpy(m_byOldKeyState, m_byKeyState, sizeof(_byte) * 256);
	m_pKeyboard->GetDeviceState(256, m_byKeyState);


	ZeroMemory(m_bIsMousePulsDeltaTime, sizeof(_bool) * MBS_END);
	memcpy(&m_byOldMouseState, &(m_MouseState.rgbButtons), sizeof(_byte)* MBS_END);
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	return S_OK;
}

void CInput_Device::Free()
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyboard);
	Safe_Release(m_pInput);
}
