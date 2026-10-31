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
#include "Buggy.h"
#include "../Minecraft.Client/Minecraft.h"
#include "AnimalChest.h"
#include "net.minecraft.network.packet.h"
#include "Dimension.h"

const double Buggy::MAX_SPEED = 0.7;
const double Buggy::ACCELERATION = 0.07;

// 4J - added for common ctor code
void Buggy::_init()
{
	wheelRotation = 0.0f;
	noCollisionTicks = 0;

	this->defineSynchedData();

	lSteps = 0;
	lx = ly = lz = lyr = lxr = 0.0;
	lxd = lyd = lzd = 0.0;

	blocksBuilding = true;
	//setSize(1.5f, 0.6f);

	setSize(2.5f,2.5f);
	items = ItemInstanceArray(9*4);
	customName = L"";
}

Buggy::Buggy(Level* level) : Entity(level)
{
	_init();
}

bool Buggy::makeStepSound()
{
	return false;
}

void Buggy::defineSynchedData()
{
	entityData->define(DATA_ID_DAMAGE, false);
	entityData->define(DATA_ID_HAS_STORAGE, false);
}


AABB* Buggy::getCollideAgainstBox(shared_ptr<Entity> entity)
{
	return entity->bb;
}

AABB* Buggy::getCollideBox()
{
	return bb;
}

bool Buggy::isPushable()
{
	return false;
}

Buggy::Buggy(Level* level, double x, double y, double z) : Entity(level)
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

double Buggy::getRideHeight()
{
	return heightOffset + 1.0F;
}

bool Buggy::isPickable()
{
	return !removed;
}

void Buggy::tick()
{
	Entity::tick();
	if (getDamage() > 0) setDamage(getDamage() - 1);
	xo = x;
	yo = y;
	zo = z;

	double lastSpeed = sqrt(xd * xd + zd * zd);

	// Interpolation
	if (level->isClientSide)
	{
		if (lSteps > 0)
		{
			double xt = x + (lx - x) / lSteps;
			double yt = y + (ly - y) / lSteps;
			double zt = z + (lz - z) / lSteps;

			double yrd = Mth::wrapDegrees(lyr - yRot);

			yRot += (float) ( (yrd) / lSteps );
			xRot += (float) ( (lxr - xRot) / lSteps );

			lSteps--;
			setPos(xt, yt, zt);
			setRot(yRot, xRot);
		}
		else
		{
			move(xd, yd, zd);
			xd *= 0.95f;
			yd *= 0.95f;
			zd *= 0.95f;
		}
	}

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

	if (rider.lock() != NULL && rider.lock()->instanceof(eTYPE_LIVINGENTITY))
	{
		shared_ptr<LivingEntity> livingRider = dynamic_pointer_cast<LivingEntity>(rider.lock());

		float forward = livingRider->yya;
		float strafe = livingRider->xxa; 

		if(forward > 0.000001 || forward < -0.000001) {
			//yRotO = yRot -= strafe * 4.0f;
			double rotDiff = Mth::wrapDegrees((livingRider->yRot + 180) - yRot);

			float turnSpeed = 4.0f;

			if(rotDiff > turnSpeed) rotDiff = turnSpeed;
			else if(rotDiff < -turnSpeed) rotDiff = -turnSpeed;

			yRot += (float)rotDiff;
		}

		double dirX = -sin(yRot * PI / 180.0);
		double dirZ = cos(yRot * PI / 180.0);

		xd -= dirX * forward * ACCELERATION;
		zd -= dirZ * forward * ACCELERATION;

		double forwardSpeed = xd * dirX + zd * dirZ;

		wheelRotation += (forwardSpeed / 8) * 90.0f * (PI / 180.0);
	}

	double curSpeed = sqrt(xd * xd + zd * zd);

	if (curSpeed > MAX_SPEED)
	{
		double ratio = MAX_SPEED / curSpeed;

		xd *= ratio;
		zd *= ratio;
		curSpeed = MAX_SPEED;
	}

	footSize = 1;
	move(xd, yd, zd);

	xd *= 0.95f;
	yd *= 0.95f;
	zd *= 0.95f;

	setRot(yRot, xRot);

	//Old climbing code, was pretty jank
	/*if (horizontalCollision) {
		noCollisionTicks = 0;

		//int blockAtFeet = level->getTile(Mth::floor(x), Mth::floor(bb->y0), Mth::floor(z));
		//int blockAtChest = level->getTile(Mth::floor(x), Mth::floor(bb->y0 + 0.6), Mth::floor(z));
		if (getHasBeenClimbing()) {
			fallDistance = 0;
			if (onGround && yd < 0) {
				yd = 0.0f;
			}

			setClimbingTicks(getClimbingTicks() + 1);
			float modifier = 1.0f - (getClimbingTicks() / MAX_CLIMB_TICKS);
			if (modifier < 0) modifier = 0;
			yd = 0.2f * modifier;
		}
		else {
			yd = 0.2f;
			setHasBeenClimbing(true);
			setClimbingTicks(0);
		}

		if (!(blockAtFeet > 0 && blockAtChest == 0)) {
			xd = 0.0f;
			zd = 0.0f;
		}
	}
	else {
		noCollisionTicks++;
		if (noCollisionTicks >= MAX_COLLISION_TICKS) {
			setHasBeenClimbing(false);
			setClimbingTicks(0);
		}
	}*/

	// Server code after this
	if (level->isClientSide) return;

	vector<shared_ptr<Entity> >* entities = level->getEntities(shared_from_this(), bb->grow(0.2f, 0, 0.2f));
	if (entities && !entities->empty())
	{
		double xm = (bb->x0 + bb->x1) / 2;
		double zm = (bb->z0 + bb->z1) / 2;
		AUTO_VAR(riderPtr, rider.lock());
		for (vector<shared_ptr<Entity> >::iterator it = entities->begin(); it != entities->end(); ++it)
		{
			shared_ptr<Entity> e = *it;
			if (e != riderPtr && e->isPushable() && e->GetType() == eTYPE_BUGGY)
			{
				e->push(shared_from_this());
			}
			else if (e != riderPtr && e->isPushable() && curSpeed > 0.1f) {
				e->hurt(DamageSource::genericSource, 1.0f * (curSpeed/2));

				double px = e->x - xm;
				double pz = e->z - zm;
				double dd = sqrt(px * px + pz * pz);
				double push = (4 * curSpeed) / dd;
				e->push(px * push, 0.2f, pz * push);
			}
		}
	}

	int minX = (int)floor(bb->x0);
	int maxX = (int)ceil(bb->x1);
	int minY = (int)floor(bb->y0);
	int maxY = (int)ceil(bb->y1);
	int minZ = (int)floor(bb->z0);
	int maxZ = (int)ceil(bb->z1);

	for (int bx = minX - 2; bx < maxX + 2; bx++) {
		for (int by = minY; by < maxY + 2; by++) {
			for (int bz = minZ - 2; bz < maxZ + 2; bz++) {
				int tile = level->getTile(bx, by, bz);

				if (tile == Tile::flower_Id || tile == Tile::rose_Id || tile == Tile::tallgrass_Id || tile == Tile::deadBush_Id)
				{
					level->destroyTile(bx, by, bz, true);
				}
			}
		}
	}

	if (rider.lock() != NULL)
	{
		if (rider.lock()->removed) rider = weak_ptr<Entity>();
	}

	if (rider.lock() == NULL) {
		xd = 0;
		zd = 0;
	}
}

void Buggy::positionRider()
{
	if (rider.lock() == NULL) return;
	rider.lock()->setPos(x, y + getRideHeight() + rider.lock()->getRidingHeight() - 0.4, z);
}

float Buggy::getShadowHeightOffs()
{
	return 0;
}

bool Buggy::interact(shared_ptr<Player> player)
{
	if ((rider.lock() != NULL) && rider.lock()->instanceof(eTYPE_PLAYER) && (rider.lock() != player)) return true;
	if (!level->isClientSide)
	{
		if (player->isSneaking()) {
			shared_ptr<ItemInstance> item = player->getCarriedItem();
			if (item != NULL && item->id == Item::buggyStorage_Id && !this->getHasStorage()) {
				this->setHasStorage(true);
				item->remove(1);
				return true;
			}
			if (this->getHasStorage() && (rider.lock() == NULL || rider.lock() == player)) {
				player->openContainer(dynamic_pointer_cast<Container>(shared_from_this()));
				return true;
			}
		}
		player->ride(rider.lock() == player ? NULL : shared_from_this());
	}
	return true;
}

bool Buggy::hurt(DamageSource* source, float hurtDamage)
{
	if (isInvulnerable()) return false;
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
			shared_ptr<ItemInstance> buggy = make_shared<ItemInstance>(Item::buggy, 1);
			if (!customName.empty()) buggy->setHoverName(customName);
			spawnAtLocation(buggy, 0);
			if (getHasStorage()) {
				spawnAtLocation(Item::buggyStorage_Id, 1, 0);
			}
		}
		if (getHasStorage()) {
			dropInventory();
		}
		remove();
	}
	return true;
}

void Buggy::lerpTo(double x, double y, double z, float yRot, float xRot, int steps)
{
	lSteps = steps;

	lx = x;
	ly = y;
	lz = z;
	lyr = yRot;
	lxr = xRot;

	xd = lxd;
	yd = lyd;
	zd = lzd;
}

void Buggy::lerpMotion(double xd, double yd, double zd)
{
	lxd = this->xd = xd;
	lyd = this->yd = yd;
	lzd = this->zd = zd;
}

double Buggy::getWheelRot() {
	return this->wheelRotation;
}

bool Buggy::shouldFall(int tile) {
	return (tile == 0 || tile == Tile::water_Id || tile == Tile::calmWater_Id || tile == Tile::lava_Id || tile == Tile::calmLava_Id || tile == Tile::oil_Id || tile == Tile::calmOil_Id);
}

void Buggy::setDamage(float damage)
{
	entityData->set(DATA_ID_DAMAGE, damage);
}

float Buggy::getDamage()
{
	return entityData->getFloat(DATA_ID_DAMAGE);
}

void Buggy::setHasStorage(bool hasStorage)
{
	entityData->set(DATA_ID_HAS_STORAGE, hasStorage);
}

bool Buggy::getHasStorage()
{
	return entityData->getInteger(DATA_ID_HAS_STORAGE);
}

bool Buggy::hasCustomName()
{
	return !customName.empty();
}

wstring Buggy::getCustomName()
{
	return customName;
}

void Buggy::setCustomName(wstring name) {
	this->customName = name;
}

void Buggy::dropInventory()
{
	for (int i = 0; i < getContainerSize(); i++)
	{
		shared_ptr<ItemInstance> itemstack = getItem(i);
		if (itemstack == NULL)
		{
			continue;
		}
		spawnAtLocation(itemstack, 0);
	}
}

shared_ptr<ItemInstance> Buggy::getItem(unsigned int slot)
{
	return items[slot];
}

shared_ptr<ItemInstance> Buggy::removeItem(unsigned int slot, int count)
{
	if (items[slot] != NULL)
	{
		if (items[slot]->count <= count)
		{
			shared_ptr<ItemInstance> item = items[slot];
			items[slot] = NULL;
			return item;
		}
		else
		{
			shared_ptr<ItemInstance> i = items[slot]->remove(count);
			if (items[slot]->count == 0) items[slot] = NULL;
			return i;
		}
	}
	return NULL;
}

shared_ptr<ItemInstance> Buggy::removeItemNoUpdate(int slot)
{
	if (items[slot] != NULL)
	{
		shared_ptr<ItemInstance> item = items[slot];
		items[slot] = NULL;
		return item;
	}
	return NULL;
}

void Buggy::setItem(unsigned int slot, shared_ptr<ItemInstance> item)
{
	items[slot] = item;
	if (item != NULL && item->count > getMaxStackSize()) item->count = getMaxStackSize();
}

void Buggy::setChanged()
{
}

bool Buggy::stillValid(shared_ptr<Player> player)
{
	if (removed) return false;
	if (player->distanceToSqr(shared_from_this()) > 8 * 8) return false;
	return true;
}

void Buggy::startOpen()
{
}

void Buggy::stopOpen()
{
}

bool Buggy::canPlaceItem(int slot, shared_ptr<ItemInstance> item)
{
	return true;
}

wstring Buggy::getAName()
{
	if (!customName.empty()) return customName;
	return Entity::getAName();
}

wstring Buggy::getName()
{
	if (!customName.empty()) return customName;
	return app.GetString(IDS_BUGGY);
}

int Buggy::getMaxStackSize() 
{
	return Container::LARGE_MAX_STACK_SIZE;
}

int Buggy::getContainerType() {
	return ContainerOpenPacket::BUGGY_STORAGE;
}

void Buggy::addAdditonalSaveData(CompoundTag* base)
{
	base->putBoolean(L"hasStorage", getHasStorage());
	if (!customName.empty()) base->putString(L"customName", customName);
	
	ListTag<CompoundTag>* listTag = new ListTag<CompoundTag>();

	for (int i = 0; i < items.length; i++)
	{
		if (items[i] != NULL)
		{
			CompoundTag* tag = new CompoundTag();
			tag->putByte(L"Slot", static_cast<byte>(i));
			items[i]->save(tag);
			listTag->add(tag);
		}
	}
	base->put(L"Items", listTag);
}

void Buggy::readAdditionalSaveData(CompoundTag* base)
{
	setHasStorage(base->getBoolean(L"hasStorage"));
	if (base->contains(L"customName") && base->getString(L"customName").length() > 0) setCustomName(base->getString(L"customName"));
	
	ListTag<CompoundTag>* inventoryList = (ListTag<CompoundTag> *) base->getList(L"Items");
	delete[] items.data;
	items = ItemInstanceArray(getContainerSize());
	for (int i = 0; i < inventoryList->size(); i++)
	{
		CompoundTag* tag = inventoryList->get(i);
		int slot = tag->getByte(L"Slot") & 0xff;
		if (slot >= 0 && slot < items.length) items[slot] = ItemInstance::fromTag(tag);
	}
}