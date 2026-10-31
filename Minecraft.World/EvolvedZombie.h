#pragma once
using namespace std;

#include "Zombie.h"

class DamageSource;

// SKIN BY XaPhobia Chris Beidler  
class EvolvedZombie : public Zombie
{
public:
	eINSTANCEOF GetType() { return eTYPE_EVOLVEDZOMBIE; }
	static Entity* create(Level* level) { return new EvolvedZombie(level); }

private:
	void _init();

public:
	EvolvedZombie(Level* level);

protected:
	virtual void registerAttributes();
	virtual bool useNewAi();

public:
	virtual bool canSpawn();

public:
	virtual bool mobInteract(shared_ptr<Player> player);
	virtual void killed(shared_ptr<LivingEntity> mob);

protected:
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

public:
	virtual MobGroupData* finalizeMobSpawn(MobGroupData* groupData, int extraData = 0); // 4J Added extraData param
};