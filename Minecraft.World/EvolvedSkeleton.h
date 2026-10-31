#pragma once

#include "Skeleton.h"

class EvolvedSkeleton : public Skeleton
{
public:
	eINSTANCEOF GetType() { return eTYPE_EVOLVEDSKELETON; }
	static Entity* create(Level* level) { return new EvolvedSkeleton(level); }

private:
	RangedAttackGoal* bowGoal;

public:
	EvolvedSkeleton(Level* level);
	virtual ~EvolvedSkeleton();

protected:
	void registerAttributes();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

public:
	virtual bool useNewAi();
	virtual MobType getMobType();
	virtual bool doHurtTarget(shared_ptr<Entity> target);
	MobGroupData* finalizeMobSpawn(MobGroupData* groupData, int extraData = 0); // 4J Added extraData param
};