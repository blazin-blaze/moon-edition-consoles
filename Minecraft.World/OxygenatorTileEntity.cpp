#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "OxygenatorTileEntity.h"
#include "SharedConstants.h"
#include "net.minecraft.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.item.alchemy.h"
#include "net.minecraft.world.phys.h"

OxygenatorTileEntity::OxygenatorTileEntity()
{
	oxygenTime = 0;
	items = ItemInstanceArray(1);
	name = L"";
}

OxygenatorTileEntity::~OxygenatorTileEntity()
{
	delete [] items.data;
}

wstring OxygenatorTileEntity::getName()
{
	return hasCustomName() ? name : app.GetString(IDS_TILE_OXYGENATOR);
}

wstring OxygenatorTileEntity::getCustomName()
{
	return hasCustomName() ? name : L"";
}

bool OxygenatorTileEntity::hasCustomName()
{
	return !name.empty();
}

void OxygenatorTileEntity::setCustomName(const wstring &name)
{
	this->name = name;
}

unsigned int OxygenatorTileEntity::getContainerSize()
{
	return items.length;
}

void OxygenatorTileEntity::tick()
{
	if (oxygenTime > 0)
	{
		oxygenTime--;

		if (oxygenTime == 0)
		{
			doOxygen();
			setChanged();
		}
		else if (!canOxygenate())
		{
			oxygenTime = 0;
			setChanged();
		}
		else if (ingredientId != items[INGREDIENT_SLOT]->id)
		{
			oxygenTime = 0;
			setChanged();
		}
	}
	else if (canOxygenate())
	{
		oxygenTime = SharedConstants::TICKS_PER_SECOND * PotionBrewing::BREWING_TIME_SECONDS;
		ingredientId = items[INGREDIENT_SLOT]->id;
	}

	TileEntity::tick();
}

int OxygenatorTileEntity::getOxygenTime()
{
	return oxygenTime;
}

bool OxygenatorTileEntity::canOxygenate()
{
	if (items[INGREDIENT_SLOT] == NULL || items[INGREDIENT_SLOT]->count <= 0)
	{
		return false;
	}
	if(items[INGREDIENT_SLOT]->id != Item::oxygenTank_Id) return false;
	if(items[INGREDIENT_SLOT]->getAuxValue() <= 0) return false;
	if(!hasLeavesNearby()) return false;
	return true;
}

bool OxygenatorTileEntity::hasLeavesNearby() {
	int leaves = 0;

	//Check to see if all blocks surrounding the oxygenator immediately are empty; if not, oxygenator doesn't have access to "open air"
	for(int xO = -1; xO <= 1; xO++) {
		for(int zO = -1; zO <= 1; zO++) {
			if(xO == 0 && zO == 0) continue;
			if(((!level->isEmptyTile(x + xO, y, z + zO))) || !level->isEmptyTile(x + xO, y + 1, z + zO)) {
				if(level->getTile(x + xO, y, z + zO) != Tile::leaves_Id && level->getTile(x + xO, y + 1, z + zO) != Tile::leaves_Id) return false;
			}
		}
	}

	//Needs to be completely empty above where the fan is
	if (!level->isEmptyTile(x, y + 1, z)) {
		if(level->getTile(x, y + 1, z) != Tile::leaves_Id) return false;
	}

	for(int xO = -5; xO <= 5; xO++) {
		for(int yO = 0; yO < 5; yO++) {
			for(int zO = -5; zO <= 5; zO++) {
				int dX = this->x + xO;
				int dY = this->y + yO;
				int dZ = this->z + zO;
			
				int id = level->getTile(dX, dY, dZ);
				if(id == Tile::leaves_Id) leaves++;
				if(leaves >= 10) return true;
			}
		}
	}
	return leaves >= 10;
}

void OxygenatorTileEntity::doOxygen()
{
	if (!canOxygenate())
	{
		return;
	}

	shared_ptr<ItemInstance> ingredient = items[INGREDIENT_SLOT];

	//decrease damage
	ingredient->setAuxValue(ingredient->getAuxValue() < 20 ? 0 : ingredient->getAuxValue() - 20);
}

void OxygenatorTileEntity::load(CompoundTag *base)
{
	TileEntity::load(base);

	ListTag<CompoundTag> *inventoryList = (ListTag<CompoundTag> *) base->getList(L"Items");
	delete [] items.data;
	items = ItemInstanceArray(getContainerSize());
	for (int i = 0; i < inventoryList->size(); i++)
	{
		CompoundTag *tag = inventoryList->get(i);
		int slot = tag->getByte(L"Slot");
		if (slot >= 0 && slot < items.length) items[slot] = ItemInstance::fromTag(tag);
	}

	oxygenTime = base->getShort(L"OxygenTime");
	if (base->contains(L"CustomName")) name = base->getString(L"CustomName");
}

void OxygenatorTileEntity::save(CompoundTag *base)
{
	TileEntity::save(base);

	base->putShort(L"OxygenTime", (short) (oxygenTime));
	ListTag<CompoundTag> *listTag = new ListTag<CompoundTag>();

	for (int i = 0; i < items.length; i++)
	{
		if (items[i] != NULL) 
		{
			CompoundTag *tag = new CompoundTag();
			tag->putByte(L"Slot", (byte) i);
			items[i]->save(tag);
			listTag->add(tag);
		}
	}
	base->put(L"Items", listTag);
	if (hasCustomName()) base->putString(L"CustomName", name);
}

shared_ptr<ItemInstance> OxygenatorTileEntity::getItem(unsigned int slot)
{
	if (slot >= 0 && slot < items.length)
	{
		return items[slot];
	}
	return nullptr;
}

shared_ptr<ItemInstance> OxygenatorTileEntity::removeItem(unsigned int slot, int count)
{
	// 4J Stu - Changed the implementation of this function to be the same as ChestTileEntity to enable the "Pickup Half"
	// option on the ingredients slot
	// Fix for #65373 - TU8: Content: UI: Command "Take Half" in the Brewing Stand interface doesn't work as intended.

	if (slot >= 0 && slot < items.length && items[slot] != NULL)
	{
		if (items[slot]->count <= count) 
		{
			shared_ptr<ItemInstance> item = items[slot];
			items[slot] = nullptr;
			this->setChanged();
			// 4J Stu - Fix for duplication glitch
			if(item->count <= 0) return nullptr;
			return item;
		} 
		else 
		{
			shared_ptr<ItemInstance> i = items[slot]->remove(count);
			if (items[slot]->count == 0) items[slot] = nullptr;
			this->setChanged();
			// 4J Stu - Fix for duplication glitch
			if(i->count <= 0) return nullptr;
			return i;
		}
	}
	return nullptr;
}

shared_ptr<ItemInstance> OxygenatorTileEntity::removeItemNoUpdate(int slot)
{
	if (slot >= 0 && slot < items.length)
	{
		shared_ptr<ItemInstance> item = items[slot];
		items[slot] = nullptr;
		return item;
	}
	return nullptr;
}

void OxygenatorTileEntity::setItem(unsigned int slot, shared_ptr<ItemInstance> item)
{
	if (slot >= 0 && slot < items.length)
	{
		items[slot] = item;
	}
}

int OxygenatorTileEntity::getMaxStackSize()
{
	// this value is not used for the potion slots
	return 64;
}

bool OxygenatorTileEntity::stillValid(shared_ptr<Player> player)
{
	if (level->getTileEntity(x, y, z) != shared_from_this()) return false;
	if (player->distanceToSqr(x + 0.5, y + 0.5, z + 0.5) > 8 * 8) return false;
	return true;
}

void OxygenatorTileEntity::startOpen()
{
}

void OxygenatorTileEntity::stopOpen()
{
}

bool OxygenatorTileEntity::canPlaceItem(int slot, shared_ptr<ItemInstance> item)
{
	return item->id == Item::oxygenTank_Id;
}

void OxygenatorTileEntity::setOxygenTime(int value)
{
	oxygenTime = value;
}

intArray OxygenatorTileEntity::getSlotsForFace(int face)
{
	return OxygenatorTileEntity::INGREDIENT_SLOT;
}

bool OxygenatorTileEntity::canPlaceItemThroughFace(int slot, shared_ptr<ItemInstance> item, int face)
{
	return canPlaceItem(slot, item);
}

bool OxygenatorTileEntity::canTakeItemThroughFace(int slot, shared_ptr<ItemInstance> item, int face)
{
	return true;
}

// 4J Added
shared_ptr<TileEntity> OxygenatorTileEntity::clone()
{
	shared_ptr<OxygenatorTileEntity> result = shared_ptr<OxygenatorTileEntity>( new OxygenatorTileEntity() );
	TileEntity::clone(result);

	result->oxygenTime = oxygenTime;
	result->ingredientId = ingredientId;

	for (unsigned int i = 0; i < items.length; i++)
	{
		if (items.data[i] != NULL)
		{
			result->items.data[i] = ItemInstance::clone(items.data[i]);
		}
	}
	return result;
}