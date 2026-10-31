#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.item.enchantment.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.damagesource.h"
#include "EvolvedZombie.h"
#include "..\Minecraft.Client\Textures.h"
#include "SoundTypes.h"

void EvolvedZombie::_init()
{
	registerAttributes();
}

EvolvedZombie::EvolvedZombie(Level* level) : Zombie(level)
{
	_init();
	this->setSize(0.6f*1.2f, 1.8f*1.3f);
}

void EvolvedZombie::registerAttributes()
{
	Zombie::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(16 * 2);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.23f*2);
	getAttribute(SharedMonsterAttributes::ATTACK_DAMAGE)->setBaseValue(3*2);
}

bool EvolvedZombie::useNewAi()
{
	return true;
}

bool EvolvedZombie::canSpawn()
{
	return level->difficulty > Difficulty::PEACEFUL && level->isUnobstructed(bb) && level->getCubes(shared_from_this(), bb)->empty() && !level->containsAnyLiquid(bb);
}

bool EvolvedZombie::mobInteract(shared_ptr<Player> player)
{
	return false;
}

void EvolvedZombie::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	Zombie::dropDeathLoot(wasKilledByPlayer, playerBonusLevel);
	bool shouldDropKey = random->nextInt(7) == 1 ? true : false;
	if (shouldDropKey) {
		spawnAtLocation(Item::moonDungeonKey_Id, 1);
	}
}

int EvolvedZombie::getDeathLoot()
{
	return Item::rotten_flesh_Id;
}

void EvolvedZombie::killed(shared_ptr<LivingEntity> mob)
{
	Monster::killed(mob);
}

MobGroupData* EvolvedZombie::finalizeMobSpawn(MobGroupData* groupData, int extraData /*= 0*/) // 4J Added extraData param
{
	setVillager(false);
	setBaby(false);
	return groupData;
}