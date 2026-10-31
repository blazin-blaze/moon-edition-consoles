#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.chunk.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.damagesource.h"
#include "Rocket.h"
#include "Dimension.h"
#include <cstdint>

static uint64_t g_nextRocketId = 1;

// 4J - added for common ctor code
void Rocket::_init()
{
	doLerp = true;

	this->defineSynchedData();
	launchingCooldown = 10;

	lSteps = 0;
	lx = ly = lz = lyr = lxr = 0.0;
	lxd = lyd = lzd = 0.0;

	blocksBuilding = true;

	ticksUntilNextSound = (20 * 3) - 1;
	hasPlayedLaunchSound = false;

	this->setRocketId(g_nextRocketId++);
	customName = L"";
	this->setSize(0.5f*1.5f, 1.8f*1.5f);
}

Rocket::Rocket(Level* level) : Entity(level)
{
	_init();
}

bool Rocket::makeStepSound()
{
	return false;
}

void Rocket::defineSynchedData()
{
	entityData->define(DATA_FUELED, false);
	entityData->define(DATA_HAS_LAUNCHED, false);
	entityData->define(DATA_PRE_LAUNCH, 10*20);
	entityData->define(DATA_ID_DAMAGE, false);
	entityData->define(DATA_ROCKET_ID, 1);
	entityData->define(DATA_COOLDOWN, 0);
	entityData->define(DATA_CAKE, false);
}


AABB* Rocket::getCollideAgainstBox(shared_ptr<Entity> entity)
{
	return entity->bb;
}

AABB* Rocket::getCollideBox()
{
	return bb;
}

bool Rocket::isPushable()
{
	return false;
}

Rocket::Rocket(Level* level, double x, double y, double z) : Entity(level)
{
	_init();
	setPos(x, y, z);

	xd = 0;
	yd = 0;
	zd = 0;

	xo = x;
	yo = y;
	zo = z;
}

double Rocket::getRideHeight()
{
	return heightOffset + 1.25F;
}

bool Rocket::isPickable()
{
	return !removed;
}

void Rocket::tick()
{
	Entity::tick();
	if (getDamage() > 0) setDamage(getDamage() - 1);
	xo = x;
	yo = y;
	zo = z;

	if (rider.lock() == NULL && getLaunched())
	{
		if (getPrelaunchTime() <= 0) {
			remove();
		}
		else {
			setLaunched(false);
			ticksUntilNextSound = (20 * 3) - 1;
			setPrelaunchTime(20 * 10);
			setCooldown(3 * 20);
		}
	}

	if (!level->isClientSide && getLaunched() && getPrelaunchTime() <= 0) {
		int minX = (int)floor(bb->x0);
		int maxX = (int)floor(bb->x1);
		int minZ = (int)floor(bb->z0);
		int maxZ = (int)floor(bb->z1);
		int topY = (int)floor(bb->y1); 

		for (int by = topY; by < topY + 3; by++) {
			for (int bx = minX; bx <= maxX; bx++) {
				for (int bz = minZ; bz <= maxZ; bz++) {
					if (level->getTile(bx, by, bz) != 0) {
						level->destroyTile(bx, by, bz, true);
					}
				}
			}
		}
	}

	if (getLaunched() && getPrelaunchTime() <= 0) {
		if (getFueled()) { 
			setFueled(false); 
		}
		ticksUntilNextSound = 0;
		if (yd <= 1.0f) {
			yd += 0.02f;
		}
	}

	if (getLaunched() && getPrelaunchTime() > 0) {
		setPrelaunchTime(getPrelaunchTime() - 1);
	}

	if (getLaunched() && getPrelaunchTime() <= 0) {
		level->addParticle(eParticleType_flame, x, y + 0.1f, z, 0, 0, 0);
		level->addParticle(eParticleType_smokeBig, x, y + 0.3f, z, 0, -4.0f, 0);
		level->addParticle(eParticleType_fierySmoke, x, y + 0.3f, z, 0, -3.0f, 0);
	}else if (getLaunched() && getPrelaunchTime() > 0) {
		level->addParticle(eParticleType_smokeMedium, x, y + 0.3f, z, 0.1f, -0.1f, 0.1f);
		level->addParticle(eParticleType_smokeMedium, x, y + 0.3f, z, 0.1f, -0.1f, -0.1f);
		level->addParticle(eParticleType_smokeMedium, x, y + 0.3f, z, -0.1f, -0.1f, 0.1f);
		level->addParticle(eParticleType_smokeMedium, x, y + 0.3f, z, -0.1f, -0.1f, -0.1f);
	}

	// Interpolation
	if (level->isClientSide && doLerp)
	{
		if (lSteps > 0)
		{
			double xt = x + (lx - x) / lSteps;
			double yt = y + (ly - y) / lSteps;
			double zt = z + (lz - z) / lSteps;

			lSteps--;
			setPos(xt, yt, zt);
		}
		else
		{
			move(xd, yd, zd);

			xd *= 0.99f;
			yd *= 0.95f;
			zd *= 0.99f;
		}
		return;
	}

	if(getPrelaunchTime() > 0) {
		if (!level->isClientSide || (level->hasChunkAt((int) x, 0, (int) z) && level->getChunkAt((int) x, (int) z)->loaded))
		{
			int* id = &(level->dimension->id);
			if (id != nullptr && *id == 2) {
				yd -= 0.0133333333;
			}
			else {
				yd -= 0.08;
			}
		}
		else if (y > 0)
		{
			yd = -0.1;
		}
		else
		{
			yd = 0;
		}
	}
	

	move(xd, yd, zd);

	if (ticksUntilNextSound <= 0) {
		if (getPrelaunchTime() <= 0) {
			if(!hasPlayedLaunchSound) {
				level->playSound(x, y, z, eSoundType_RANDOM_EXPLODE, 1.0f, 0.5f);
				level->playRocketSound(x,y,z,this->getRocketId());
				hasPlayedLaunchSound = true;
			}
			level->moveRocketSound(x,y,z,this->getRocketId());
		}
		else {
			level->playSound(x, y, z, eSoundType_MOB_ROCKET_RUMBLE, 1.0f, 0.5f);
			ticksUntilNextSound = (20 * 3) - 1;
		}
	}
	else if (getLaunched()) {
		ticksUntilNextSound--;
	}

	if (getCooldown() > 0) {
		setCooldown(getCooldown() - 1);
	}

	// Server code after this
	if (level->isClientSide) return;

	if (rider.lock() != NULL)
	{
		if (rider.lock()->removed) rider = weak_ptr<Entity>();
	}
}

void Rocket::lerpTo(double x, double y, double z, float yRot, float xRot, int steps)
{
	if (doLerp)
	{
		lSteps = steps + 5;
	}
	else
	{
		double xdiff = x - this->x;
		double ydiff = y - this->y;
		double zdiff = z - this->z;
		double diff = xdiff * xdiff + ydiff * ydiff + zdiff * zdiff;

		if (diff > 1)
		{
			lSteps = 3;
		}
		else
		{
			return;
		}
	}

	lx = x;
	ly = y;
	lz = z;
	lyr = yRot;
	lxr = xRot;

	xd = lxd;
	yd = lyd;
	zd = lzd;
}

void Rocket::lerpMotion(double xd, double yd, double zd)
{
	lxd = this->xd = xd;
	lyd = this->yd = yd;
	lzd = this->zd = zd;
}

bool Rocket::getDoLerp()
{
	return doLerp;
}

void Rocket::setDoLerp(bool doLerp)
{
	this->doLerp = doLerp;
}

void Rocket::positionRider()
{
	if (rider.lock() == NULL) return;
	// We minus 0.4 to ensure that the player is not hovering above the Rocket.
	rider.lock()->setPos(x, y + getRideHeight() + rider.lock()->getRidingHeight() - 0.4, z);
}

bool Rocket::interact(shared_ptr<Player> player)
{
	if ((rider.lock() != NULL) && rider.lock()->instanceof(eTYPE_PLAYER)) return true;
	if (level->isClientSide) {
		if (player->getCarriedItem() != NULL && player->getCarriedItem()->id == Item::fuelCanister_Id && !getFueled()) {
			level->playLocalSound(x, y, z, eSoundType_RANDOM_DRINK, 1.0F, 1.0F, false);
		}else if (player->getCarriedItem() != NULL && player->getCarriedItem()->id == Item::cake_Id && !getCake()) {
			level->playLocalSound(x, y, z, eSoundType_RANDOM_BURP, 1.0F, 1.0F, false);
		}
	}
	if (!level->isClientSide)
	{
		if (player->getCarriedItem() != NULL && player->getCarriedItem()->id == Item::fuelCanister_Id && !getFueled()) {
			setFueled(true);
			if(!player->abilities.instabuild) player->getCarriedItem()->remove(1);
		}
		else if (player->getCarriedItem() != NULL && player->getCarriedItem()->id == Item::cake_Id && !getCake()) {
			setFueled(true);
			setCake(true);
			if(!player->abilities.instabuild) player->getCarriedItem()->remove(1);
		}
		else if(getFueled() && getCooldown() <= 0) {
			if (isLaunchBlocked()) {
				player->sendMessage(L"There are blocks above the rocket blocking the launch.");
				setCooldown(3 * 20);
				return true;
			}
			player->ride(rider.lock() == player ? NULL : shared_from_this());
			setLaunched(true);
			ticksUntilNextSound = 0;
			if (player != NULL && !player->inventory->getOxygenSetup() && !player->abilities.instabuild && player->dimension != 2) {
				player->sendMessage(L"You don't have an oxygen setup! Are you sure you want to do this?");
			}
		}
		else {
			if (getCooldown() > 0) {
				wstringstream ss;
				ss << (getCooldown () / 20);
				player->sendMessage(L"Rocket cooldown for " + ss.str() + L" seconds.");
			}
			else {
				player->sendMessage(L"The rocket has not been fueled yet!");
			}
		}
	}
	return true;
}

void Rocket::remove() {
	level->removeRocketSound(x, y, z, this->getRocketId());
	Entity::remove();
}

bool Rocket::shouldFall(int tile) {
	//|| tile == Tile::oil_Id || tile == Tile::calmOil_Id
	return (tile == 0 || tile == Tile::water_Id || tile == Tile::calmWater_Id || tile == Tile::lava_Id || tile == Tile::calmLava_Id);
}

bool Rocket::hurt(DamageSource* source, float hurtDamage)
{
	if (isInvulnerable() || (getLaunched() && getPrelaunchTime() <= 0)) return false;
	if (rider.lock() != NULL && rider.lock() == source->getEntity()) return false;
	if (level->isClientSide || removed) return true;

	// 4J-JEV: Fix for #88212,
	// Untrusted players shouldn't be able to damage minecarts or boats.
	if (dynamic_cast<EntityDamageSource*>(source) != NULL)
	{
		shared_ptr<Entity> attacker = source->getDirectEntity();

		if (attacker->instanceof(eTYPE_PLAYER) && !dynamic_pointer_cast<Player>(attacker)->isAllowedToHurtEntity(shared_from_this()))
		{
			return false;
		}
	}

	setDamage(getDamage() + hurtDamage * 10);
	markHurt();

	// 4J Stu - Brought froward from 12w36 to fix #46611 - TU5: Gameplay: Minecarts and boat requires more hits than one to be destroyed in creative mode
	// 4J-PB - Fix for XB1 #175735 - [CRASH] [Multi-Plat]: Code: Gameplay: Placing a boat on harmful surfaces causes the game to crash
	bool creativePlayer = (source->getEntity() != NULL) && source->getEntity()->instanceof(eTYPE_PLAYER) && dynamic_pointer_cast<Player>(source->getEntity())->abilities.instabuild;

	if (creativePlayer || getDamage() > 20 * 2)
	{
		if (rider.lock() != NULL) rider.lock()->ride(NULL);
		if (!creativePlayer || hasCustomName()) {
			shared_ptr<ItemInstance> rocket;
			if (getCake()) {
				rocket = make_shared<ItemInstance>(Item::rocketCake, 1);
			}else {
				rocket = make_shared<ItemInstance>(Item::rocket, 1);
			}
			if (!customName.empty()) rocket->setHoverName(customName);
			spawnAtLocation(rocket, 0);
			if (getFueled()) {
				spawnAtLocation(Item::fuelCanister_Id, 1, 0);
			}
		}
		remove();
	}
	return true;
}

float Rocket::getShadowHeightOffs()
{
	return 0;
}

wstring Rocket::getName()
{
	if (!customName.empty()) return customName;
	return app.GetString(IDS_ROCKET);
}

wstring Rocket::getAName()
{
	if (!customName.empty()) return customName;
	return Entity::getAName();
}

void Rocket::setLaunched(bool launched) {
	entityData->set(DATA_HAS_LAUNCHED, launched);
}

bool Rocket::getLaunched() {
	return entityData->getInteger(DATA_HAS_LAUNCHED);
}

void Rocket::setPrelaunchTime(int preLaunch) {
	entityData->set(DATA_PRE_LAUNCH, preLaunch);
}

int Rocket::getPrelaunchTime() {
	return entityData->getInteger(DATA_PRE_LAUNCH);
}

void Rocket::setFueled(bool isFueled) {
	entityData->set(DATA_FUELED, isFueled);
}

bool Rocket::getFueled() {
	return entityData->getInteger(DATA_FUELED);
}

void Rocket::setDamage(float damage)
{
	entityData->set(DATA_ID_DAMAGE, damage);
}

float Rocket::getDamage()
{
	return entityData->getFloat(DATA_ID_DAMAGE);
}

void Rocket::setRocketId(int rocketId)
{
	entityData->set(DATA_ROCKET_ID, rocketId);
}

int Rocket::getRocketId()
{
	return entityData->getInteger(DATA_ROCKET_ID);
}

void Rocket::setCooldown(int cooldown)
{
	entityData->set(DATA_COOLDOWN, cooldown);
}

int Rocket::getCooldown()
{
	return entityData->getInteger(DATA_COOLDOWN);
}

void Rocket::setCake(bool cake)
{
	entityData->set(DATA_CAKE, cake);
}

bool Rocket::getCake()
{
	return entityData->getInteger(DATA_CAKE);
}

bool Rocket::hasCustomName()
{
	return !customName.empty();
}

wstring Rocket::getCustomName()
{
	return customName;
}

void Rocket::setCustomName(wstring name) {
	this->customName = name;
}

bool Rocket::isLaunchBlocked() {
	int rocketX = (int)floor(x);
	int rocketZ = (int)floor(z);
	int rocketY = (int)floor(bb->y0);

	int topY = level->getTopSolidBlock(rocketX, rocketZ);
	if (topY != -1 && topY > rocketY) return true;

	return false;
}

void Rocket::addAdditonalSaveData(CompoundTag* base)
{
	base->putBoolean(L"fueled", getFueled());
	base->putBoolean(L"hasLaunched", getLaunched());
	base->putInt(L"prelaunch", getPrelaunchTime());
	base->putInt(L"cake", getCake());
	//base->putInt(L"rocketId", getRocketId());
	if (!customName.empty()) base->putString(L"customName", customName);
}

void Rocket::readAdditionalSaveData(CompoundTag* base)
{
	setFueled(base->getBoolean(L"fueled"));
	setLaunched(base->getBoolean(L"hasLaunched"));
	setPrelaunchTime(base->getInt(L"prelaunch"));
	setCake(base->getInt(L"cake"));
	//setRocketId(base->getInt(L"rocketId"));
	if (base->contains(L"customName") && base->getString(L"customName").length() > 0) setCustomName(base->getString(L"customName"));
}