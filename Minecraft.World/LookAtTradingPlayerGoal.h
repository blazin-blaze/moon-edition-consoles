#pragma once

#include "LookAtPlayerGoal.h"

class Villager;
class AlienVillager;

class LookAtTradingPlayerGoal : public LookAtPlayerGoal
{
private:
	Villager *villager; // This is the owner of this goal
	AlienVillager *alienVillager; // This is the owner of this goal

public:
	LookAtTradingPlayerGoal(Villager *villager);
	LookAtTradingPlayerGoal(AlienVillager* alienVillager);

	virtual bool canUse();
};