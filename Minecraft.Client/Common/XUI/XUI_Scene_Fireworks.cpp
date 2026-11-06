#include "stdafx.h"

#include "..\..\..\Minecraft.World\Container.h"
#include "..\..\..\Minecraft.World\FireworksMenu.h"
#include "..\..\MultiplayerLocalPlayer.h"
#include "XUI_Ctrl_SlotList.h"
#include "XUI_Scene_Fireworks.h"
#include "..\..\Common\Tutorial\Tutorial.h"
#include "..\..\Common\Tutorial\TutorialMode.h"
#include "..\..\Common\Tutorial\TutorialEnum.h"

HRESULT CXuiSceneFireworks::OnInit( XUIMessageInit* pInitData, BOOL& bHandled )
{
	D3DXVECTOR3 vec;
	MapChildControls();

	XuiControlSetText(m_FireworksText, app.GetString(IDS_FIREWORKS));

	Minecraft *pMinecraft = Minecraft::GetInstance();

	FireworksScreenInput* initData = (FireworksScreenInput*)pInitData->pvInitData;
	m_iPad=initData->iPad;
	m_bSplitscreen=initData->bSplitscreen;

#ifdef _XBOX
	if( pMinecraft->localgameModes[initData->iPad] != NULL )
	{
		TutorialMode *gameMode = (TutorialMode *)pMinecraft->localgameModes[initData->iPad];
		m_previousTutorialState = gameMode->getTutorial()->getCurrentState();
		gameMode->getTutorial()->changeTutorialState(e_Tutorial_State_Fireworks_Menu, this);
	}
#endif

	if(m_bSplitscreen)
	{
		app.AdjustSplitscreenScene(m_hObj,&m_OriginalPosition,m_iPad);
	}

	FireworksMenu* menu = new FireworksMenu(initData->player->inventory, initData->player->level, initData->x, initData->y, initData->z);

	InitDataAssociations(initData->iPad, menu);

	CXuiSceneAbstractContainer::Initialize( initData->iPad, menu, true, FireworksMenu::INV_SLOT_START, eSectionFireworksUsing, eSectionFireworksMax );

	delete initData;

	return S_OK;
}

HRESULT CXuiSceneFireworks::OnDestroy()
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

CXuiControl* CXuiSceneFireworks::GetSectionControl( ESceneSection eSection )
{
	switch( eSection )
	{
		case eSectionFireworksResult:
			return (CXuiControl *)m_resultControl;
			break;
		case eSectionFireworksIngredients:
			return (CXuiControl *)m_ingredientsControl;
			break;
		case eSectionFireworksInventory:
			return (CXuiControl *)m_inventoryControl;
			break;
		case eSectionFireworksUsing:
			return (CXuiControl *)m_useRowControl;
			break;
		default:
			assert( false );
			break;
	}
	return NULL;
}

CXuiCtrlSlotList* CXuiSceneFireworks::GetSectionSlotList( ESceneSection eSection )
{
	switch( eSection )
	{
		case eSectionFireworksResult:
			return m_resultControl;
			break;
		case eSectionFireworksIngredients:
			return m_ingredientsControl;
			break;
		case eSectionFireworksInventory:
			return m_inventoryControl;
			break;
		case eSectionFireworksUsing:
			return m_useRowControl;
			break;
		default:
			assert( false );
			break;
	}
	return NULL;
}

void CXuiSceneFireworks::InitDataAssociations(int iPad, AbstractContainerMenu *menu, int startIndex)
{
	int containerSize = menu->getSize() - (27 + 9);

	m_resultControl->SetData( iPad, menu, 1, 1, FireworksMenu::RESULT_SLOT );
	m_ingredientsControl->SetData( iPad, menu, 3, 3, FireworksMenu::CRAFT_SLOT_START );

	CXuiSceneAbstractContainer::InitDataAssociations(iPad, menu, containerSize);
}
