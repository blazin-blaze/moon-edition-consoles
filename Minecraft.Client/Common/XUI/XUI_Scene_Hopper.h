#pragma once
#include "..\Media\xuiscene_hopper.h"
#include "XUI_Scene_AbstractContainer.h"
#include "XUI_CustomMessages.h"
#include "..\UI\IUIScene_HopperMenu.h"

class CXuiSceneHopper : public CXuiSceneAbstractContainer, public IUIScene_HopperMenu
{
public:
	XUI_IMPLEMENT_CLASS( CXuiSceneHopper, L"CXuiSceneHopper", XUI_CLASS_SCENE )

protected:
	XUI_BEGIN_MSG_MAP()
		XUI_ON_XM_INIT( OnInit )
		XUI_ON_XM_KEYDOWN( OnKeyDown )
		XUI_ON_XM_DESTROY( OnDestroy )
		XUI_ON_XM_TIMER( OnTimer )
		XUI_ON_XM_SPLITSCREENPLAYER_MESSAGE(OnCustomMessage_Splitscreenplayer)
		XUI_ON_XM_TRANSITION_START(OnTransitionStart)
	XUI_END_MSG_MAP()

	BEGIN_CONTROL_MAP()
		MAP_CONTROL(IDC_Group, m_sceneGroup)
		BEGIN_MAP_CHILD_CONTROLS( m_sceneGroup )
			MAP_OVERRIDE(IDC_Inventory, m_inventoryControl)
			MAP_OVERRIDE(IDC_UseRow, m_useRowControl)
			MAP_OVERRIDE(IDC_Pointer, m_pointerControl)
			MAP_CONTROL(IDC_InventoryText, m_InventoryText)
			MAP_OVERRIDE(IDC_Hopper, m_hopperControl)
			MAP_CONTROL(IDC_HopperText, m_HopperText)
		END_MAP_CHILD_CONTROLS()
	END_CONTROL_MAP()

	HRESULT OnInit( XUIMessageInit* pInitData, BOOL& bHandled );
	HRESULT OnDestroy();
	virtual void InitDataAssociations(int iPad, AbstractContainerMenu *menu, int startIndex = 0);

private:
	CXuiCtrlSlotList* m_hopperControl;
	CXuiControl m_sceneGroup;
	CXuiControl m_HopperText;

	virtual CXuiControl* GetSectionControl( ESceneSection eSection );
	virtual CXuiCtrlSlotList* GetSectionSlotList( ESceneSection eSection );
};
