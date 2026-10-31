#include "stdafx.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.npc.h"
#include "LookAtTradingPlayerGoal.h"

LookAtTradingPlayerGoal::LookAtTradingPlayerGoal(Villager *villager) : LookAtPlayerGoal((Mob *)villager, typeid(Player), 8)
{
	this->villager = villager;
	this->alienVillager = NULL;
}

LookAtTradingPlayerGoal::LookAtTradingPlayerGoal(AlienVillager* alienVillager) : LookAtPlayerGoal(static_cast<Mob*>(alienVillager), typeid(Player), 8)
{
	this->alienVillager = alienVillager;
	this->villager = NULL;
}

bool LookAtTradingPlayerGoal::canUse()
{
	if (villager != NULL && villager->isTrading()) {
		lookAt = weak_ptr<Entity>(dynamic_pointer_cast<Entity>(villager->getTradingPlayer()));
		return true;
	}
	else if (alienVillager != NULL && alienVillager->isTrading()) {
		lookAt = weak_ptr<Entity>(dynamic_pointer_cast<Entity>(alienVillager->getTradingPlayer()));
		return true;
	}
	return false;
}