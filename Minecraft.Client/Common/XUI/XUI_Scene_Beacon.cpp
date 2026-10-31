#include "stdafx.h"

#include "..\..\..\Minecraft.World\Container.h"
#include "..\..\..\Minecraft.World\BeaconMenu.h"
#include "..\..\..\Minecraft.World\net.minecraft.world.level.tile.entity.h"
#include "..\..\..\Minecraft.World\net.minecraft.world.effect.h"
#include "..\..\..\Minecraft.World\HtmlString.h"
#include "..\..\MultiplayerLocalPlayer.h"
#include "XUI_Ctrl_SlotList.h"
#include "XUI_Scene_Beacon.h"
#include "XUI_Ctrl_SlotItemListItem.h"
#include "XUI_Ctrl_SlotItem.h"
#include "..\..\Common\Tutorial\Tutorial.h"
#include "..\..\Common\Tutorial\TutorialMode.h"
#include "..\..\Common\Tutorial\TutorialEnum.h"

#define BEACON_TICK_TIMER_ID 12

HRESULT CXuiSceneBeacon::OnInit( XUIMessageInit* pInitData, BOOL& bHandled )
{
	D3DXVECTOR3 vec;
	MapChildControls();

	Minecraft *pMinecraft = Minecraft::GetInstance();

	BeaconScreenInput* initData = (BeaconScreenInput*)pInitData->pvInitData;

	m_beacon = initData->beacon;

	BeaconMenu* menu = new BeaconMenu( initData->inventory, initData->beacon );

	m_iPad=initData->iPad;
	m_bSplitscreen=initData->bSplitscreen;

#ifdef _XBOX
	if( pMinecraft->localgameModes[initData->iPad] != NULL )
	{
		TutorialMode *gameMode = (TutorialMode *)pMinecraft->localgameModes[initData->iPad];
		m_previousTutorialState = gameMode->getTutorial()->getCurrentState();
		gameMode->getTutorial()->changeTutorialState(e_Tutorial_State_Container_Menu, this);
	}
#endif

	if(m_bSplitscreen)
	{
		app.AdjustSplitscreenScene(m_hObj,&m_OriginalPosition,m_iPad);
	}

	InitDataAssociations(initData->iPad, menu);

	CXuiSceneAbstractContainer::Initialize( initData->iPad, menu, true, BeaconMenu::INV_SLOT_START, (ESceneSection)0, eSectionBeaconMax );

	m_bIgnoreKeyPresses = false;

	m_initPowerButtons = true;

	XuiSetTimer( m_hObj, BEACON_TICK_TIMER_ID, 50 );

	handleTick();

	delete initData;

	return S_OK;
}

HRESULT CXuiSceneBeacon::OnDestroy()
{
	Minecraft *pMinecraft = Minecraft::GetInstance();

#ifdef _XBOX
	if( pMinecraft->localgameModes[m_iPad] != NULL )
	{
		TutorialMode *gameMode = (TutorialMode *)pMinecraft->localgameModes[m_iPad];
		if(gameMode != NULL) gameMode->getTutorial()->changeTutorialState(m_previousTutorialState);
	}
#endif

	if(Minecraft::GetInstance()->localplayers[m_iPad] != NULL) Minecraft::GetInstance()->localplayers[m_iPad]->closeContainer();
	return S_OK;
}

CXuiControl* CXuiSceneBeacon::GetSectionControl( ESceneSection eSection )
{
	switch( eSection )
	{
		case eSectionBeaconItem:
			return (CXuiControl *)m_paymentControl;
			break;
		case eSectionBeaconInventory:
			return (CXuiControl *)m_inventoryControl;
			break;
		case eSectionBeaconUsing:
			return (CXuiControl *)m_useRowControl;
			break;
		case eSectionBeaconPrimaryTierOneOne:
			return (CXuiControl *)m_primaryTierOneOne;
			break;
		case eSectionBeaconPrimaryTierOneTwo:
			return (CXuiControl *)m_primaryTierOneTwo;
			break;
		case eSectionBeaconPrimaryTierTwoOne:
			return (CXuiControl *)m_primaryTierTwoOne;
			break;
		case eSectionBeaconPrimaryTierTwoTwo:
			return (CXuiControl *)m_primaryTierTwoTwo;
			break;
		case eSectionBeaconPrimaryTierThree:
			return (CXuiControl *)m_primaryTierThree;
			break;
		case eSectionBeaconSecondaryOne:
			return (CXuiControl *)m_secondaryOne;
			break;
		case eSectionBeaconSecondaryTwo:
			return (CXuiControl *)m_secondaryTwo;
			break;
		case eSectionBeaconConfirm:
			return (CXuiControl *)m_confirmButton;
			break;
		default:
			break;
	}
	return NULL;
}

CXuiCtrlSlotList* CXuiSceneBeacon::GetSectionSlotList( ESceneSection eSection )
{
	switch( eSection )
	{
		case eSectionBeaconItem:
			return m_paymentControl;
			break;
		case eSectionBeaconInventory:
			return m_inventoryControl;
			break;
		case eSectionBeaconUsing:
			return m_useRowControl;
			break;
		default:
			break;
	}
	return NULL;
}

void CXuiSceneBeacon::InitDataAssociations(int iPad, AbstractContainerMenu *menu, int startIndex)
{
	m_paymentControl->SetData( iPad, menu, 1, 1, BeaconMenu::PAYMENT_SLOT );

	int invRows = (BeaconMenu::INV_SLOT_END - BeaconMenu::INV_SLOT_START) / 9;
	m_inventoryControl->SetData( iPad, menu, invRows, 9, BeaconMenu::INV_SLOT_START );

	m_useRowControl->SetData( iPad, menu, 1, 9, BeaconMenu::USE_ROW_SLOT_START );

	CXuiSceneAbstractContainer::InitDataAssociations(iPad, menu, BeaconMenu::INV_SLOT_START);
}

void CXuiSceneBeacon::SetConfirmButtonEnabled(bool enabled)
{
	if(m_confirmButton != NULL)
		m_confirmButton->SetButtonActive(enabled);
}

void CXuiSceneBeacon::AddPowerButton(int id, int icon, int tier, int count, bool active, bool selected)
{
	switch(tier)
	{
	case 0:
		if(count == 0)
		{
			if(m_primaryTierOneOne != NULL) m_primaryTierOneOne->SetData(id, icon, active, selected);
		}
		else
		{
			if(m_primaryTierOneTwo != NULL) m_primaryTierOneTwo->SetData(id, icon, active, selected);
		}
		break;
	case 1:
		if(count == 0)
		{
			if(m_primaryTierTwoOne != NULL) m_primaryTierTwoOne->SetData(id, icon, active, selected);
		}
		else
		{
			if(m_primaryTierTwoTwo != NULL) m_primaryTierTwoTwo->SetData(id, icon, active, selected);
		}
		break;
	case 2:
		if(m_primaryTierThree != NULL) m_primaryTierThree->SetData(id, icon, active, selected);
		break;
	case 3:
		if(count == 0)
		{
			if(m_secondaryOne != NULL) m_secondaryOne->SetData(id, icon, active, selected);
		}
		else
		{
			if(m_secondaryTwo != NULL) m_secondaryTwo->SetData(id, icon, active, selected);
		}
		break;
	};
}

int CXuiSceneBeacon::GetPowerButtonId(ESceneSection eSection)
{
	switch(eSection)
	{
	case eSectionBeaconPrimaryTierOneOne:
		return m_primaryTierOneOne != NULL ? m_primaryTierOneOne->GetData() : 0;
	case eSectionBeaconPrimaryTierOneTwo:
		return m_primaryTierOneTwo != NULL ? m_primaryTierOneTwo->GetData() : 0;
	case eSectionBeaconPrimaryTierTwoOne:
		return m_primaryTierTwoOne != NULL ? m_primaryTierTwoOne->GetData() : 0;
	case eSectionBeaconPrimaryTierTwoTwo:
		return m_primaryTierTwoTwo != NULL ? m_primaryTierTwoTwo->GetData() : 0;
	case eSectionBeaconPrimaryTierThree:
		return m_primaryTierThree != NULL ? m_primaryTierThree->GetData() : 0;
	case eSectionBeaconSecondaryOne:
		return m_secondaryOne != NULL ? m_secondaryOne->GetData() : 0;
	case eSectionBeaconSecondaryTwo:
		return m_secondaryTwo != NULL ? m_secondaryTwo->GetData() : 0;
	};
	return 0;
}

bool CXuiSceneBeacon::IsPowerButtonSelected(ESceneSection eSection)
{
	switch(eSection)
	{
	case eSectionBeaconPrimaryTierOneOne:
		return m_primaryTierOneOne != NULL ? m_primaryTierOneOne->IsButtonSelected() : false;
	case eSectionBeaconPrimaryTierOneTwo:
		return m_primaryTierOneTwo != NULL ? m_primaryTierOneTwo->IsButtonSelected() : false;
	case eSectionBeaconPrimaryTierTwoOne:
		return m_primaryTierTwoOne != NULL ? m_primaryTierTwoOne->IsButtonSelected() : false;
	case eSectionBeaconPrimaryTierTwoTwo:
		return m_primaryTierTwoTwo != NULL ? m_primaryTierTwoTwo->IsButtonSelected() : false;
	case eSectionBeaconPrimaryTierThree:
		return m_primaryTierThree != NULL ? m_primaryTierThree->IsButtonSelected() : false;
	case eSectionBeaconSecondaryOne:
		return m_secondaryOne != NULL ? m_secondaryOne->IsButtonSelected() : false;
	case eSectionBeaconSecondaryTwo:
		return m_secondaryTwo != NULL ? m_secondaryTwo->IsButtonSelected() : false;
	};
	return false;
}

void CXuiSceneBeacon::SetPowerButtonSelected(ESceneSection eSection)
{
	switch(eSection)
	{
	case eSectionBeaconPrimaryTierOneOne:
	case eSectionBeaconPrimaryTierOneTwo:
	case eSectionBeaconPrimaryTierTwoOne:
	case eSectionBeaconPrimaryTierTwoTwo:
	case eSectionBeaconPrimaryTierThree:
		if(m_primaryTierOneOne != NULL) m_primaryTierOneOne->SetButtonSelected(false);
		if(m_primaryTierOneTwo != NULL) m_primaryTierOneTwo->SetButtonSelected(false);
		if(m_primaryTierTwoOne != NULL) m_primaryTierTwoOne->SetButtonSelected(false);
		if(m_primaryTierTwoTwo != NULL) m_primaryTierTwoTwo->SetButtonSelected(false);
		if(m_primaryTierThree != NULL) m_primaryTierThree->SetButtonSelected(false);
		break;
	case eSectionBeaconSecondaryOne:
	case eSectionBeaconSecondaryTwo:
		if(m_secondaryOne != NULL) m_secondaryOne->SetButtonSelected(false);
		if(m_secondaryTwo != NULL) m_secondaryTwo->SetButtonSelected(false);
		break;
	};

	switch(eSection)
	{
	case eSectionBeaconPrimaryTierOneOne:
		if(m_primaryTierOneOne != NULL) m_primaryTierOneOne->SetButtonSelected(true);
		break;
	case eSectionBeaconPrimaryTierOneTwo:
		if(m_primaryTierOneTwo != NULL) m_primaryTierOneTwo->SetButtonSelected(true);
		break;
	case eSectionBeaconPrimaryTierTwoOne:
		if(m_primaryTierTwoOne != NULL) m_primaryTierTwoOne->SetButtonSelected(true);
		break;
	case eSectionBeaconPrimaryTierTwoTwo:
		if(m_primaryTierTwoTwo != NULL) m_primaryTierTwoTwo->SetButtonSelected(true);
		break;
	case eSectionBeaconPrimaryTierThree:
		if(m_primaryTierThree != NULL) m_primaryTierThree->SetButtonSelected(true);
		break;
	case eSectionBeaconSecondaryOne:
		if(m_secondaryOne != NULL) m_secondaryOne->SetButtonSelected(true);
		break;
	case eSectionBeaconSecondaryTwo:
		if(m_secondaryTwo != NULL) m_secondaryTwo->SetButtonSelected(true);
		break;
	};
}

HRESULT CXuiSceneBeacon::handleCustomTimer( XUIMessageTimer *pTimer, BOOL& bHandled )
{
	if( pTimer->nId == BEACON_TICK_TIMER_ID )
	{
		handleTick();
		bHandled = TRUE;
		return S_OK;
	}

	return CXuiSceneAbstractContainer::handleCustomTimer( pTimer, bHandled );
}

void CXuiSceneBeacon::handleTick()
{
	IUIScene_BeaconMenu::handleTick();
}
