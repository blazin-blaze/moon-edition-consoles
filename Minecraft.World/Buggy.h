#pragma once
#include "Entity.h"
#include "Container.h"

class AnimalChest;

class Buggy : public Entity, public Container
{
public:
	eINSTANCEOF GetType() { return eTYPE_BUGGY; };
	static Entity* create(Level* level) { return new Buggy(level); }
	ItemInstanceArray items;

private:
	// 4J - added for common ctor code
	void _init();
public:
	static const int serialVersionUID = 0;

public:
	Buggy(Level* level);

	virtual shared_ptr<ItemInstance> getItem(unsigned int slot);
	virtual shared_ptr<ItemInstance> removeItem(unsigned int slot, int count);
	virtual shared_ptr<ItemInstance> removeItemNoUpdate(int slot);
	virtual void setItem(unsigned int slot, shared_ptr<ItemInstance> item);
	virtual void setChanged();
	virtual bool stillValid(shared_ptr<Player> player);
	virtual void startOpen();
	virtual void stopOpen();
	virtual bool canPlaceItem(int slot, shared_ptr<ItemInstance> item);
	virtual wstring getAName();
	virtual wstring getName();
	virtual int getMaxStackSize();
	virtual bool hasCustomName();
	virtual wstring getCustomName();
	void setCustomName(wstring name);
	virtual unsigned int getContainerSize() { return 9*3; }
	virtual int getContainerType();
	void dropInventory();

protected:
	virtual bool makeStepSound();
	virtual void defineSynchedData();

public:
	virtual AABB* getCollideAgainstBox(shared_ptr<Entity> entity);
	virtual AABB* getCollideBox();
	virtual bool isPushable();

	Buggy(Level* level, double x, double y, double z);

	virtual double getRideHeight();
	virtual bool hurt(DamageSource* source, float damage);
	virtual bool isPickable();
private:
	int lSteps;
	double lx, ly, lz, lyr, lxr;
	double lxd, lyd, lzd;
	static const int DATA_ID_DAMAGE = 17;
	static const int DATA_ID_HAS_STORAGE = 20;
	double wheelRotation;
	int noCollisionTicks;
	wstring customName;
	static const double MAX_SPEED;
	static const double ACCELERATION;
	bool shouldFall(int tile);
public:
	virtual void tick();
	virtual void positionRider();

protected:
	virtual void addAdditonalSaveData(CompoundTag* base);
	virtual void readAdditionalSaveData(CompoundTag* base);

public:
	virtual void setDamage(float damage);
	virtual float getDamage();
	virtual float getShadowHeightOffs();
	virtual bool interact(shared_ptr<Player> player);
	virtual void lerpTo(double x, double y, double z, float yRot, float xRot, int steps);
	virtual void lerpMotion(double xd, double yd, double zd);

	double getWheelRot();
	bool getHasStorage();
	void setHasStorage(bool hasStorage);
};