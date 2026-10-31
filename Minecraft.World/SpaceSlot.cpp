#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.inventory.h"
#include "net.minecraft.world.item.crafting.h"
#include "SpaceSlot.h"

SpaceSlot::SpaceSlot(int slotNum, shared_ptr<Container> container, int id, int x, int y)
	: Slot(container, id, x, y),
	slotNum(slotNum)
{
}

int SpaceSlot::getMaxStackSize() const
{
	return 1;
}

bool SpaceSlot::mayPlace(shared_ptr<ItemInstance> item)
{
	if (item == nullptr)
	{
		return false;
	}
	//if (dynamic_cast<ArmorItem*>(item->getItem()) != nullptr)
	//{
	//	return dynamic_cast<ArmorItem*>(item->getItem())->slot == slotNum;
	//}
	if (item->getItem()->id == Item::frequencyModule_Id)
	{
		return slotNum == 0;
	}else if (item->getItem()->id == Item::oxygenMask_Id)
	{
		return slotNum == 1;
	}else if (item->getItem()->id == Item::oxygenGear_Id)
	{
		return slotNum == 2;
	}else if (item->getItem()->id == Item::oxygenTank_Id)
	{
		return slotNum == 3;
	}
	return false;
}

Icon* SpaceSlot::getNoItemIcon()
{
	return ArmorItem::getEmptyIcon(slotNum);
}

//
//bool ArmorSlot::mayCombine(shared_ptr<ItemInstance> item)
//{
//	shared_ptr<ItemInstance> thisItemI = getItem();
//	if(thisItemI == nullptr || item == nullptr) return false;
//
//	ArmorItem *thisItem = (ArmorItem *)thisItemI->getItem();
//	bool thisIsDyableArmor = thisItem->getMaterial() == ArmorItem::ArmorMaterial::CLOTH;
//	bool itemIsDye = item->id == Item::dye_powder_Id;
//	return thisIsDyableArmor && itemIsDye;
//}
//
//shared_ptr<ItemInstance> ArmorSlot::combine(shared_ptr<ItemInstance> item)
//{
//	shared_ptr<CraftingContainer> craftSlots = shared_ptr<CraftingContainer>( new CraftingContainer(nullptr, 2, 2) );
//	craftSlots->setItem(0, item);
//	craftSlots->setItem(1, getItem()); // Armour item needs to go second
//	shared_ptr<ItemInstance> result = ArmorDyeRecipe::assembleDyedArmor(craftSlots);
//	craftSlots->setItem(0, nullptr);
//	craftSlots->setItem(1, nullptr);
//	return result;
//}