#pragma once
#include "Item.h"
#include "DefaultDispenseItemBehavior.h"

class Player;
class Level;

class RocketItem : public Item
{
private:
	class RocketDispenseBehavior : public DefaultDispenseItemBehavior
	{
	private:
		DefaultDispenseItemBehavior defaultDispenseItemBehavior;

	public:
		virtual shared_ptr<ItemInstance> execute(BlockSource* source, shared_ptr<ItemInstance> dispensed, eOUTCOME& outcome);

	protected:
		virtual void playSound(BlockSource* source);
	};
public:

	RocketItem(int id);
	RocketItem(int id, bool isCake);

	bool isCake;

	virtual bool TestUse(shared_ptr<ItemInstance> itemInstance, Level* level, shared_ptr<Player> player);
	virtual shared_ptr<ItemInstance> use(shared_ptr<ItemInstance> itemInstance, Level* level, shared_ptr<Player> player);
};