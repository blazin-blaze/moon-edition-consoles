/*
MIT License

Copyright (c) 2026 Patoke

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "INP_Main.h"
#include "vendor\SDL\include\SDL3\SDL.h"

CInput InternalInputManager;
SDL_Event event;

CInput::CInput()
{
    m_Keyboard = CKeyboard();

    m_uiSigninJoypadMask = 0;
    for (int i = 0; i < 4; ++i)
        m_bIsMenuDisplayed[i] = 0;

	if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		assert(0);
    }

    InitTime();

    m_fRepeatDelaySecs = 0.5f;
    m_fRepeatRateSecs = 0.8f;
    m_sDebugSequenceName = 0;
    m_uiDebugSequenceIndex = 0;
    m_pDebugSequenceFn = 0;
}

void CInput::Initialise(int iInputStateC, unsigned char ucMapC, unsigned char ucActionC, unsigned char ucMenuActionC)
{
    assert(iInputStateC > 0);
    assert(ucMapC > 0);
    assert(ucActionC > 0);

    ZeroMemory(m_Joypads, sizeof(m_Joypads));

    m_ucInputStateC = iInputStateC;

    m_iDeadzone = 0x2710;
    m_iMovementRangeMax = 0x7FFF;

    m_iEffectiveRange = m_iMovementRangeMax - m_iDeadzone;
    m_iHalfRange = m_iEffectiveRange / 2;
    m_fEffectiveRange = (float)m_iEffectiveRange;

    for (int i = 0; i < MAX_JOYPADS; i++)
    {
        m_Joypads[i].m_pGamepad = nullptr;
        m_Joypads[i].m_ucInputStateIndex = -1;
        m_Joypads[i].m_pfLeftThumbXAxisMap = &m_Joypads[i].m_fNormalizedLeftThumbX;
        m_Joypads[i].m_pfLeftThumbYAxisMap = &m_Joypads[i].m_fNormalizedLeftThumbY;
        m_Joypads[i].m_pfRightThumbXAxisMap = &m_Joypads[i].m_fNormalizedRightThumbX;
        m_Joypads[i].m_pfRightThumbYAxisMap = &m_Joypads[i].m_fNormalizedRightThumbY;
        m_Joypads[i].m_pucLeftTriggerAxisMap = &m_Joypads[i].m_ucLeftTriggerState;
        m_Joypads[i].m_pucRightTriggerAxisMap = &m_Joypads[i].m_ucRightTriggerState;
        m_Joypads[i].m_fSensitivity = 1.0f;
    }

    m_JoypadMap = (unsigned int **)operator new[](ucMapC * sizeof(int *));
    for (int i = 0; i < ucMapC; i++)
    {
        m_JoypadMap[i] = (unsigned int *)operator new[](ucActionC * sizeof(int));
    }

    m_ucJoypadMapC = ucMapC;
    m_ucJoypadMapActionC = ucActionC;
    m_ucMenuActionC = ucMenuActionC;
    m_bJoypadMapArrayIsSetup = false;
}

void CInput::Tick(void)
{
	HandleGamepadEvents();
    UpdateJoypads();
    m_Keyboard.Tick();
}

void CInput::SetJoypadValues(JOYPAD *pThisPad)
{
    SDL_Gamepad *pGamePad = pThisPad->m_pGamepad;

    if (SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > (SDL_MAX_SINT16 / 2))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_LT;
    }
    if (SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > (SDL_MAX_SINT16 / 2))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_RT;
    }
	
    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_RB;
    }
    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_LB;
    }

    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_SOUTH))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_A;
    }
    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_EAST))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_B;
    }

    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_WEST))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_X;
    }
    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_NORTH))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_Y;
    }

    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_START))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_START;
    }
    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_BACK))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_BACK;
    }

    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_LEFT_STICK))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_LTHUMB;
    }
    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_RIGHT_STICK))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_RTHUMB;
    }

    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_DPAD_UP))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_DPAD_UP;
    }
    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_DPAD_DOWN))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_DPAD_DOWN;
    }
    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_DPAD_LEFT))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_DPAD_LEFT;
    }
    if (SDL_GetGamepadButton(pGamePad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT))
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_DPAD_RIGHT;
    }

	// RY and LY are inverted, so we have to negate them.
	// This does not apply to RX and LX.
    if (SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_LEFTX) > m_iHalfRange + m_iDeadzone)
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_LSTICK_RIGHT;
    }
    if (SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_LEFTX) < -m_iDeadzone - m_iHalfRange)
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_LSTICK_LEFT;
    }

    if (-SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_RIGHTY) < -m_iDeadzone - m_iHalfRange)
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_RSTICK_DOWN;
    }
    if (-SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_RIGHTY) > m_iHalfRange + m_iDeadzone)
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_RSTICK_UP;
    }
    if (SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_RIGHTX) > m_iHalfRange + m_iDeadzone)
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_RSTICK_RIGHT;
    }
    if (SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_RIGHTX) < -m_iDeadzone - m_iHalfRange)
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_RSTICK_LEFT;
    }

    if (-SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_LEFTY) < -m_iDeadzone - m_iHalfRange)
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_LSTICK_DOWN;
    }
    if (-SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_LEFTY) > m_iHalfRange + m_iDeadzone)
    {
        pThisPad->m_uiButtons |= _360_JOY_BUTTON_LSTICK_UP;
    }

    pThisPad->m_ucLeftTriggerState  = (unsigned char)(SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER)  >> 7);
    pThisPad->m_ucRightTriggerState = (unsigned char)(SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) >> 7);

	Sint16 sThumbRX = SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_RIGHTX);
    if (sThumbRX <= m_iDeadzone)
    {
        if (sThumbRX >= -m_iDeadzone)
            pThisPad->m_iRightThumbX = 0;
        else
            pThisPad->m_iRightThumbX = m_iDeadzone + sThumbRX;
    }
    else
    {
        pThisPad->m_iRightThumbX = sThumbRX - m_iDeadzone;
    }

	// RY and LY are inverted, so we have to negate them.
	// We have to also keep in mind that the thumbstick shouldn't go over the integer limit
	// then flip (Talking from experience :3)

	// This does not apply to RX and LX.
	Sint16 rawRY = SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_RIGHTY);
	if (rawRY < -SDL_MAX_SINT16) rawRY = -SDL_MAX_SINT16;
	Sint16 sThumbRY = -rawRY;

    if (sThumbRY <= m_iDeadzone)
    {
        if (sThumbRY >= -m_iDeadzone)
            pThisPad->m_iRightThumbY = 0;
        else
            pThisPad->m_iRightThumbY = m_iDeadzone + sThumbRY;
    }
    else
    {
        pThisPad->m_iRightThumbY = sThumbRY - m_iDeadzone;
    }

	Sint16 sThumbLX = SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_LEFTX);
    if (sThumbLX <= m_iDeadzone)
    {
        if (sThumbLX >= -m_iDeadzone)
            pThisPad->m_iLeftThumbX = 0;
        else
            pThisPad->m_iLeftThumbX = m_iDeadzone + sThumbLX;
    }
    else
    {
        pThisPad->m_iLeftThumbX = sThumbLX - m_iDeadzone;
    }

	Sint16 rawLY = SDL_GetGamepadAxis(pGamePad, SDL_GAMEPAD_AXIS_LEFTY);
	if (rawLY < -SDL_MAX_SINT16) rawLY = -SDL_MAX_SINT16;
	Sint16 sThumbLY = -rawLY;

    if (sThumbLY <= m_iDeadzone)
    {
        if (sThumbLY >= -m_iDeadzone)
            pThisPad->m_iLeftThumbY = 0;
        else
            pThisPad->m_iLeftThumbY = m_iDeadzone + sThumbLY;
    }
    else
    {
        pThisPad->m_iLeftThumbY = sThumbLY - m_iDeadzone;
    }

    pThisPad->m_iNormalizedLeftThumbX = pThisPad->m_iLeftThumbX / m_iEffectiveRange;
    pThisPad->m_iNormalizedLeftThumbY = pThisPad->m_iLeftThumbY / m_iEffectiveRange;
    pThisPad->m_iNormalizedRightThumbX = pThisPad->m_iRightThumbX / m_iEffectiveRange;
    pThisPad->m_iNormalizedRightThumbY = pThisPad->m_iRightThumbY / m_iEffectiveRange;

    pThisPad->m_fNormalizedLeftThumbX = pThisPad->m_iLeftThumbX / m_fEffectiveRange;
    pThisPad->m_fNormalizedLeftThumbY = pThisPad->m_iLeftThumbY / m_fEffectiveRange;
    pThisPad->m_fNormalizedRightThumbX = pThisPad->m_iRightThumbX / m_fEffectiveRange;
    pThisPad->m_fNormalizedRightThumbY = pThisPad->m_iRightThumbY / m_fEffectiveRange;
}

void CInput::SetDeadzoneAndMovementRange(unsigned int uiDeadzone, unsigned int uiMovementRangeMax)
{
    m_iDeadzone = uiDeadzone;
    m_iMovementRangeMax = uiMovementRangeMax;
    m_iEffectiveRange = uiMovementRangeMax - uiDeadzone;
    m_iHalfRange = m_iEffectiveRange / 2;
}

void CInput::SetJoypadSensitivity(int iPad, float fSensitivity)
{
    m_Joypads[iPad].m_fSensitivity = fSensitivity;
}

void CInput::SetGameJoypadMaps(unsigned char ucMap, unsigned char ucAction, unsigned int uiActionVal)
{
    assert(ucMap < m_ucJoypadMapC);
    assert(ucAction < m_ucJoypadMapActionC);

    m_JoypadMap[ucMap][ucAction] = uiActionVal;
}

unsigned int CInput::GetGameJoypadMaps(unsigned char ucMap, unsigned char ucAction)
{
    assert(ucMap < m_ucJoypadMapC);
    assert(ucAction < m_ucJoypadMapActionC);

    return m_JoypadMap[ucMap][ucAction];
}

void CInput::SetJoypadMapVal(int iPad, unsigned char ucMap)
{
    m_bJoypadMapArrayIsSetup = true;
    m_Joypads[iPad].m_ucMappingValue = ucMap;
}

unsigned char CInput::GetJoypadMapVal(int iPad)
{
    return m_Joypads[iPad].m_ucMappingValue;
}

void CInput::SetJoypadStickAxisMap(int iPad, unsigned int uiFrom, unsigned int uiTo)
{
    float *pfTo = nullptr;

    switch (uiTo)
    {
    case AXIS_MAP_LX:
        pfTo = &m_Joypads[iPad].m_fNormalizedLeftThumbX;
        break;
    case AXIS_MAP_LY:
        pfTo = &m_Joypads[iPad].m_fNormalizedLeftThumbY;
        break;
    case AXIS_MAP_RX:
        pfTo = &m_Joypads[iPad].m_fNormalizedRightThumbX;
        break;
    case AXIS_MAP_RY:
        pfTo = &m_Joypads[iPad].m_fNormalizedRightThumbY;
        break;
    }

    switch (uiFrom)
    {
    case AXIS_MAP_LX:
        m_Joypads[iPad].m_pfLeftThumbXAxisMap = pfTo;
        break;
    case AXIS_MAP_LY:
        m_Joypads[iPad].m_pfLeftThumbYAxisMap = pfTo;
        break;
    case AXIS_MAP_RX:
        m_Joypads[iPad].m_pfRightThumbXAxisMap = pfTo;
        break;
    case AXIS_MAP_RY:
        m_Joypads[iPad].m_pfRightThumbYAxisMap = pfTo;
        break;
    }
}

void CInput::SetJoypadStickTriggerMap(int iPad, unsigned int uiFrom, unsigned int uiTo)
{
    unsigned char *pucTo = nullptr;

    switch (uiTo)
    {
    case TRIGGER_MAP_0:
        pucTo = &m_Joypads[iPad].m_ucRightTriggerState;
        break;
    case TRIGGER_MAP_1:
        pucTo = &m_Joypads[iPad].m_ucLeftTriggerState;
        break;
    }

    switch (uiFrom)
    {
    case TRIGGER_MAP_0:
        m_Joypads[iPad].m_pucRightTriggerAxisMap = pucTo;
        break;
    case TRIGGER_MAP_1:
        m_Joypads[iPad].m_pucLeftTriggerAxisMap = pucTo;
        break;
    }
}

bool CInput::IsPadConnected(int iPad)
{
    return m_Joypads[iPad].m_bIsConnected;
}

void CInput::SetSigninJoypadMask(unsigned int mask)
{
    m_uiSigninJoypadMask = mask;
}

unsigned int CInput::GetValue(int iPad, unsigned char ucAction, bool bRepeat)
{
    if (m_bIsMenuDisplayed[iPad] && ucAction > this->m_ucMenuActionC)
    {
        return 0;
    }

    unsigned char ucMap = this->m_Joypads[iPad].m_ucMappingValue;
    unsigned int uiVal = this->m_JoypadMap[ucMap][ucAction] & this->m_Joypads[iPad].m_uiButtons;
    if (!uiVal || !bRepeat)
    {
        return uiVal;
    }

    int iKeyVal = 0;
    for (int iKey = this->m_JoypadMap[ucMap][ucAction]; (iKey & 1) == 0; iKey >>= 1)
    {
        iKeyVal++;
    }

    if (m_fRepeatDelaySecs <= m_Timers[iPad][iKeyVal].m_fTotalHoldTime || m_Timers[iPad][iKeyVal].m_bInitialPressHandled)
    {
        if (m_Timers[iPad][iKeyVal].m_fTotalHoldTime < m_fRepeatDelaySecs)
        {
            return 0;
        }
        else if (m_Timers[iPad][iKeyVal].m_fLastRepeatTime == 0.0f)
        {
            m_Timers[iPad][iKeyVal].m_fLastRepeatTime = m_fRepeatDelaySecs;
            return uiVal;
        }
        else if ((m_Timers[iPad][iKeyVal].m_fTotalHoldTime - m_Timers[iPad][iKeyVal].m_fLastRepeatTime) <= m_fRepeatRateSecs)
        {
            return 0;
        }
        else
        {
            m_Timers[iPad][iKeyVal].m_fLastRepeatTime = m_Timers[iPad][iKeyVal].m_fLastRepeatTime + m_fRepeatRateSecs;
            return uiVal;
        }
    }
    else
    {
        m_Timers[iPad][iKeyVal].m_bInitialPressHandled = true;
        return uiVal;
    }
}

bool CInput::IsSet(int iPad, unsigned char ucAction)
{
    return (!m_bIsMenuDisplayed[iPad] || ucAction <= m_ucMenuActionC) &&
           (m_JoypadMap[m_Joypads[iPad].m_ucMappingValue][ucAction] & m_Joypads[iPad].m_uiButtons) != 0;
}

bool CInput::ButtonPressed(int iPad, unsigned char ucAction)
{
    if (ucAction == 255)
    {
        return m_Joypads[iPad].m_uiButtonsPressed != 0;
    }

    if (m_bIsMenuDisplayed[iPad] && ucAction > m_ucMenuActionC)
    {
        return 0;
    }

    return (m_JoypadMap[m_Joypads[iPad].m_ucMappingValue][ucAction] & m_Joypads[iPad].m_uiButtonsPressed) != 0;
}

bool CInput::ButtonReleased(int iPad, unsigned char ucAction)
{
    return (!m_bIsMenuDisplayed[iPad] || ucAction <= m_ucMenuActionC) &&
           (m_JoypadMap[m_Joypads[iPad].m_ucMappingValue][ucAction] & m_Joypads[iPad].m_uiButtonsReleased) != 0;
}

bool CInput::ButtonDown(int iPad, unsigned char ucAction)
{
    if (ucAction == 255)
    {
        return m_Joypads[iPad].m_uiButtons != 0;
    }

    if (m_bIsMenuDisplayed[iPad] && ucAction > m_ucMenuActionC)
    {
        return 0;
    }

    return (m_JoypadMap[m_Joypads[iPad].m_ucMappingValue][ucAction] & m_Joypads[iPad].m_uiButtons) != 0;
}

float CInput::GetJoypadStick_Menu_LX(unsigned char ucPad)
{
    return m_Joypads[ucPad].m_iNormalizedLeftThumbX;
}

float CInput::GetJoypadStick_Menu_LY(unsigned char ucPad)
{
    return m_Joypads[ucPad].m_iNormalizedLeftThumbY;
}

float CInput::GetJoypadStick_Menu_RX(unsigned char ucPad)
{
    return m_Joypads[ucPad].m_iNormalizedRightThumbX;
}

float CInput::GetJoypadStick_Menu_RY(unsigned char ucPad)
{
    return m_Joypads[ucPad].m_iNormalizedRightThumbY;
}

unsigned char CInput::GetJoypadLTrigger_Menu(unsigned char ucPad)
{
    return m_Joypads[ucPad].m_ucLeftTriggerState;
}

unsigned char CInput::GetJoypadRTrigger_Menu(unsigned char ucPad)
{
    return m_Joypads[ucPad].m_ucRightTriggerState;
}

float CInput::GetJoypadStick_LX(int iPad, bool bCheckMenuDisplay)
{
    if (bCheckMenuDisplay && m_bIsMenuDisplayed[iPad])
    {
        return 0.0f;
    }

    return *m_Joypads[iPad].m_pfLeftThumbXAxisMap * m_Joypads[iPad].m_fSensitivity;
}

float CInput::GetJoypadStick_LY(int iPad, bool bCheckMenuDisplay)
{
    if (bCheckMenuDisplay && m_bIsMenuDisplayed[iPad])
    {
        return 0.0f;
    }

    return *m_Joypads[iPad].m_pfLeftThumbYAxisMap * m_Joypads[iPad].m_fSensitivity;
}

float CInput::GetJoypadStick_RX(int iPad, bool bCheckMenuDisplay)
{
    if (bCheckMenuDisplay && m_bIsMenuDisplayed[iPad])
    {
        return 0.0f;
    }

    return *m_Joypads[iPad].m_pfRightThumbXAxisMap * m_Joypads[iPad].m_fSensitivity;
}

float CInput::GetJoypadStick_RY(int iPad, bool bCheckMenuDisplay)
{
    if (bCheckMenuDisplay && m_bIsMenuDisplayed[iPad])
    {
        return 0.0f;
    }

    return *m_Joypads[iPad].m_pfRightThumbYAxisMap * m_Joypads[iPad].m_fSensitivity;
}

unsigned char CInput::GetJoypadLTrigger(int iPad, bool bCheckMenuDisplay)
{
    if (bCheckMenuDisplay && m_bIsMenuDisplayed[iPad])
    {
        return 0;
    }

    return *m_Joypads[iPad].m_pucLeftTriggerAxisMap;
}

unsigned char CInput::GetJoypadRTrigger(int iPad, bool bCheckMenuDisplay)
{
    if (bCheckMenuDisplay && m_bIsMenuDisplayed[iPad])
    {
        return 0;
    }

    return *m_Joypads[iPad].m_pucRightTriggerAxisMap;
}

void CInput::SetMenuDisplayed(int iPad, bool bVal)
{
    m_bIsMenuDisplayed[iPad] = bVal;
}

void CInput::SetKeyRepeatRate(float fRepeatDelaySecs, float fRepeatRateSecs)
{
    m_fRepeatDelaySecs = fRepeatDelaySecs;
    m_fRepeatRateSecs = fRepeatRateSecs;
}

void CInput::SetDebugSequence(const char *chSequenceA, int (*Func)(LPVOID), LPVOID lpParam)
{
    int iLen = strlen(chSequenceA);

    m_sDebugSequenceName = (char *)operator new[](iLen);
    strcpy(m_sDebugSequenceName, chSequenceA);

    m_pDebugSequenceFn = Func;
    m_pDebugSequenceParam = lpParam;
}

FLOAT CInput::GetIdleSeconds(int iPad)
{
    LARGE_INTEGER qwDeltaTime;
    QueryPerformanceCounter(&qwDeltaTime);

    LARGE_INTEGER fElapsedTime = this->m_LastActivityTime[iPad];
    uint64_t qwNewTime = qwDeltaTime.QuadPart - fElapsedTime.QuadPart;

    return this->m_fTickToSeconds * (qwDeltaTime.LowPart - fElapsedTime.LowPart);
}

void CInput::HandleGamepadEvents()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_GAMEPAD_ADDED:
			for (int i = 0; i < MAX_JOYPADS; i++)
			{
				if (!m_Joypads[i].m_pGamepad)
				{
					SDL_Gamepad *pad = SDL_OpenGamepad(event.gdevice.which);

					if (pad)
					{
						SDL_SetGamepadPlayerIndex(pad, i);
						m_Joypads[i].m_pGamepad = pad;
					}

					break;
				}
			}
			break;
		case SDL_EVENT_GAMEPAD_REMOVED:
			for (int i = 0; i < MAX_JOYPADS; i++)
			{
				if (m_Joypads[i].m_pGamepad &&
					SDL_GetGamepadID(m_Joypads[i].m_pGamepad) == event.gdevice.which)
				{
					SDL_CloseGamepad(m_Joypads[i].m_pGamepad);
					m_Joypads[i].m_pGamepad = nullptr;
					m_Joypads[i].m_bIsConnected = false;

					break;
				}
			}
			break;
		}
	}
}

bool CInput::UpdateJoypads()
{
    assert(m_bJoypadMapArrayIsSetup);

    for (int i = 0; i < MAX_JOYPADS; i++)
    {
        JOYPAD *pThisPad = &m_Joypads[i];

        pThisPad->m_uiOldButtons = pThisPad->m_uiButtons;
        pThisPad->m_uiButtons = 0;
        pThisPad->m_iRightThumbY = 0;
        pThisPad->m_iRightThumbX = 0;
        pThisPad->m_iLeftThumbY = 0;
        pThisPad->m_iLeftThumbX = 0;
        pThisPad->m_ucLeftTriggerState = 0;
        pThisPad->m_ucRightTriggerState = 0;
    }

    for (int i = 0; i < MAX_JOYPADS; i++)
    {
        if (!m_Joypads[i].m_bIsDisabled)
        {
            JOYPAD *pThisPad = &m_Joypads[i];

            bool bOldIsConnected = pThisPad->m_bIsConnected;

            pThisPad->m_ucInputStateIndex = pThisPad->m_ucInputStateIndex++;
            if (pThisPad->m_ucInputStateIndex == m_ucInputStateC)
            {
                pThisPad->m_ucInputStateIndex = 0;
            }

            pThisPad->m_bIsConnected = (pThisPad->m_pGamepad != nullptr);

            bool bWasConnected = bOldIsConnected && !pThisPad->m_bIsConnected;
            pThisPad->m_bWasConnected = bWasConnected;

            bool bHasConnected = !bOldIsConnected && pThisPad->m_bIsConnected;
            pThisPad->m_bHasConnected = bHasConnected;

            if (pThisPad->m_bIsConnected)
            {
                SetJoypadValues(pThisPad);

                pThisPad->m_uiButtonsPressed = ~pThisPad->m_uiOldButtons & pThisPad->m_uiButtons;
                pThisPad->m_uiButtonsReleased = ~pThisPad->m_uiButtons & pThisPad->m_uiOldButtons;

                for (int bit = 0; bit < 24; ++bit)
                {
                    if ((pThisPad->m_uiButtonsPressed & (1 << bit)) != 0)
                    {
                        GetStartTime(i, bit);
                    }
                    else if ((pThisPad->m_uiButtons & (1 << bit)) != 0)
                    {
                        UpdateTime(i, bit);
                    }
                }
            }
            else if (pThisPad->m_bWasConnected)
            {
                ClearJoypadValues(pThisPad);
            }

            if (m_pDebugSequenceFn && (pThisPad->m_uiButtonsPressed & (_360_JOY_BUTTON_A | _360_JOY_BUTTON_B | _360_JOY_BUTTON_X | _360_JOY_BUTTON_Y |
                                                                       _360_JOY_BUTTON_RB | _360_JOY_BUTTON_LB)) != 0)
            {
                char chSeq;

                if ((pThisPad->m_uiButtonsPressed & _360_JOY_BUTTON_A) == _360_JOY_BUTTON_A)
                {
                    chSeq = 'A';
                }
                else if ((pThisPad->m_uiButtonsPressed & _360_JOY_BUTTON_B) == _360_JOY_BUTTON_B)
                {
                    chSeq = 'B';
                }
                else if ((pThisPad->m_uiButtonsPressed & _360_JOY_BUTTON_X) == _360_JOY_BUTTON_X)
                {
                    chSeq = 'X';
                }
                else if ((pThisPad->m_uiButtonsPressed & _360_JOY_BUTTON_Y) == _360_JOY_BUTTON_Y)
                {
                    chSeq = 'Y';
                }
                else if ((pThisPad->m_uiButtonsPressed & _360_JOY_BUTTON_LB) == _360_JOY_BUTTON_LB)
                {
                    chSeq = 'L';
                }
                else if ((pThisPad->m_uiButtonsPressed & _360_JOY_BUTTON_RB) == _360_JOY_BUTTON_RB)
                {
                    chSeq = 'R';
                }

                if (m_sDebugSequenceName[m_uiDebugSequenceIndex] == chSeq)
                {
                    if (!m_sDebugSequenceName[m_uiDebugSequenceIndex++])
                    {
                        m_uiDebugSequenceIndex = 0;
                        (m_pDebugSequenceFn)(m_pDebugSequenceParam);
                    }
                }
                else
                {
                    m_uiDebugSequenceIndex = 0;
                }
            }
        }
    }

    return true;
}

void CInput::ClearJoypadValues(JOYPAD *pThisPad)
{
    pThisPad->m_uiButtonsPressed = 0;
    pThisPad->m_uiButtonsReleased = 0;
    pThisPad->m_uiButtons = 0;
    pThisPad->m_ucLeftTriggerState = 0;
    pThisPad->m_ucRightTriggerState = 0;
    pThisPad->m_iRightThumbX = 0;
    pThisPad->m_iRightThumbY = 0;
    pThisPad->m_iLeftThumbX = 0;
    pThisPad->m_iLeftThumbY = 0;
    pThisPad->m_iNormalizedLeftThumbX = 0;
    pThisPad->m_iNormalizedLeftThumbY = 0;
    pThisPad->m_iNormalizedRightThumbX = 0;
    pThisPad->m_iNormalizedRightThumbY = 0;
    pThisPad->m_fNormalizedLeftThumbX = 0.0f;
    pThisPad->m_fNormalizedLeftThumbY = 0.0f;
    pThisPad->m_fNormalizedRightThumbX = 0.0f;
    pThisPad->m_fNormalizedRightThumbY = 0.0f;
}

void CInput::InitTime(void)
{
    LARGE_INTEGER qwTicksPerSec;
    QueryPerformanceFrequency(&qwTicksPerSec);

    m_fTickToSeconds = 1.0 / (float)(int)qwTicksPerSec.LowPart;
    for (int i = 0; i < MAX_JOYPADS; i++)
    {
        m_LastActivityTime[i].QuadPart = 0;
    }
}

void CInput::GetStartTime(int iPad, int iKey)
{
    QueryPerformanceCounter(&m_Timers[iPad][iKey].m_qwStartTime);

    m_Timers[iPad][iKey].m_qwTotalHoldTicks.QuadPart = 0;
    m_Timers[iPad][iKey].m_fTotalHoldTime = 0;
    m_Timers[iPad][iKey].m_fLastRepeatTime = 0;
    m_Timers[iPad][iKey].m_bInitialPressHandled = false;

    m_LastActivityTime[iPad].QuadPart = m_Timers[iPad][iKey].m_qwStartTime.QuadPart;
}

void CInput::UpdateTime(int iPad, int iKey)
{
    LARGE_INTEGER qwDeltaTime;
    QueryPerformanceCounter(&qwDeltaTime);

    uint64_t qwNewTime = qwDeltaTime.QuadPart - m_Timers[iPad][iKey].m_qwStartTime.QuadPart;
    m_Timers[iPad][iKey].m_qwTotalHoldTicks.QuadPart += qwNewTime;

    m_Timers[iPad][iKey].m_qwStartTime = qwDeltaTime;
    m_Timers[iPad][iKey].m_fTotalHoldTime = m_Timers[iPad][iKey].m_fTotalHoldTime + (m_fTickToSeconds * qwNewTime);

    m_LastActivityTime[iPad].QuadPart = m_Timers[iPad][iKey].m_qwStartTime.QuadPart;
}