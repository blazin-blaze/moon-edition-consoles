#include "stdafx.h"
#include "UI.h"
#include "..\..\Minecraft.h"
#include "..\..\MultiplayerLocalPlayer.h"
#include "..\..\..\Minecraft.World\net.minecraft.world.inventory.h"
#include "UIScene_SpaceCraftingMenu.h"

#ifdef __PSVITA__
#define GAME_CRAFTING_TOUCHUPDATE_TIMER_ID 0
#define GAME_CRAFTING_TOUCHUPDATE_TIMER_TIME 100
#endif

UIScene_SpaceCraftingMenu::UIScene_SpaceCraftingMenu(int iPad, void *_initData, UILayer *parentLayer) : UIScene(iPad, parentLayer)
{
	m_bIgnoreKeyPresses = false;

	CraftingPanelScreenInput* initData = (CraftingPanelScreenInput*)_initData;
	m_iContainerType=initData->iContainerType;
	m_pPlayer=initData->player;
	m_bSplitscreen=initData->bSplitscreen;

	// Setup all the Iggy references we need for this scene
	initialiseMovie();

	for(unsigned int i = 0; i < 4; ++i)	m_labelIngredientsDesc[i].init(L"");
	m_labelDescription.init(L"");
	m_labelGroupName.init(L"");
	m_labelItemName.init(L"");
	m_labelInventory.init( app.GetString(IDS_INVENTORY) );
	m_labelIngredients.init( app.GetString(IDS_INGREDIENTS) );

	SpaceCraftingMenu *menu = new SpaceCraftingMenu(m_pPlayer->inventory, m_pPlayer->level, initData->x, initData->y, initData->z);
	Minecraft::GetInstance()->localplayers[m_iPad]->containerMenu = menu;

	m_menu = menu;
	m_iMenuInventoryStart = SpaceCraftingMenu::INV_SLOT_START;
	m_iMenuHotBarStart = SpaceCraftingMenu::USE_ROW_SLOT_START;
	m_slotListInventory.addSlots(CRAFTING_INVENTORY_SLOT_START,CRAFTING_INVENTORY_SLOT_END - CRAFTING_INVENTORY_SLOT_START);
	m_slotListHotBar.addSlots(CRAFTING_HOTBAR_SLOT_START, CRAFTING_HOTBAR_SLOT_END - CRAFTING_HOTBAR_SLOT_START);

	m_iIngredientsMaxSlotC = m_iIngredients3x3SlotC;
	m_pGroupA=(SpaceRecipy::_eGroupType *)&m_GroupTypeMapping9GridA;
	m_pGroupTabA=(_eGroupTab *)&m_GroupTabBkgMapping3x3A;
	m_iCraftablesMaxHSlotC=m_iMaxHSlot3x3C;

	app.SetRichPresenceContext(m_iPad,CONTEXT_GAME_STATE_CRAFTING);
	setGroupText(GetGroupNameText(m_pGroupA[m_iGroupIndex]));

	for(unsigned int i = 0; i < 4; ++i)
	{
		m_slotListIngredients[i].addSlot(CRAFTING_INGREDIENTS_DESCRIPTION_START + i);
	}
	m_slotListCraftingOutput.addSlot(CRAFTING_OUTPUT_SLOT_START);
	m_slotListIngredientsLayout.addSlots(CRAFTING_INGREDIENTS_LAYOUT_START, m_iIngredientsMaxSlotC);

	// 3 Slot vertical scroll
	m_slotListCrafting3VSlots[0].addSlot(CRAFTING_V_SLOT_START + 0);
	m_slotListCrafting3VSlots[1].addSlot(CRAFTING_V_SLOT_START + 1);
	m_slotListCrafting3VSlots[2].addSlot(CRAFTING_V_SLOT_START + 2);

	// 2 Slot vertical scroll
	// 2 slot scroll has swapped order
	m_slotListCrafting2VSlots[0].addSlot(CRAFTING_V_SLOT_START + 1);
	m_slotListCrafting2VSlots[1].addSlot(CRAFTING_V_SLOT_START + 0);

	// 1 Slot scroll (for 480 mainly)
	m_slotListCrafting1VSlots.addSlot(CRAFTING_V_SLOT_START);

	m_slotListCraftingHSlots.addSlots(CRAFTING_H_SLOT_START,m_iCraftablesMaxHSlotC);

	// Check which recipes are available with the resources we have
	CheckRecipesAvailable();
	// reset the vertical slots
	iVSlotIndexA[0]=CanBeMadeA[m_iCurrentSlotHIndex].iCount-1;
	iVSlotIndexA[1]=0;
	iVSlotIndexA[2]=1;
	UpdateVerticalSlots();
	UpdateHighlight();

	if(initData) delete initData;

	// in this scene, we override the press sound with our own for crafting success or fail
	ui.OverrideSFX(m_iPad,ACTION_MENU_A,true);
	ui.OverrideSFX(m_iPad,ACTION_MENU_OK,true);
#ifdef __ORBIS__
	ui.OverrideSFX(m_iPad,ACTION_MENU_TOUCHPAD_PRESS,true);
#endif
	ui.OverrideSFX(m_iPad,ACTION_MENU_LEFT_SCROLL,true);
	ui.OverrideSFX(m_iPad,ACTION_MENU_RIGHT_SCROLL,true);
	ui.OverrideSFX(m_iPad,ACTION_MENU_LEFT,true);
	ui.OverrideSFX(m_iPad,ACTION_MENU_RIGHT,true);
	ui.OverrideSFX(m_iPad,ACTION_MENU_UP,true);
	ui.OverrideSFX(m_iPad,ACTION_MENU_DOWN,true);

	// 4J-PB - Must be after the CanBeMade list has been set up with CheckRecipesAvailable
	UpdateTooltips();

#ifdef __PSVITA__
	// initialise vita touch controls with ids
	for(unsigned int i = 0; i < ETouchInput_Count; ++i)
	{
		m_TouchInput[i].init(i);
	}
	ui.TouchBoxRebuild(this);
#endif
}

void UIScene_SpaceCraftingMenu::handleDestroy()
{
	Minecraft *pMinecraft = Minecraft::GetInstance();

	// We need to make sure that we call closeContainer() anytime this menu is closed, even if it is forced to close by some other reason (like the player dying)	
	if(Minecraft::GetInstance()->localplayers[m_iPad] != NULL && Minecraft::GetInstance()->localplayers[m_iPad]->containerMenu->containerId == m_menu->containerId)
	{
		Minecraft::GetInstance()->localplayers[m_iPad]->closeContainer();
	}

	ui.OverrideSFX(m_iPad,ACTION_MENU_A,false);
	ui.OverrideSFX(m_iPad,ACTION_MENU_OK,false);
#ifdef __ORBIS__
	ui.OverrideSFX(m_iPad,ACTION_MENU_TOUCHPAD_PRESS,false);
#endif
	ui.OverrideSFX(m_iPad,ACTION_MENU_LEFT_SCROLL,false);
	ui.OverrideSFX(m_iPad,ACTION_MENU_RIGHT_SCROLL,false);
	ui.OverrideSFX(m_iPad,ACTION_MENU_LEFT,false);
	ui.OverrideSFX(m_iPad,ACTION_MENU_RIGHT,false);
	ui.OverrideSFX(m_iPad,ACTION_MENU_UP,false);
	ui.OverrideSFX(m_iPad,ACTION_MENU_DOWN,false);
}

EUIScene UIScene_SpaceCraftingMenu::getSceneType()
{
	return eUIScene_SpaceCrafting3x3Menu;
}

wstring UIScene_SpaceCraftingMenu::getMoviePath()
{
	if(app.GetLocalPlayerCount() > 1)
	{
		m_bSplitscreen = true;
		return L"SpaceCrafting3x3MenuSplit";
	}
	else
	{
		return L"SpaceCrafting3x3Menu";
	}
}

void UIScene_SpaceCraftingMenu::handleReload()
{
	m_slotListInventory.addSlots(CRAFTING_INVENTORY_SLOT_START,CRAFTING_INVENTORY_SLOT_END - CRAFTING_INVENTORY_SLOT_START);
	m_slotListHotBar.addSlots(CRAFTING_HOTBAR_SLOT_START, CRAFTING_HOTBAR_SLOT_END - CRAFTING_HOTBAR_SLOT_START);

	for(unsigned int i = 0; i < 4; ++i)
	{
		m_slotListIngredients[i].addSlot(CRAFTING_INGREDIENTS_DESCRIPTION_START + i);
	}
	m_slotListCraftingOutput.addSlot(CRAFTING_OUTPUT_SLOT_START);
	m_slotListIngredientsLayout.addSlots(CRAFTING_INGREDIENTS_LAYOUT_START, m_iIngredientsMaxSlotC);

	// 3 Slot vertical scroll
	m_slotListCrafting3VSlots[0].addSlot(CRAFTING_V_SLOT_START + 0);
	m_slotListCrafting3VSlots[1].addSlot(CRAFTING_V_SLOT_START + 1);
	m_slotListCrafting3VSlots[2].addSlot(CRAFTING_V_SLOT_START + 2);

	// 2 Slot vertical scroll
	// 2 slot scroll has swapped order
	m_slotListCrafting2VSlots[0].addSlot(CRAFTING_V_SLOT_START + 1);
	m_slotListCrafting2VSlots[1].addSlot(CRAFTING_V_SLOT_START + 0);

	// 1 Slot scroll (for 480 mainly)
	m_slotListCrafting1VSlots.addSlot(CRAFTING_V_SLOT_START);

	m_slotListCraftingHSlots.addSlots(CRAFTING_H_SLOT_START,m_iCraftablesMaxHSlotC);

	app.DebugPrintf(app.USER_SR,"Reloading MultiPanel\n");
	int temp = m_iDisplayDescription;
	m_iDisplayDescription = m_iDisplayDescription==0?1:0;
	UpdateMultiPanel();
	m_iDisplayDescription = temp;
	UpdateMultiPanel();

	app.DebugPrintf(app.USER_SR,"Reloading Highlight and scroll\n");

	// reset the vertical slots
	m_iCurrentSlotHIndex = 0;
	m_iCurrentSlotVIndex = 1;
	iVSlotIndexA[0]=CanBeMadeA[m_iCurrentSlotHIndex].iCount-1;
	iVSlotIndexA[1]=0;
	iVSlotIndexA[2]=1;
	UpdateVerticalSlots();
	UpdateHighlight();

	app.DebugPrintf(app.USER_SR,"Reloading tabs\n");
	showTabHighlight(0,false);
	showTabHighlight(m_iGroupIndex,true);
}

void UIScene_SpaceCraftingMenu::customDraw(IggyCustomDrawCallbackRegion *region)
{
	Minecraft *pMinecraft = Minecraft::GetInstance();
	if(pMinecraft->localplayers[m_iPad] == NULL || pMinecraft->localgameModes[m_iPad] == NULL) return;

	shared_ptr<ItemInstance> item = nullptr;
	int slotId = -1;
	float alpha = 1.0f;
	bool decorations = true;
	bool inventoryItem = false;
	swscanf((wchar_t*)region->name,L"slot_%d",&slotId);
	if (slotId == -1)
	{
		app.DebugPrintf("This is not the control we are looking for\n");
	}
	else if(slotId >= CRAFTING_INVENTORY_SLOT_START && slotId < CRAFTING_INVENTORY_SLOT_END)
	{
		int iIndex = slotId - CRAFTING_INVENTORY_SLOT_START;
		iIndex += m_iMenuInventoryStart;
		Slot *slot = m_menu->getSlot(iIndex);
		item = slot->getItem();
		inventoryItem = true;
	}
	else if(slotId >= CRAFTING_HOTBAR_SLOT_START && slotId < CRAFTING_HOTBAR_SLOT_END)
	{
		int iIndex = slotId - CRAFTING_HOTBAR_SLOT_START;
		iIndex += m_iMenuHotBarStart;
		Slot *slot = m_menu->getSlot(iIndex);
		item = slot->getItem();
		inventoryItem = true;
	}
	else if(slotId >= CRAFTING_V_SLOT_START && slotId < CRAFTING_V_SLOT_END )
	{
		decorations = false;
		int iIndex = slotId - CRAFTING_V_SLOT_START;
		if(m_vSlotsInfo[iIndex].show)
		{
			item = m_vSlotsInfo[iIndex].item;
			alpha = ((float)m_vSlotsInfo[iIndex].alpha)/31.0f;
		}
	}
	else if(slotId >= CRAFTING_H_SLOT_START && slotId < (CRAFTING_H_SLOT_START + m_iCraftablesMaxHSlotC) )
	{
		decorations = false;
		int iIndex = slotId - CRAFTING_H_SLOT_START;
		if(m_hSlotsInfo[iIndex].show)
		{
			item = m_hSlotsInfo[iIndex].item;
			alpha = ((float)m_hSlotsInfo[iIndex].alpha)/31.0f;
		}
	}
	else if(slotId >= CRAFTING_INGREDIENTS_LAYOUT_START && slotId < (CRAFTING_INGREDIENTS_LAYOUT_START + m_iIngredientsMaxSlotC) )
	{
		int iIndex = slotId - CRAFTING_INGREDIENTS_LAYOUT_START;
		if(m_ingredientsSlotsInfo[iIndex].show)
		{
			item = m_ingredientsSlotsInfo[iIndex].item;
			alpha = ((float)m_ingredientsSlotsInfo[iIndex].alpha)/31.0f;
		}
	}
	else if(slotId >= CRAFTING_INGREDIENTS_DESCRIPTION_START && slotId < (CRAFTING_INGREDIENTS_DESCRIPTION_START + 4) )
	{
		int iIndex = slotId - CRAFTING_INGREDIENTS_DESCRIPTION_START;
		if(m_ingredientsInfo[iIndex].show)
		{
			item = m_ingredientsInfo[iIndex].item;
			alpha = ((float)m_ingredientsInfo[iIndex].alpha)/31.0f;
		}
	}
	else if(slotId == CRAFTING_OUTPUT_SLOT_START )
	{
		if(m_craftingOutputSlotInfo.show)
		{
			item = m_craftingOutputSlotInfo.item;
			alpha = ((float)m_craftingOutputSlotInfo.alpha)/31.0f;
		}
	}

	if(item != NULL)
	{
		if(!inventoryItem)
		{
			if( item->id == Item::clock_Id || item->id == Item::compass_Id )
			{
				// 4J Stu - For clocks and compasses we set the aux value to a special one that signals we should use a default texture
				// rather than the dynamic one for the player
				item->setAuxValue(0xFF);
			}
			else if( (item->getAuxValue() & 0xFF) == 0xFF)
			{
				// 4J Stu - If the aux value is set to match any
				item->setAuxValue(0);
			}
		}
		customDrawSlotControl(region,m_iPad,item,alpha,item->isFoil(),decorations);
	}
}

int UIScene_SpaceCraftingMenu::getPad()
{
	return m_iPad;
}

bool UIScene_SpaceCraftingMenu::allowRepeat(int key)
{
	switch(key)
	{
	// X is used to open this menu, so don't let it repeat
	case ACTION_MENU_X:
		return false;
	}
	return true;
}

void UIScene_SpaceCraftingMenu::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	//app.DebugPrintf("UIScene_InventoryMenu handling input for pad %d, key %d, down- %s, pressed- %s, released- %s\n", iPad, key, down?"TRUE":"FALSE", pressed?"TRUE":"FALSE", released?"TRUE":"FALSE");
	ui.AnimateKeyPress(m_iPad, key, repeat, pressed, released);

	switch(key)
	{
	case ACTION_MENU_OTHER_STICK_UP:
	case ACTION_MENU_OTHER_STICK_DOWN:
		sendInputToMovie(key,repeat,pressed,released);
		break;
	default:
		if(pressed)
		{
			handled = handleKeyDown(m_iPad, key, repeat);
		}
		break;
	};
}

void UIScene_SpaceCraftingMenu::hideAllHSlots()
{
	for(unsigned int iIndex = 0; iIndex < m_iMaxHSlotC; ++iIndex)
	{
		m_hSlotsInfo[iIndex].item = nullptr;
		m_hSlotsInfo[iIndex].alpha = 31;
		m_hSlotsInfo[iIndex].show = false;
	}
}

void UIScene_SpaceCraftingMenu::hideAllVSlots()
{
	for(unsigned int iIndex = 0; iIndex < m_iMaxDisplayedVSlotC; ++iIndex)
	{
		m_vSlotsInfo[iIndex].item = nullptr;
		m_vSlotsInfo[iIndex].alpha = 31;
		m_vSlotsInfo[iIndex].show = false;
	}
}

void UIScene_SpaceCraftingMenu::hideAllIngredientsSlots()
{
	for(int i=0;i<m_iIngredientsC;i++)
	{
		m_ingredientsInfo[i].item = nullptr;
		m_ingredientsInfo[i].alpha = 31;
		m_ingredientsInfo[i].show = false;

		m_labelIngredientsDesc[i].setLabel(L"");

		IggyDataValue result;
		IggyDataValue value[2];

		value[0].type = IGGY_DATATYPE_number;
		value[0].number = i;

		value[1].type = IGGY_DATATYPE_boolean;
		value[1].boolval = false;
		IggyResult out = IggyPlayerCallMethodRS ( getMovie() , &result, IggyPlayerRootPath( getMovie() ) , m_funcShowIngredientSlot , 2 , value );
	}
}

void UIScene_SpaceCraftingMenu::setCraftHSlotItem(int iPad, int iIndex, shared_ptr<ItemInstance> item, unsigned int uiAlpha)
{
	m_hSlotsInfo[iIndex].item = item;
	m_hSlotsInfo[iIndex].alpha = uiAlpha;
	m_hSlotsInfo[iIndex].show = true;
}

void UIScene_SpaceCraftingMenu::setCraftVSlotItem(int iPad, int iIndex, shared_ptr<ItemInstance> item, unsigned int uiAlpha)
{
	m_vSlotsInfo[iIndex].item = item;
	m_vSlotsInfo[iIndex].alpha = uiAlpha;
	m_vSlotsInfo[iIndex].show = true;
}

void UIScene_SpaceCraftingMenu::setCraftingOutputSlotItem(int iPad, shared_ptr<ItemInstance> item)
{
	m_craftingOutputSlotInfo.item = item;
	m_craftingOutputSlotInfo.alpha = 31;
	m_craftingOutputSlotInfo.show = item != NULL;
}

void UIScene_SpaceCraftingMenu::setCraftingOutputSlotRedBox(bool show)
{
	m_slotListCraftingOutput.showSlotRedBox(0,show);
}

void UIScene_SpaceCraftingMenu::setIngredientSlotItem(int iPad, int index, shared_ptr<ItemInstance> item)
{
	m_ingredientsSlotsInfo[index].item = item;
	m_ingredientsSlotsInfo[index].alpha = 31;
	m_ingredientsSlotsInfo[index].show = item != NULL;
}

void UIScene_SpaceCraftingMenu::setIngredientSlotRedBox(int index, bool show)
{
	m_slotListIngredientsLayout.showSlotRedBox(index,show);
}

void UIScene_SpaceCraftingMenu::setIngredientDescriptionItem(int iPad, int index, shared_ptr<ItemInstance> item)
{
	m_ingredientsInfo[index].item = item;
	m_ingredientsInfo[index].alpha = 31;
	m_ingredientsInfo[index].show = item != NULL;

	IggyDataValue result;
	IggyDataValue value[2];

	value[0].type = IGGY_DATATYPE_number;
	value[0].number = index;

	value[1].type = IGGY_DATATYPE_boolean;
	value[1].boolval = m_ingredientsInfo[index].show;
	IggyResult out = IggyPlayerCallMethodRS ( getMovie() , &result, IggyPlayerRootPath( getMovie() ) , m_funcShowIngredientSlot , 2 , value );
}

void UIScene_SpaceCraftingMenu::setIngredientDescriptionRedBox(int index, bool show)
{
	m_slotListIngredients[index].showSlotRedBox(0,show);
}

void UIScene_SpaceCraftingMenu::setIngredientDescriptionText(int index, LPCWSTR text)
{
	m_labelIngredientsDesc[index].setLabel(text);
}


void UIScene_SpaceCraftingMenu::setShowCraftHSlot(int iIndex, bool show)
{
	m_hSlotsInfo[iIndex].show = show;
}

void UIScene_SpaceCraftingMenu::showTabHighlight(int iIndex, bool show)
{
	if(show)
	{	
		IggyDataValue result;
		IggyDataValue value[1];

		value[0].type = IGGY_DATATYPE_number;
		value[0].number = iIndex;
		IggyResult out = IggyPlayerCallMethodRS ( getMovie() , &result, IggyPlayerRootPath( getMovie() ) , m_funcSetActiveTab , 1 , value );
	}
}

void UIScene_SpaceCraftingMenu::setGroupText(LPCWSTR text)
{
	m_labelGroupName.setLabel(text);
}

void UIScene_SpaceCraftingMenu::setDescriptionText(LPCWSTR text)
{
	m_labelDescription.setLabel(text);
}

void UIScene_SpaceCraftingMenu::setItemText(LPCWSTR text)
{
	m_labelItemName.setLabel(text);
}

void UIScene_SpaceCraftingMenu::UpdateMultiPanel()
{
	// Call Iggy function to show the current panel
	IggyDataValue result;
	IggyDataValue value[1];

	value[0].type = IGGY_DATATYPE_number;
	value[0].number = m_iDisplayDescription;

	IggyResult out = IggyPlayerCallMethodRS ( getMovie() , &result, IggyPlayerRootPath( getMovie() ) , m_funcShowPanelDisplay , 1 , value );
}

void UIScene_SpaceCraftingMenu::scrollDescriptionUp()
{
	// handled differently
}

void UIScene_SpaceCraftingMenu::scrollDescriptionDown()
{
	// handled differently
}

void UIScene_SpaceCraftingMenu::updateHighlightAndScrollPositions()
{
	{	
		IggyDataValue result;
		IggyDataValue value[2];

		value[0].type = IGGY_DATATYPE_number;
		value[0].number = m_iCurrentSlotHIndex;

		int selectorType = 0;
		if(CanBeMadeA[m_iCurrentSlotHIndex].iCount  == 2)
		{
			selectorType = 1;
		}
		else if( CanBeMadeA[m_iCurrentSlotHIndex].iCount  > 2)
		{
			selectorType = 2;
		}

		value[1].type = IGGY_DATATYPE_number;
		value[1].number = selectorType;

		IggyResult out = IggyPlayerCallMethodRS ( getMovie() , &result, IggyPlayerRootPath( getMovie() ) , m_funcMoveSelector , 2 , value );
	}

	{
		IggyDataValue result;
		IggyDataValue value[1];

		value[0].type = IGGY_DATATYPE_number;
		value[0].number = m_iCurrentSlotVIndex;

		IggyResult out = IggyPlayerCallMethodRS ( getMovie() , &result, IggyPlayerRootPath( getMovie() ) , m_funcSelectVerticalItem , 1 , value );
	}
}

void UIScene_SpaceCraftingMenu::HandleMessage(EUIMessage message, void *data)
{
	switch(message)
	{
	case eUIMessage_InventoryUpdated:
		handleInventoryUpdated(data);
		break;
	};
}

void UIScene_SpaceCraftingMenu::handleInventoryUpdated(LPVOID data)
{
	HandleInventoryUpdated();
}

void UIScene_SpaceCraftingMenu::updateVSlotPositions(int iSlots, int i)
{
	// Not needed
}
