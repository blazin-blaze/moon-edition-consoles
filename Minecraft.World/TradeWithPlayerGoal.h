#pragma once

#include "Goal.h"

class Villager;
class AlienVillager;

class TradeWithPlayerGoal : public Goal
{
private:
	Villager *mob; // This is the owner of the goal
	AlienVillager* alienVillager; // This is the owner of the goal

public:
	TradeWithPlayerGoal(Villager *mob);
	TradeWithPlayerGoal(AlienVillager* alienVillager);

	bool canUse();
	void start();
	void stop();
};