#include "stdafx.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.phys.h"
#include "ItemInstance.h"
#include "BuggyItem.h"
//#include "AlienVillager.h"

BuggyItem::BuggyItem(int id) : Item(id)
{
	maxStackSize = 1;
	DispenserTile::REGISTRY.add(this, new BuggyDispenseBehavior());
}

shared_ptr<ItemInstance> BuggyItem::BuggyDispenseBehavior::execute(BlockSource* source, shared_ptr<ItemInstance> dispensed, eOUTCOME& outcome)
{
	FacingEnum* facing = DispenserTile::getFacing(source->getData());
	Level* world = source->getWorld();

	double spawnX = source->getX() + facing->getStepX() * (1 + 12.0f / 16);
	double spawnY = source->getY() + facing->getStepY() * (1 + 2.0f / 16);
	double spawnZ = source->getZ() + facing->getStepZ() * (1 + 12.0f / 16);

	outcome = ACTIVATED_ITEM;

	/*shared_ptr<Buggy> buggy = make_shared<Buggy>(world, spawnX, spawnY, spawnZ);
	buggy->yRot = 180 * facing->getDataValue();
	if (dispensed->hasCustomHoverName())
	{
		buggy->setCustomName(dispensed->getHoverName());
	}
	world->addEntity(buggy);*/

	dispensed->remove(1);
	return dispensed;
}

void BuggyItem::BuggyDispenseBehavior::playSound(BlockSource* source)
{
	source->getWorld()->levelEvent(LevelEvent::SOUND_CLICK, source->getBlockX(), source->getBlockY(), source->getBlockZ(), 0);
}

bool BuggyItem::TestUse(shared_ptr<ItemInstance> itemInstance, Level* level, shared_ptr<Player> player)
{
	// 4J-PB - added for tooltips to test use
	// 4J TODO really we should have the crosshair hitresult telling us if it hit water, and at what distance, so we don't need to do this again
	// if the player happens to have a boat in their hand

	float xRot = player->xRotO + (player->xRot - player->xRotO);
	float yRot = player->yRotO + (player->yRot - player->yRotO);

	double x = player->xo + (player->x - player->xo);
	double y = player->yo + (player->y - player->yo) + 1.62 - player->heightOffset;
	double z = player->zo + (player->z - player->zo);

	Vec3* from = Vec3::newTemp(x, y, z);

	float yCos = Mth::cos(-yRot * Mth::RAD_TO_GRAD - PI);
	float ySin = Mth::sin(-yRot * Mth::RAD_TO_GRAD - PI);
	float xCos = -Mth::cos(-xRot * Mth::RAD_TO_GRAD);
	float xSin = Mth::sin(-xRot * Mth::RAD_TO_GRAD);

	float xa = ySin * xCos;
	float ya = xSin;
	float za = yCos * xCos;

	double range = 5;
	Vec3* to = from->add(xa * range, ya * range, za * range);
	HitResult* hr = level->clip(from, to, true);
	if (hr == NULL) return false;

	if (hr->type == HitResult::TILE)
	{
		delete hr;
		return true;
	}
	delete hr;
	return false;
}
shared_ptr<ItemInstance> BuggyItem::use(shared_ptr<ItemInstance> itemInstance, Level* level, shared_ptr<Player> player)
{
	float a = 1;

	float xRot = player->xRotO + (player->xRot - player->xRotO) * a;
	float yRot = player->yRotO + (player->yRot - player->yRotO) * a;

	double x = player->xo + (player->x - player->xo) * a;
	double y = player->yo + (player->y - player->yo) * a + 1.62 - player->heightOffset;
	double z = player->zo + (player->z - player->zo) * a;

	Vec3* from = Vec3::newTemp(x, y, z);

	float yCos = Mth::cos(-yRot * Mth::RAD_TO_GRAD - PI);
	float ySin = Mth::sin(-yRot * Mth::RAD_TO_GRAD - PI);
	float xCos = -Mth::cos(-xRot * Mth::RAD_TO_GRAD);
	float xSin = Mth::sin(-xRot * Mth::RAD_TO_GRAD);

	float xa = ySin * xCos;
	float ya = xSin;
	float za = yCos * xCos;

	double range = 5;
	Vec3* to = from->add(xa * range, ya * range, za * range);
	HitResult* hr = level->clip(from, to, true);
	if (hr == NULL) return itemInstance;

	// check entity collision
	Vec3* b = player->getViewVector(a);
	bool hitEntity = false;
	float overlap = 1;
	vector<shared_ptr<Entity> >* objects = level->getEntities(player, player->bb->expand(b->x * (range), b->y * (range), b->z * (range))->grow(overlap, overlap, overlap));

	for (vector<shared_ptr<Entity>>::iterator it = objects->begin(); it != objects->end(); ++it)
	{
		shared_ptr<Entity> e = *it;
		if (!e->isPickable()) continue;

		float rr = e->getPickRadius();
		AABB* bb = e->bb->grow(rr, rr, rr);
		if (bb->contains(from))
		{
			hitEntity = true;
		}
	}
	if (hitEntity)
	{
		return itemInstance;
	}

	if (hr->type == HitResult::TILE)
	{
		int xt = hr->x;
		int yt = hr->y;
		int zt = hr->z;

		if (level->getTile(xt, yt, zt) == Tile::topSnow_Id) yt--;
		shared_ptr<Buggy> buggy = std::make_shared<Buggy>(level, xt + 0.5f, yt + 1.0f, zt + 0.5f);
		buggy->yRot = (((Mth::floor(player->yRot * 4.0F / 360.0F + 0.5) & 0x3) - 1) * 90) - 90;
		if (!level->getCubes(buggy, buggy->bb->grow(-.1, -.1, -.1))->empty())
		{
			return itemInstance;
		}
		if (!level->isClientSide)
		{
			if (itemInstance->hasCustomHoverName()) {
				buggy->setCustomName(itemInstance->getHoverName());
			}
			level->addEntity(buggy);
		}
		if (!player->abilities.instabuild)
		{
			itemInstance->count--;
		}
	}
	delete hr;

	return itemInstance;
}