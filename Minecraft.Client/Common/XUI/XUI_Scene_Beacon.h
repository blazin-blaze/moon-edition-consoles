#pragma once
#include "..\Media\xuiscene_beacon.h"
#include "XUI_Scene_AbstractContainer.h"
#include "XUI_Ctrl_BeaconButton.h"
#include "XUI_CustomMessages.h"
#include "..\UI\IUIScene_BeaconMenu.h"

class CXuiSceneBeacon : public CXuiSceneAbstractContainer, public IUIScene_BeaconMenu
{
public:
	XUI_IMPLEMENT_CLASS( CXuiSceneBeacon, L"CXuiSceneBeacon", XUI_CLASS_SCENE )

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
			MAP_OVERRIDE(IDC_Payment, m_paymentControl)
			MAP_CONTROL(IDC_PrimaryText, m_PrimaryText)
			MAP_CONTROL(IDC_SecondaryText, m_SecondaryText)
			MAP_OVERRIDE(IDC_PrimaryTierOneOne, m_primaryTierOneOne)
			MAP_OVERRIDE(IDC_PrimaryTierOneTwo, m_primaryTierOneTwo)
			MAP_OVERRIDE(IDC_PrimaryTierTwoOne, m_primaryTierTwoOne)
			MAP_OVERRIDE(IDC_PrimaryTierTwoTwo, m_primaryTierTwoTwo)
			MAP_OVERRIDE(IDC_PrimaryTierThree, m_primaryTierThree)
			MAP_OVERRIDE(IDC_SecondaryOne, m_secondaryOne)
			MAP_OVERRIDE(IDC_SecondaryTwo, m_secondaryTwo)
			MAP_OVERRIDE(IDC_Confirm, m_confirmButton)
		END_MAP_CHILD_CONTROLS()
	END_CONTROL_MAP()

	HRESULT OnInit( XUIMessageInit* pInitData, BOOL& bHandled );
	HRESULT OnDestroy();
	virtual void InitDataAssociations(int iPad, AbstractContainerMenu *menu, int startIndex = 0);

	virtual void SetConfirmButtonEnabled(bool enabled);
	virtual void AddPowerButton(int id, int icon, int tier, int count, bool active, bool selected);
	virtual int GetPowerButtonId(ESceneSection eSection);
	virtual bool IsPowerButtonSelected(ESceneSection eSection);
	virtual void SetPowerButtonSelected(ESceneSection eSection);

	virtual void handleTick();
	virtual HRESULT handleCustomTimer( XUIMessageTimer *pTimer, BOOL& bHandled );

private:
	CXuiCtrlSlotList* m_paymentControl;
	CXuiCtrlBeaconButton* m_primaryTierOneOne;
	CXuiCtrlBeaconButton* m_primaryTierOneTwo;
	CXuiCtrlBeaconButton* m_primaryTierTwoOne;
	CXuiCtrlBeaconButton* m_primaryTierTwoTwo;
	CXuiCtrlBeaconButton* m_primaryTierThree;
	CXuiCtrlBeaconButton* m_secondaryOne;
	CXuiCtrlBeaconButton* m_secondaryTwo;
	CXuiCtrlBeaconButton* m_confirmButton;
	CXuiControl m_sceneGroup;
	CXuiControl m_PrimaryText;
	CXuiControl m_SecondaryText;

	virtual CXuiControl* GetSectionControl( ESceneSection eSection );
	virtual CXuiCtrlSlotList* GetSectionSlotList( ESceneSection eSection );
};
