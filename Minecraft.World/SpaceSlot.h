#pragma once
// 4J Stu Added
// In InventoryMenu.java they create an anoymous class while creating some slot. I have moved the content
// of that anonymous class to here

#include "Slot.h"

class Container;

class SpaceSlot : public Slot
{
private:
	const int slotNum;

public:
	SpaceSlot(int slotNum, shared_ptr<Container> container, int id, int x, int y);
	virtual ~SpaceSlot() {}

	virtual int getMaxStackSize() const;
	virtual bool mayPlace(shared_ptr<ItemInstance> item);
	Icon* getNoItemIcon();
	//virtual bool mayCombine(shared_ptr<ItemInstance> item); // 4J Added
	//virtual shared_ptr<ItemInstance> combine(shared_ptr<ItemInstance> item); // 4J Added
}; 