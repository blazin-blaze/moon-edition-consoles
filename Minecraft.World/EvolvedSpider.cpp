#include "stdafx.h"
#include "SharedConstants.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.effect.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.h"
#include "..\Minecraft.Client\Textures.h"
#include "EvolvedSpider.h"
#include "Item.h"

EvolvedSpider::EvolvedSpider(Level* level) : Spider(level)
{
	// 4J Stu - This function call had to be moved here from the Entity ctor to ensure that the derived version of the function is called
	registerAttributes();
	this->setSize(1.4f*1.2f, 0.9f*1.2f);
}

void EvolvedSpider::registerAttributes()
{
	Spider::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(16*2);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.8f*2);
	getAttribute(SharedMonsterAttributes::ATTACK_DAMAGE)->setBaseValue(3 * 2);
}

bool EvolvedSpider::doHurtTarget(shared_ptr<Entity> target)
{
	if (Spider::doHurtTarget(target))
	{
		return true;
	}
	return false;
}

void EvolvedSpider::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	int loot = getDeathLoot();
	if (loot > 0)
	{
		int count = random->nextInt(3);
		if (playerBonusLevel > 0)
		{
			count += random->nextInt(playerBonusLevel + 1);
		}
		for (int i = 0; i < count; i++)
			spawnAtLocation(make_shared<ItemInstance>(loot, count, 1), 0);
	}

	bool shouldDropKey = random->nextInt(7) == 1 ? true : false;
	if (shouldDropKey) {
		spawnAtLocation(Item::moonDungeonKey_Id, 1);
	}

	if (wasKilledByPlayer && (random->nextInt(3) == 0 || random->nextInt(1 + playerBonusLevel) > 0))
	{
		spawnAtLocation(Item::spiderEye_Id, 1);
	}
}


MobGroupData* EvolvedSpider::finalizeMobSpawn(MobGroupData* groupData, int extraData /*= 0*/) // 4J Added extraData param
{
	#ifndef _CONTENT_PACKAGE
	// 4J-JEV: Added for spider-jockey spawn-egg.
	if ((level->random->nextInt(100) == 0) || (extraData != 0))
#else
	if (level->random->nextInt(100) == 0)
#endif
	{
		shared_ptr<EvolvedSkeleton> skeleton = std::make_shared<EvolvedSkeleton>(level);
		skeleton->moveTo(x, y, z, yRot, 0);
		skeleton->finalizeMobSpawn(nullptr);
		level->addEntity(skeleton);
		skeleton->ride(shared_from_this());
	}

	return groupData;
}

void EvolvedSpider::checkHurtTarget(shared_ptr<Entity> target, float d)
{
	//do not check for brightness levels as evolved spider

	if (d > 2 && d < 6 && random->nextInt(10) == 0)
	{
		if (onGround)
		{
			double xdd = target->x - x;
			double zdd = target->z - z;
			float dd = static_cast<float>(sqrt(xdd * xdd + zdd * zdd));
			xd = (xdd / dd * 0.5f) * 0.8f + xd * 0.2f;
			zd = (zdd / dd * 0.5f) * 0.8f + zd * 0.2f;
			yd = 0.4f;
		}
	}
	else
	{
		Monster::checkHurtTarget(target, d);
	}
}

shared_ptr<Entity> EvolvedSpider::findAttackTarget()
{
	#ifndef _FINAL_BUILD
	#ifdef _DEBUG_MENUS_ENABLED
	if (app.GetMobsDontAttackEnabled())
	{
		return shared_ptr<Player>();
	}
	#endif
	#endif

	//do not check for brightness levels as evolved spider
	double maxDist = 16;
	return level->getNearestAttackablePlayer(shared_from_this(), maxDist);
}