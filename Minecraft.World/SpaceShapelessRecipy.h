#pragma once

class SpaceShapelessRecipy : public SpaceRecipy
{
private:
	_eGroupType group;
	const ItemInstance* result;
	vector<ItemInstance*>* ingredients;

public:
	SpaceShapelessRecipy(ItemInstance* result, vector<ItemInstance*>* ingredients, _eGroupType egroup = SpaceRecipy::eGroupType_Space_Misc);

	virtual const ItemInstance* getResultItem();
	virtual const int getGroup();
	virtual bool matches(shared_ptr<CraftingContainer> craftSlots, Level* level);
	virtual shared_ptr<ItemInstance> assemble(shared_ptr<CraftingContainer> craftSlots);
	virtual int size();

	// 4J-PB - to return the items required to make a recipe
	virtual bool requires(int iRecipe);
	virtual void requires(INGREDIENTS_REQUIRED* pIngReq);

};