#include "stdafx.h"
#include "XUI_Ctrl_BeaconButton.h"

HRESULT CXuiCtrlBeaconButton::OnInit( XUIMessageInit* pInitData, BOOL& bHandled )
{
	m_buttonId = 0;
	m_iconId = 0;
	m_bActive = false;
	m_bSelected = false;

	return S_OK;
}

void CXuiCtrlBeaconButton::SetData(int buttonId, int iconId, bool bActive, bool bSelected)
{
	m_buttonId = buttonId;
	m_iconId = iconId;
	m_bActive = bActive;
	m_bSelected = bSelected;

	XuiControlSetEnable( m_hObj, bActive ? TRUE : FALSE );
}

void CXuiCtrlBeaconButton::SetButtonActive(bool bActive)
{
	m_bActive = bActive;
	XuiControlSetEnable( m_hObj, bActive ? TRUE : FALSE );
}

void CXuiCtrlBeaconButton::SetButtonSelected(bool bSelected)
{
	m_bSelected = bSelected;
}
