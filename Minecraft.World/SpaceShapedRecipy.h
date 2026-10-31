#pragma once

class SpaceShapedRecipy : public SpaceRecipy
{
private:
	int width, height, group;
	ItemInstance** recipeItems;
	ItemInstance* result;
	bool _keepTag;
public:
	const int resultId;

public:
	SpaceShapedRecipy(int width, int height, ItemInstance** recipeItems, ItemInstance* result, int iGroup = SpaceRecipy::eGroupType_Space_Misc);

	virtual const ItemInstance* getResultItem();
	virtual const int getGroup();
	virtual bool matches(shared_ptr<CraftingContainer> craftSlots, Level* level);

private:
	bool matches(shared_ptr<CraftingContainer> craftSlots, int xOffs, int yOffs, bool xFlip);

public:
	virtual shared_ptr<ItemInstance> assemble(shared_ptr<CraftingContainer> craftSlots);
	virtual int size();
	SpaceShapedRecipy* keepTag();

	// 4J-PB - to return the items required to make a recipe
	virtual bool requires(int iRecipe);
	virtual void requires(INGREDIENTS_REQUIRED* pIngReq);
};
