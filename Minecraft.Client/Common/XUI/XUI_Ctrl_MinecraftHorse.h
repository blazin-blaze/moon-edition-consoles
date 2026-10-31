#pragma once

class CXuiCtrlMinecraftHorse : public CXuiControlImpl
{
public:
	XUI_IMPLEMENT_CLASS( CXuiCtrlMinecraftHorse, L"CXuiCtrlMinecraftHorse", XUI_CLASS_CONTROL )

	XUI_BEGIN_MSG_MAP()
		XUI_ON_XM_INIT( OnInit )
	XUI_END_MSG_MAP()

	HRESULT OnInit( XUIMessageInit* pInitData, BOOL& bHandled );
};
