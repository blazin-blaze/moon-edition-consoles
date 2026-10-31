#pragma once
#include "IUIScene_AbstractContainerMenu.h"

// The 0-indexed slot in the inventory list that lines up with the result slot
#define OXYGENATOR_SCENE_INGREDIENT_SLOT_UP_OFFSET 5
#define OXYGENATOR_SCENE_INGREDIENT_SLOT_DOWN_OFFSET 5

class IUIScene_OxygenatorMenu : public virtual IUIScene_AbstractContainerMenu
{
protected:
	virtual ESceneSection GetSectionAndSlotInDirection( ESceneSection eSection, ETapState eTapDirection, int *piTargetX, int *piTargetY );
	int getSectionStartOffset(ESceneSection eSection);
};