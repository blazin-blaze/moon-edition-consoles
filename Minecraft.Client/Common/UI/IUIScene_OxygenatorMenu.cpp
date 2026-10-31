#include "stdafx.h"

#include "IUIScene_OxygenatorMenu.h"
#include "..\..\..\Minecraft.World\net.minecraft.world.inventory.h"

IUIScene_AbstractContainerMenu::ESceneSection IUIScene_OxygenatorMenu::GetSectionAndSlotInDirection( ESceneSection eSection, ETapState eTapDirection, int *piTargetX, int *piTargetY )
{
	ESceneSection newSection = eSection;
	int xOffset = 0;

	// Find the new section if there is one
	switch( eSection )
	{
	case eSectionOxygenatorIngredient:
		if(eTapDirection == eTapStateUp)
		{
			newSection = eSectionOxygenatorUsing;
			xOffset = OXYGENATOR_SCENE_INGREDIENT_SLOT_DOWN_OFFSET;
		}
		else if(eTapDirection == eTapStateDown)
		{
			newSection = eSectionOxygenatorInventory;
		}
		break;
	case eSectionOxygenatorInventory:
		if(eTapDirection == eTapStateDown)
		{
			newSection = eSectionOxygenatorUsing;
		}
		else if(eTapDirection == eTapStateUp)
		{
			newSection = eSectionOxygenatorIngredient;
		}
		break;
	case eSectionOxygenatorUsing:
		if(eTapDirection == eTapStateUp)
		{
			newSection = eSectionOxygenatorInventory;
		}
		else if(eTapDirection == eTapStateDown)
		{				
			newSection = eSectionOxygenatorIngredient;
		}
		break;
	default:
		assert( false );
		break;
	}

	updateSlotPosition(eSection, newSection, eTapDirection, piTargetX, piTargetY, xOffset);

	return newSection;
}

int IUIScene_OxygenatorMenu::getSectionStartOffset(ESceneSection eSection)
{
	int offset = 0;
	switch( eSection )
	{
	case eSectionOxygenatorIngredient:
		offset = OxygenatorMenu::INGREDIENT_SLOT;
		break;
	case eSectionOxygenatorInventory:
		offset = OxygenatorMenu::INV_SLOT_START;
		break;
	case eSectionOxygenatorUsing:
		offset = OxygenatorMenu::INV_SLOT_START + 27;
		break;
	default:
		assert( false );
		break;
	}
	return offset;
}