#include "stdafx.h"
#include "net.minecraft.world.entity.npc.h"
#include "net.minecraft.world.entity.ai.control.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.inventory.h"
#include "TradeWithPlayerGoal.h"

TradeWithPlayerGoal::TradeWithPlayerGoal(Villager *mob)
{
	this->mob = mob;
	this->alienVillager = NULL;
	setRequiredControlFlags(Control::JumpControlFlag | Control::MoveControlFlag);
}

TradeWithPlayerGoal::TradeWithPlayerGoal(AlienVillager* alienVillager)
{
	this->alienVillager = alienVillager;
	this->mob = NULL;
	setRequiredControlFlags(Control::JumpControlFlag | Control::MoveControlFlag);
}

bool TradeWithPlayerGoal::canUse()
{
	if (mob != NULL) {
		if (!mob->isAlive()) return false;
		if (mob->isInWater()) return false;
		if (!mob->onGround) return false;
		if (mob->hurtMarked) return false;

		shared_ptr<Player> trader = mob->getTradingPlayer();
		if (trader == NULL)
		{
			// no interaction
			return false;
		}

		if (mob->distanceToSqr(trader) > (4 * 4))
		{
			// too far away
			return false;
		}

		if (!(trader->containerMenu == trader->inventoryMenu))
		{
			// closed container
			return false;
		}
	}else if(alienVillager != NULL) {
		if (!alienVillager->isAlive()) return false;
		if (alienVillager->isInWater()) return false;
		if (!alienVillager->onGround) return false;
		if (alienVillager->hurtMarked) return false;

		shared_ptr<Player> trader = alienVillager->getTradingPlayer();
		if (trader == NULL)
		{
			// no interaction
			return false;
		}

		if (alienVillager->distanceToSqr(trader) > (4 * 4))
		{
			// too far away
			return false;
		}

		if (!(trader->containerMenu == trader->inventoryMenu))
		{
			// closed container
			return false;
		}
	}

	return true;
}

void TradeWithPlayerGoal::start()
{
	if (mob != NULL) {
		mob->setTradingPlayer(NULL);
	}
	else if (alienVillager != NULL) {
		alienVillager->setTradingPlayer(NULL);
	}
}

void TradeWithPlayerGoal::stop()
{
	if (mob != NULL) {
		mob->setTradingPlayer(NULL);
	}
	else if (alienVillager != NULL) {
		alienVillager->setTradingPlayer(NULL);
	}
}