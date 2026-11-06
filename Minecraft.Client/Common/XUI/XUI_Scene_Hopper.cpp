#include "stdafx.h"

#include "..\..\..\Minecraft.World\HopperTileEntity.h"
#include "..\..\..\Minecraft.World\HopperMenu.h"
#include "..\..\MultiplayerLocalPlayer.h"
#include "XUI_Ctrl_SlotList.h"
#include "XUI_Scene_Hopper.h"
#include "..\..\Common\Tutorial\Tutorial.h"
#include "..\..\Common\Tutorial\TutorialMode.h"
#include "..\..\Common\Tutorial\TutorialEnum.h"

HRESULT CXuiSceneHopper::OnInit( XUIMessageInit* pInitData, BOOL& bHandled )
{
	D3DXVECTOR3 vec;
	MapChildControls();

	XuiControlSetText(m_HopperText, app.GetString(IDS_CONTAINER_HOPPER));

	Minecraft *pMinecraft = Minecraft::GetInstance();

	HopperScreenInput* initData = (HopperScreenInput*)pInitData->pvInitData;
	m_iPad=initData->iPad;
	m_bSplitscreen=initData->bSplitscreen;

#ifdef _XBOX
	if( pMinecraft->localgameModes[initData->iPad] != NULL )
	{
		TutorialMode *gameMode = (TutorialMode *)pMinecraft->localgameModes[initData->iPad];
		m_previousTutorialState = gameMode->getTutorial()->getCurrentState();
		gameMode->getTutorial()->changeTutorialState(e_Tutorial_State_Hopper_Menu, this);
	}
#endif

	if(m_bSplitscreen)
	{
		app.AdjustSplitscreenScene(m_hObj,&m_OriginalPosition,m_iPad);
	}

	HopperMenu* menu = new HopperMenu(initData->inventory, initData->hopper);

	InitDataAssociations(initData->iPad, menu);

	CXuiSceneAbstractContainer::Initialize( initData->iPad, menu, true, initData->hopper->getContainerSize(), eSectionHopperUsing, eSectionHopperMax );

	delete initData;

	return S_OK;
}

HRESULT CXuiSceneHopper::OnDestroy()
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

CXuiControl* CXuiSceneHopper::GetSectionControl( ESceneSection eSection )
{
	switch( eSection )
	{
		case eSectionHopperContents:
			return (CXuiControl *)m_hopperControl;
			break;
		case eSectionHopperInventory:
			return (CXuiControl *)m_inventoryControl;
			break;
		case eSectionHopperUsing:
			return (CXuiControl *)m_useRowControl;
			break;
		default:
			assert( false );
			break;
	}
	return NULL;
}

CXuiCtrlSlotList* CXuiSceneHopper::GetSectionSlotList( ESceneSection eSection )
{
	switch( eSection )
	{
		case eSectionHopperContents:
			return m_hopperControl;
			break;
		case eSectionHopperInventory:
			return m_inventoryControl;
			break;
		case eSectionHopperUsing:
			return m_useRowControl;
			break;
		default:
			assert( false );
			break;
	}
	return NULL;
}

void CXuiSceneHopper::InitDataAssociations(int iPad, AbstractContainerMenu *menu, int startIndex)
{
	int containerSize = menu->getSize() - (27 + 9);

	m_hopperControl->SetData( iPad, menu, 1, 5, 0 );

	CXuiSceneAbstractContainer::InitDataAssociations(iPad, menu, containerSize);
}
