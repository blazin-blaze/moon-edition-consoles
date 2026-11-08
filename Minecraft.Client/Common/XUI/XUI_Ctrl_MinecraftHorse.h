#pragma once

#include <string>
#include <XuiApp.h>

using namespace std;

class HorseRenderer;
class CXuiSceneHorseInventory;

class CXuiCtrlMinecraftHorse : public CXuiControlImpl
{
public:
	XUI_IMPLEMENT_CLASS(CXuiCtrlMinecraftHorse, L"CXuiCtrlMinecraftHorse", XUI_CLASS_CONTROL)

	CXuiCtrlMinecraftHorse();
	virtual ~CXuiCtrlMinecraftHorse() { };

protected:

	XUI_BEGIN_MSG_MAP()
		XUI_ON_XM_INIT( OnInit )
		XUI_ON_XM_RENDER(OnRender)
	XUI_END_MSG_MAP()

	HRESULT OnInit(XUIMessageInit* pInitData, BOOL& rfHandled);
	HRESULT OnRender(XUIMessageRender *pRenderData, BOOL &rfHandled);

private:
	BOOL m_bDirty;
	float m_fScale, m_fAlpha;
	int	m_iPad;
	CXuiSceneHorseInventory *m_containerScene;

	float m_fScreenWidth, m_fScreenHeight;
	float m_fRawWidth, m_fRawHeight;

};
