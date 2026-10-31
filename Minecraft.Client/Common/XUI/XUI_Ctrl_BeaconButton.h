#pragma once

class CXuiCtrlBeaconButton : public CXuiControlImpl
{
public:
	XUI_IMPLEMENT_CLASS( CXuiCtrlBeaconButton, L"CXuiCtrlBeaconButton", XUI_CLASS_CONTROL )

	XUI_BEGIN_MSG_MAP()
		XUI_ON_XM_INIT( OnInit )
	XUI_END_MSG_MAP()

	HRESULT OnInit( XUIMessageInit* pInitData, BOOL& bHandled );

public:
	void SetData(int buttonId, int iconId, bool bActive, bool bSelected);
	bool IsButtonActive() const { return m_bActive; }
	bool IsButtonSelected() const { return m_bSelected; }
	int GetData() const { return m_buttonId; }
	void SetButtonActive(bool bActive);
	void SetButtonSelected(bool bSelected);

private:
	int m_buttonId;
	int m_iconId;
	bool m_bActive;
	bool m_bSelected;
};
