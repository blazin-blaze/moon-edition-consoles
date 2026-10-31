#include "stdafx.h"

#include "..\..\..\Minecraft.World\Container.h"
#include "..\..\..\Minecraft.World\HopperMenu.h"
#include "..\..\MultiplayerLocalPlayer.h"
#include "XUI_Ctrl_SlotList.h"
#include "XUI_Scene_Hopper.h"
#include "XUI_Ctrl_SlotItemListItem.h"
#include "XUI_Ctrl_SlotItem.h"
#include "..\..\Common\Tutorial\Tutorial.h"
#include "..\..\Common\Tutorial\TutorialMode.h"
#include "..\..\Common\Tutorial\TutorialEnum.h"

#define HOPPER_DEFAULT_ROWS 1

HRESULT CXuiSceneHopper::OnInit( XUIMessageInit* pInitData, BOOL& bHandled )
{
	D3DXVECTOR3 vec;
	MapChildControls();

	Minecraft *pMinecraft = Minecraft::GetInstance();

	HopperScreenInput* initData = (HopperScreenInput*)pInitData->pvInitData;

	HopperMenu* menu = new HopperMenu( initData->inventory, initData->hopper );

	shared_ptr<Container> hopper = initData->hopper;
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

	float fPointerWidth,fPointerHeight;
	m_pointerControl->GetBounds(&fPointerWidth, &fPointerHeight);

	float height, width;
	this->GetBounds( &width, &height );
	int rowDiff = HOPPER_DEFAULT_ROWS - (hopper->getContainerSize() / 5);
	height = height - (rowDiff * fPointerHeight);
	this->SetBounds( width, height );

	D3DXVECTOR3 vPos;
	this->GetPosition( &vPos );
	vPos.y = vPos.y + ( (rowDiff * fPointerHeight) / 2 );
	if(!RenderManager.IsHiDef())
	{
		int iY = (int)(vPos.y);
		iY &= 0xfffffffe;
		vPos.y = (float)iY;
	}
	this->SetPosition( &vPos );

	InitDataAssociations(initData->iPad, menu);

	CXuiSceneAbstractContainer::Initialize( initData->iPad, menu, true, hopper->getContainerSize(), (ESceneSection)0, eSectionHopperMax );

	m_bIgnoreKeyPresses = false;

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
	int containerSize = menu->getSize() - 36;
	if (containerSize <= 0) containerSize = 5;
	int rows = (containerSize + 4) / 5;

	m_hopperControl->SetData( iPad, menu, rows, 5, 0 );

	CXuiSceneAbstractContainer::InitDataAssociations(iPad, menu, containerSize);
}
