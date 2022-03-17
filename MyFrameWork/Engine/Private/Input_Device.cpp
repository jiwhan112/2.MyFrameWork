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
	//���� Ű�Է°��� �˻��ؾ��ϴ� ��Ȳ�̰� ���� �����ӿ� ��ŸŸ���� �÷��� ���� ���ٸ�
	if (m_byDoubleKeyState[eKeyID] && !m_bIsKeyPulsDeltaTime[eKeyID])
	{
		//��ŸŸ���� �ø��� �̹� �����ӿ��� �ش� Ű�� ���� ������ ������ ������ٴ°��� ǥ��
		m_byDoubleKeyState[eKeyID] += m_fDeltaTime;
		m_bIsKeyPulsDeltaTime[eKeyID] = true;

		//���� Ű�Է� �˻�ð��� �ʰ��ߴٸ� 0���� �ٲ��
		if (m_byDoubleKeyState[eKeyID] > m_DoubleInterver)
			m_byDoubleKeyState[eKeyID] = 0;
	}

	if (m_byKeyState[eKeyID] & 0x80)
	{
		//���� ���Ȱ� ���� �����ӿ� ������ �ʾ�����(DOWN)
		if (!m_byOldKeyState[eKeyID])
		{
			//������ �������� ������ ���������Ӻ��� ��ŸŸ���� �˻��� �� �ֵ��� �ѹ� ���������ְ�
			if (!m_byDoubleKeyState[eKeyID])
			{
				m_byDoubleKeyState[eKeyID] += m_fDeltaTime;
				m_bIsKeyPulsDeltaTime[eKeyID] = true;
				return 0b00000010;//�ٿ�
			}
			//�� ������ ������ ������ ȣ�� �� �� ���� Ű�Է����� �Ǵ� ���� �����ϰ� ���� Ű�Է� ���͹� ���� Ű �Է��� �ٽ� ���� ���̶��
			else if (m_byDoubleKeyState[eKeyID] > m_fDeltaTime && m_byDoubleKeyState[eKeyID] <= m_DoubleInterver)
				return 0b00000100;//����ٿ�
			
			else
				return 0b00000010;
			

		}

		//���� ���Ȱ� ���� �����ӿ��� ������ ��(Pressing)
		else
			return 0b00001000;
	}
	else 
	{
		//���� �ȴ��Ȱ� ���� �����ӿ� ������ �ʾ�����(NoPressing)
		if ((!m_byOldKeyState[eKeyID]))
			return 0b00000000;

		//���� �ȴ��Ȱ� ���� �����ӿ� ������ ��(Up)
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

	//�������Ӹ��� ���� Ű�Է� �˻縦 ���� m_byDoubleKeyState(float * 256)���� 
	//���Ű�� ���� �˻��ϴ� ���� ��ȿ�� ���� �� ���Ƽ�
	//���� Ű �Է°˻縦 Ű�˻� ȣ��Ǵ� ������ �ѱ�� ���� ��ŸŸ���� �ѱ��
	//�������� ������ ������ ȣ��Ǵ��� �ѹ��� DeltaTime�� �ø��� ���� �ش� Ű�� �÷�������� memcpy�� false�� �ٲ� 
	m_fDeltaTime = fDeltaTime;


	ZeroMemory(m_bIsKeyPulsDeltaTime, sizeof(_bool) * 256);
	memcpy(m_byOldKeyState, m_byKeyState,sizeof(_byte)*256);
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
