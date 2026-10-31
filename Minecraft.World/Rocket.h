#pragma once
#include "Entity.h"

class Rocket : public Entity
{
public:
	eINSTANCEOF GetType() { return eTYPE_ROCKET; };
	static Entity* create(Level* level) { return new Rocket(level); }

private:
	// 4J - added for common ctor code
	void _init();
public:
	static const int serialVersionUID = 0;

public:
	Rocket(Level* level);

protected:
	virtual bool makeStepSound();
	virtual void defineSynchedData();

public:
	virtual AABB* getCollideAgainstBox(shared_ptr<Entity> entity);
	virtual AABB* getCollideBox();
	virtual bool isPushable();

	Rocket(Level* level, double x, double y, double z);

	virtual double getRideHeight();
	virtual bool hurt(DamageSource* source, float damage);
	virtual bool isPickable();
	virtual bool getLaunched();
	virtual void setLaunched(bool launched);
	virtual bool getFueled();
	virtual void setFueled(bool isFueled);
	virtual int getPrelaunchTime();
private:
	int lSteps;
	double lx, ly, lz, lyr, lxr;
	double lxd, lyd, lzd;
	static const int DATA_FUELED = 14;
	static const int DATA_PRE_LAUNCH = 15;
	static const int DATA_HAS_LAUNCHED = 16;
	static const int DATA_ID_DAMAGE = 17;
	static const int DATA_ROCKET_ID = 18;
	static const int DATA_COOLDOWN = 19;
	static const int DATA_CAKE = 20;
	int launchingCooldown;
	int ticksUntilNextSound;
	bool hasPlayedLaunchSound;
	bool doLerp;
	wstring customName;
	virtual void setPrelaunchTime(int preLaunch);
	bool shouldFall(int tile);
	bool isLaunchBlocked();

public:
	virtual void tick();
	virtual void positionRider();

protected:
	virtual void addAdditonalSaveData(CompoundTag* base);
	virtual void readAdditionalSaveData(CompoundTag* base);

public:
	virtual void setDamage(float damage);
	virtual float getDamage();
	void setRocketId(int rocketId);
	int getRocketId();
	void setCooldown(int cooldown);
	int getCooldown();
	void setCake(bool cake);
	bool getCake();
	virtual float getShadowHeightOffs();
	virtual wstring getName();
	virtual wstring getAName();
	wstring getCustomName();
	void setCustomName(wstring name);
	bool hasCustomName();
	virtual bool interact(shared_ptr<Player> player);
	virtual void lerpTo(double x, double y, double z, float yRot, float xRot, int steps);
	virtual void lerpMotion(double xd, double yd, double zd);

	bool getDoLerp();
	void setDoLerp(bool doLerp);

public:
	virtual void remove();
};
