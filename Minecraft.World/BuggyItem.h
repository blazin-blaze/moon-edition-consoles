#pragma once

#include "Item.h"
#include "DefaultDispenseItemBehavior.h"

class Player;
class Level;

class BuggyItem : public Item
{
private:
	class BuggyDispenseBehavior : public DefaultDispenseItemBehavior
	{
	private:
		DefaultDispenseItemBehavior defaultDispenseItemBehavior;

	public:
		virtual shared_ptr<ItemInstance> execute(BlockSource* source, shared_ptr<ItemInstance> dispensed, eOUTCOME& outcome);

	protected:
		virtual void playSound(BlockSource* source);
	};

public:

	BuggyItem(int id);

	virtual bool TestUse(shared_ptr<ItemInstance> itemInstance, Level* level, shared_ptr<Player> player);
	virtual shared_ptr<ItemInstance> use(shared_ptr<ItemInstance> itemInstance, Level* level, shared_ptr<Player> player);
};