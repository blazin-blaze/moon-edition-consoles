#include "stdafx.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.item.alchemy.h"
#include "net.minecraft.stats.h"
#include "OxygenatorMenu.h"

OxygenatorMenu::OxygenatorMenu(shared_ptr<Inventory> inventory, shared_ptr<OxygenatorTileEntity> oxygenator)
{
	tc = 0;

	this->oxygenator = oxygenator;

	ingredientSlot = addSlot(new OxygenSlot(oxygenator, 0, 79, 17));

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			addSlot(new Slot(inventory, x + y * 9 + 9, 8 + x * 18, 84 + y * 18));
		}
	}
	for (int x = 0; x < 9; x++)
	{
		addSlot(new Slot(inventory, x, 8 + x * 18, 142));
	}
}

void OxygenatorMenu::addSlotListener(ContainerListener *listener)
{
	AbstractContainerMenu::addSlotListener(listener);
	listener->setContainerData(this, 0, oxygenator->getOxygenTime());
}

void OxygenatorMenu::broadcastChanges()
{
	AbstractContainerMenu::broadcastChanges();

	//for (int i = 0; i < containerListeners->size(); i++)
	for(AUTO_VAR(it, containerListeners.begin()); it != containerListeners.end(); ++it)
	{
		ContainerListener *listener = *it; //containerListeners.at(i);
		if (tc != oxygenator->getOxygenTime())
		{
			listener->setContainerData(this, 0, oxygenator->getOxygenTime());
		}
	}
	tc = oxygenator->getOxygenTime();
}

void OxygenatorMenu::setData(int id, int value)
{
	if (id == 0) oxygenator->setOxygenTime(value);
}

bool OxygenatorMenu::stillValid(shared_ptr<Player> player)
{
	return oxygenator->stillValid(player);
}

shared_ptr<ItemInstance> OxygenatorMenu::quickMoveStack(shared_ptr<Player> player, int slotIndex)
{
	shared_ptr<ItemInstance> clicked = nullptr;
	Slot *slot = slots.at(slotIndex);
	Slot *IngredientSlot = slots.at(INGREDIENT_SLOT);

	if (slot != NULL && slot->hasItem())
	{
		shared_ptr<ItemInstance> stack = slot->getItem();
		clicked = stack->copy();

		if (!IngredientSlot->hasItem() && IngredientSlot->mayPlace(stack))
		{
			if (!moveItemStackTo(stack, INGREDIENT_SLOT, INGREDIENT_SLOT + 1, false))
			{
				return nullptr;
			}
		}
		else if (OxygenSlot::mayPlaceItem(clicked))
		{
			if (!moveItemStackTo(stack, INGREDIENT_SLOT, INGREDIENT_SLOT + 1, false))
			{
				return nullptr;
			}
		}
		else if (slotIndex >= INV_SLOT_START && slotIndex < INV_SLOT_END)
		{
			// 4J-PB - if the item is an ingredient, quickmove it into the ingredient slot
			if( (stack->id==Item::oxygenTank_Id) && 
				(!IngredientSlot->hasItem() || (stack->id==IngredientSlot->getItem()->id) ) )
			{
				if(!moveItemStackTo(stack, INGREDIENT_SLOT, INGREDIENT_SLOT+1, false))
				{
					return nullptr;
				}
			}
			else if (!moveItemStackTo(stack, USE_ROW_SLOT_START, USE_ROW_SLOT_END, false))
			{
				return nullptr;
			}
		}
		else if (slotIndex >= USE_ROW_SLOT_START && slotIndex < USE_ROW_SLOT_END)
		{
			// 4J-PB - if the item is an ingredient, quickmove it into the ingredient slot
			if((stack->id==Item::oxygenTank_Id) && 
				(!IngredientSlot->hasItem() || (stack->id==IngredientSlot->getItem()->id) ))
			{
				if(!moveItemStackTo(stack, INGREDIENT_SLOT, INGREDIENT_SLOT+1, false))
				{
					return nullptr;
				}
			}	
			else if (!moveItemStackTo(stack, INV_SLOT_START, INV_SLOT_END, false))
			{
				return nullptr;
			}
		}
		else
		{
			if (!moveItemStackTo(stack, INV_SLOT_START, USE_ROW_SLOT_END, false))
			{
				return nullptr;
			}
		}
		if (stack->count == 0)
		{
			slot->set(nullptr);
		}
		else
		{
			slot->setChanged();
		}
		if (stack->count == clicked->count)
		{
			return nullptr;
		}
		else
		{
			slot->onTake(player, stack);
		}
	}
	return clicked;
}

OxygenatorMenu::OxygenSlot::OxygenSlot(shared_ptr<OxygenatorTileEntity> oxygenator, int slot, int x, int y) : Slot(oxygenator, slot, x, y)
{
	this->oxygenator = oxygenator;
}

bool OxygenatorMenu::OxygenSlot::mayPlace(shared_ptr<ItemInstance> item)
{
	return mayPlaceItem(item);
}

int OxygenatorMenu::OxygenSlot::getMaxStackSize()
{
	return 1;
}

void OxygenatorMenu::OxygenSlot::onTake(shared_ptr<Player> player, shared_ptr<ItemInstance> carried)
{
	//if (carried->id == Item::oxygenTank_Id && carried->getAuxValue() > 0) this->player->awardStat(GenericStats::potion(),GenericStats::param_potion());
	Slot::onTake(player, carried);
}

bool OxygenatorMenu::OxygenSlot::mayCombine(shared_ptr<ItemInstance> second)
{
	return false;
}

bool OxygenatorMenu::OxygenSlot::mayPlaceItem(shared_ptr<ItemInstance> item)
{
	return item != NULL && (item->id == Item::oxygenTank_Id);
}
