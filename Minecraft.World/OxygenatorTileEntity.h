#pragma once
#include "TileEntity.h"
#include "WorldlyContainer.h"

class OxygenatorTileEntity : public TileEntity, public WorldlyContainer
{
public:
	eINSTANCEOF GetType() { return eTYPE_OXYGENATORTILEENTITY; }
	static TileEntity *create() { return new OxygenatorTileEntity(); }

	static const int INGREDIENT_SLOT = 0;

private:
	ItemInstanceArray items;

	int oxygenTime;
	int ingredientId;
	wstring name;

public:
	OxygenatorTileEntity();
	~OxygenatorTileEntity();
	virtual wstring getName();
	virtual wstring getCustomName();
	virtual bool hasCustomName();
	virtual void setCustomName(const wstring &name);
	virtual unsigned int getContainerSize();
	virtual void tick();

	int getOxygenTime();

private:
	void doOxygen();
   
public:
	bool canOxygenate();
	virtual void load(CompoundTag *base);
	virtual void save(CompoundTag *base);
	virtual shared_ptr<ItemInstance> getItem(unsigned int slot);
	virtual shared_ptr<ItemInstance> removeItem(unsigned int slot, int i);
	virtual shared_ptr<ItemInstance> removeItemNoUpdate(int slot);
	virtual void setItem(unsigned int slot, shared_ptr<ItemInstance> item);
	virtual int getMaxStackSize();
	virtual bool stillValid(shared_ptr<Player> player);
	virtual void startOpen();
	virtual void stopOpen();
	virtual bool canPlaceItem(int slot, shared_ptr<ItemInstance> item);
	virtual void setOxygenTime(int value);
	virtual void setChanged() { TileEntity::setChanged(); } // 4J added
	virtual intArray getSlotsForFace(int face);
	virtual bool canPlaceItemThroughFace(int slot, shared_ptr<ItemInstance> item, int face);
	virtual bool canTakeItemThroughFace(int slot, shared_ptr<ItemInstance> item, int face);
	virtual bool hasLeavesNearby();

	// 4J Added
	virtual shared_ptr<TileEntity> clone();
};