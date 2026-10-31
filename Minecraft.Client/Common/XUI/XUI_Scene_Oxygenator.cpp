#include "stdafx.h"

#include "..\..\..\Minecraft.World\net.minecraft.world.inventory.h"
#include "..\..\MultiPlayerLocalPlayer.h"
#include "..\..\Common\Tutorial\Tutorial.h"
#include "..\..\Common\Tutorial\TutorialMode.h"
#include "..\..\Common\Tutorial\TutorialEnum.h"
#include "..\..\Minecraft.h"
#include "XUI_Ctrl_SlotList.h"
#include "XUI_Scene_Oxygenator.h"
#include "XUI_Ctrl_BubblesProgress.h"


//--------------------------------------------------------------------------------------
// Name: CXuiSceneOxygenator::OnInit
// Desc: Message handler for XM_INIT
//--------------------------------------------------------------------------------------
HRESULT CXuiSceneOxygenator::OnInit( XUIMessageInit* pInitData, BOOL& bHandled )
{
	MapChildControls();
	
	XuiControlSetText(m_OxygenatorText,app.GetString(IDS_OXYGENATOR));

	Minecraft *pMinecraft = Minecraft::GetInstance();

	OxygenatorScreenInput* initData = (OxygenatorScreenInput*)pInitData->pvInitData;
	m_iPad=initData->iPad;
	m_bSplitscreen=initData->bSplitscreen;

	// if we are in splitscreen, then we need to figure out if we want to move this scene

	if(m_bSplitscreen)
	{
		app.AdjustSplitscreenScene(m_hObj,&m_OriginalPosition,m_iPad);	
	}

	OxygenatorMenu* menu = new OxygenatorMenu( initData->inventory, initData->oxygenator );
	

	InitDataAssociations(m_iPad, menu);
	
	m_bubbleProgress->isOxygenator = true;
	m_bubbleProgress->SetUserData( initData->oxygenator.get() );

	delete initData;

	CXuiSceneAbstractContainer::Initialize( m_iPad, menu, true, OxygenatorMenu::INV_SLOT_START, eSectionOxygenatorUsing, eSectionOxygenatorMax );
	
	//app.SetRichPresenceContextValue(m_iPad,CONTEXT_GAME_STATE_FORGING);

	return S_OK;
}

HRESULT CXuiSceneOxygenator::OnDestroy()
{
	Minecraft *pMinecraft = Minecraft::GetInstance();

	// 4J Stu - Fix for #11302 - TCR 001: Network Connectivity: Host crashed after being killed by the client while accessing a chest during burst packet loss.
	// We need to make sure that we call closeContainer() anytime this menu is closed, even if it is forced to close by some other reason (like the player dying)	
	if(Minecraft::GetInstance()->localplayers[m_iPad] != NULL) Minecraft::GetInstance()->localplayers[m_iPad]->closeContainer();
	return S_OK;
}

CXuiControl* CXuiSceneOxygenator::GetSectionControl( ESceneSection eSection )
{
	switch( eSection )
	{
		case eSectionOxygenatorIngredient:
			return (CXuiControl *)m_ingredientControl;
			break;
		case eSectionOxygenatorInventory:
			return (CXuiControl *)m_inventoryControl;
			break;
		case eSectionOxygenatorUsing:
			return (CXuiControl *)m_useRowControl;
			break;
		default:
			assert( false );
			break;
	}
	return NULL;
}

CXuiCtrlSlotList* CXuiSceneOxygenator::GetSectionSlotList( ESceneSection eSection )
{
	switch( eSection )
	{
		case eSectionOxygenatorIngredient:
			return m_ingredientControl;
			break;
		case eSectionOxygenatorInventory:
			return m_inventoryControl;
			break;
		case eSectionOxygenatorUsing:
			return m_useRowControl;
			break;
		default:
			assert( false );
			break;
	}
	return NULL;
}

// 4J Stu - Added to support auto-save. Need to re-associate on a navigate back
void CXuiSceneOxygenator::InitDataAssociations(int iPad, AbstractContainerMenu *menu, int startIndex /*= 0*/)
{
	// TODO Inventory dimensions need defined as constants
	m_ingredientControl->SetData( iPad, menu, 1, 1, OxygenatorMenu::INGREDIENT_SLOT );

	//m_litProgressControl->SetUserData( initData->furnace.get() );
	
	//m_burnProgress->SetUserData( initData->furnace.get() );

	CXuiSceneAbstractContainer::InitDataAssociations(iPad, menu, OxygenatorMenu::INV_SLOT_START);
}