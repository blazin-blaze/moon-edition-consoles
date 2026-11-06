#include "stdafx.h"

#include "..\..\..\Minecraft.World\HorseInventoryMenu.h"
#include "..\..\..\Minecraft.World\EntityHorse.h"
#include "..\..\MultiplayerLocalPlayer.h"
#include "XUI_Ctrl_SlotList.h"
#include "XUI_Scene_HorseInventory.h"
#include "..\..\Common\Tutorial\Tutorial.h"
#include "..\..\Common\Tutorial\TutorialMode.h"
#include "..\..\Common\Tutorial\TutorialEnum.h"

HRESULT CXuiSceneHorseInventory::OnInit( XUIMessageInit* pInitData, BOOL& bHandled )
{
	D3DXVECTOR3 vec;
	MapChildControls();

	Minecraft *pMinecraft = Minecraft::GetInstance();

	HorseScreenInput* initData = (HorseScreenInput*)pInitData->pvInitData;

	XuiControlSetText(m_HorseText, initData->container->getName().c_str());

	m_iPad=initData->iPad;
	m_horse = initData->horse;
	m_inventory = initData->inventory;
	m_bSplitscreen=initData->bSplitscreen;

#ifdef _XBOX
	if( pMinecraft->localgameModes[initData->iPad] != NULL )
	{
		TutorialMode *gameMode = (TutorialMode *)pMinecraft->localgameModes[initData->iPad];
		m_previousTutorialState = gameMode->getTutorial()->getCurrentState();
		gameMode->getTutorial()->changeTutorialState(e_Tutorial_State_Horse_Menu, this);
	}
#endif

	if(m_bSplitscreen)
	{
		app.AdjustSplitscreenScene(m_hObj,&m_OriginalPosition,m_iPad);
	}

	HorseInventoryMenu* menu = new HorseInventoryMenu(initData->inventory, initData->container, initData->horse);

	InitDataAssociations(initData->iPad, menu);

	int startSlot = EntityHorse::INV_BASE_COUNT;
	if (m_horse->isChestedHorse())
	{
		startSlot += EntityHorse::INV_DONKEY_CHEST_COUNT;
	}
	CXuiSceneAbstractContainer::Initialize( initData->iPad, menu, true, startSlot, eSectionHorseUsing, eSectionHorseMax );

	// remove horse inventory
	if (!m_horse->isChestedHorse())
		m_chestControl->SetShow(FALSE);
	
	// cannot wear armor? remove armor slot!
	if (!m_horse->canWearArmor())
		m_armorControl->SetShow(FALSE);

	delete initData;

	return S_OK;
}

HRESULT CXuiSceneHorseInventory::OnDestroy()
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

CXuiControl* CXuiSceneHorseInventory::GetSectionControl( ESceneSection eSection )
{
	switch( eSection )
	{
		case eSectionHorseChest:
			return (CXuiControl *)m_chestControl;
			break;
		case eSectionHorseSaddle:
			return (CXuiControl *)m_saddleControl;
			break;
		case eSectionHorseArmor:
			return (CXuiControl *)m_armorControl;
			break;
		case eSectionHorseInventory:
			return (CXuiControl *)m_inventoryControl;
			break;
		case eSectionHorseUsing:
			return (CXuiControl *)m_useRowControl;
			break;
		default:
			assert( false );
			break;
	}
	return NULL;
}

CXuiCtrlSlotList* CXuiSceneHorseInventory::GetSectionSlotList( ESceneSection eSection )
{
	switch( eSection )
	{
		case eSectionHorseChest:
			return m_chestControl;
			break;
		case eSectionHorseSaddle:
			return m_saddleControl;
			break;
		case eSectionHorseArmor:
			return m_armorControl;
			break;
		case eSectionHorseInventory:
			return m_inventoryControl;
			break;
		case eSectionHorseUsing:
			return m_useRowControl;
			break;
		default:
			assert( false );
			break;
	}
	return NULL;
}

void CXuiSceneHorseInventory::InitDataAssociations(int iPad, AbstractContainerMenu *menu, int startIndex)
{
	m_saddleControl->SetData( iPad, menu, 1, 1, EntityHorse::INV_SLOT_SADDLE );

	m_armorControl->SetData( iPad, menu, 1, 1, EntityHorse::INV_SLOT_ARMOR );

	int chestCount = 0;
	if(m_horse != NULL && m_horse->isChestedHorse())
	{
		chestCount = EntityHorse::INV_DONKEY_CHEST_COUNT;
	}
	m_chestControl->SetData( iPad, menu, chestCount / 9, 9, EntityHorse::INV_BASE_COUNT );

	int invStart = EntityHorse::INV_BASE_COUNT + chestCount;
	m_inventoryControl->SetData( iPad, menu, 3, 9, invStart );

	m_useRowControl->SetData( iPad, menu, 1, 9, invStart + (3 * 9));

	CXuiSceneAbstractContainer::InitDataAssociations(iPad, menu, EntityHorse::INV_BASE_COUNT + chestCount);
}
