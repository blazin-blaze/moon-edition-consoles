#pragma once

#include "FoodItem.h"

class Player;
class Level;

class CannedFoodItem : public FoodItem
{
public:
	CannedFoodItem(int id, int nutrition, float saturationMod, bool isMeat);

	shared_ptr<ItemInstance> useTimeDepleted(shared_ptr<ItemInstance> instance, Level* level, shared_ptr<Player> player);
}; 