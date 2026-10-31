#pragma once
#include "CraftingContainer.h"

#define RECIPE_TYPE_3x3 1

class SpaceRecipy
{
public:
	enum _eGroupType
	{
		eGroupType_First = 0,
		eGroupType_Rocket = 0,
		eGroupType_Compressed,
		eGroupType_Accessory,
		eGroupType_Space_Misc,
		eGroupType_Max
	}
	eGroupType; // to class the item produced by the recipe

	// 4J-PB - we'll classing an ingredient ID with a different aux value as a different IngID AuxVal pair
	typedef struct
	{
		int iIngC;
		int iType; // Can be a 2x2 or a 3x3. Inventory crafting can only make a 2x2.
		int* iIngIDA;
		int* iIngValA;
		int* iIngAuxValA;
		SpaceRecipy* pRecipy;
		bool bCanMake[XUSER_MAX_COUNT];
		unsigned int* uiGridA; // hold the layout of the recipe (id | auxval<<24)
		unsigned short usBitmaskMissingGridIngredients[XUSER_MAX_COUNT]; // each bit set means we don't have that grid ingredient
	}
	INGREDIENTS_REQUIRED;
	~SpaceRecipy() {}
	virtual bool matches(shared_ptr<CraftingContainer> craftSlots, Level* level) = 0;
	virtual shared_ptr<ItemInstance> assemble(shared_ptr<CraftingContainer> craftSlots) = 0;
	virtual int size() = 0;
	virtual const ItemInstance* getResultItem() = 0;
	virtual const int getGroup() = 0;

	// 4J-PB
	virtual bool requires(int iRecipe) = 0;
	virtual void requires(INGREDIENTS_REQUIRED* pIngReq) = 0;
};