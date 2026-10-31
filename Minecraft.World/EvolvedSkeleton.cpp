#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.item.enchantment.h"
#include "net.minecraft.world.effect.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.goal.target.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.projectile.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.damagesource.h"
#include "SharedConstants.h"
#include "EvolvedSkeleton.h"
#include "..\Minecraft.Client\Textures.h"
#include "SoundTypes.h"

EvolvedSkeleton::EvolvedSkeleton(Level* level) : Skeleton(level)
{
	// 4J Stu - This function call had to be moved here from the Entity ctor to ensure that
	// the derived version of the function is called
	registerAttributes();
	setHealth(getMaxHealth());
	bowGoal = new RangedAttackGoal(this, this, 1.0, SharedConstants::TICKS_PER_SECOND * 1, SharedConstants::TICKS_PER_SECOND * 3, 15);
	this->setSize(0.6f * 1.2f, 1.8f * 1.3f);
}

EvolvedSkeleton::~EvolvedSkeleton()
{
	delete bowGoal;
}

void EvolvedSkeleton::registerAttributes()
{
	Monster::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(16 * 2);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.25f*2);
	getAttribute(SharedMonsterAttributes::ATTACK_DAMAGE)->setBaseValue(3 * 2);
}

bool EvolvedSkeleton::useNewAi()
{
	return true;
}

bool EvolvedSkeleton::doHurtTarget(shared_ptr<Entity> target)
{
	if (Monster::doHurtTarget(target))
	{
		return true;
	}
	return false;
}

MobType EvolvedSkeleton::getMobType()
{
	return UNDEAD;
}

void EvolvedSkeleton::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	Skeleton::dropDeathLoot(wasKilledByPlayer, playerBonusLevel);
	bool shouldDropKey = random->nextInt(7) == 1 ? true : false;
	if (shouldDropKey) {
		spawnAtLocation(Item::moonDungeonKey_Id, 1);
	}
}

MobGroupData* EvolvedSkeleton::finalizeMobSpawn(MobGroupData* groupData, int extraData /*= 0*/) // 4J Added extraData param
{
	groupData = Monster::finalizeMobSpawn(groupData);
	goalSelector.addGoal(4, bowGoal, false);
	setSkeletonType(0);

	populateDefaultEquipmentSlots();
	populateDefaultEquipmentEnchantments();
	return groupData;
}