#include "stdafx.h"
#include "net.minecraft.world.entity.player.h"
#include "ItemInstance.h"
#include "CannedFoodItem.h"

CannedFoodItem::CannedFoodItem(int id, int nutrition, float saturationMod, bool isMeat) : FoodItem(id, nutrition, saturationMod, isMeat)
{
}

shared_ptr<ItemInstance> CannedFoodItem::useTimeDepleted(shared_ptr<ItemInstance> instance, Level* level, shared_ptr<Player> player)
{
	FoodItem::useTimeDepleted(instance, level, player);

	shared_ptr<ItemInstance> tinCanister = std::make_shared<ItemInstance>(Item::tinCanister);

	if (player->inventory->add(tinCanister) == false)
	{
		player->drop(tinCanister);
	}

	return instance;
}