#pragma once

#include "Spider.h"

class EvolvedSpider : public Spider
{
public:
	eINSTANCEOF GetType() { return eTYPE_EVOLVEDSPIDER; }
	static Entity* create(Level* level) { return new EvolvedSpider(level); }

public:
	EvolvedSpider(Level* level);

protected:
	void registerAttributes();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

public:
	virtual bool doHurtTarget(shared_ptr<Entity> target);
	MobGroupData* finalizeMobSpawn(MobGroupData* groupData, int extraData = 0); // 4J Added extraData param
	virtual shared_ptr<Entity> findAttackTarget();
	virtual void checkHurtTarget(shared_ptr<Entity> target, float d);
};