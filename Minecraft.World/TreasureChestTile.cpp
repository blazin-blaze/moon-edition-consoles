#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.redstone.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.phys.h"
#include "TreasureChestTile.h"
#include "Facing.h"
#include "LevelEvent.h"

TreasureChestTile::TreasureChestTile(int id, int type) : BaseEntityTile(id, Material::stone, isSolidRender())
{
	random = new Random();
	this->type = type;

	setShape(1 / 16.0f, 0, 1 / 16.0f, 15 / 16.0f, 14 / 16.0f, 15 / 16.0f);
}

TreasureChestTile::~TreasureChestTile()
{
	delete random;
}

bool TreasureChestTile::isSolidRender(bool isServerLevel)
{
	return false;
}

bool TreasureChestTile::isCubeShaped()
{
	return false;
}

int TreasureChestTile::getRenderShape()
{
	return Tile::SHAPE_ENTITYTILE_ANIMATED;
}

void TreasureChestTile::updateShape(LevelSource* level, int x, int y, int z, int forceData, shared_ptr<TileEntity> forceEntity)
{
	if (level->getTile(x, y, z - 1) == id)
	{
		setShape(1 / 16.0f, 0, 0, 15 / 16.0f, 14 / 16.0f, 15 / 16.0f);
	}
	else if (level->getTile(x, y, z + 1) == id)
	{
		setShape(1 / 16.0f, 0, 1 / 16.0f, 15 / 16.0f, 14 / 16.0f, 1);
	}
	else if (level->getTile(x - 1, y, z) == id)
	{
		setShape(0, 0, 1 / 16.0f, 15 / 16.0f, 14 / 16.0f, 15 / 16.0f);
	}
	else if (level->getTile(x + 1, y, z) == id)
	{
		setShape(1 / 16.0f, 0, 1 / 16.0f, 1, 14 / 16.0f, 15 / 16.0f);
	}
	else
	{
		setShape(1 / 16.0f, 0, 1 / 16.0f, 15 / 16.0f, 14 / 16.0f, 15 / 16.0f);
	}
}

void TreasureChestTile::onPlace(Level* level, int x, int y, int z)
{
	BaseEntityTile::onPlace(level, x, y, z);
	recalcLockDir(level, x, y, z);

	int n = level->getTile(x, y, z - 1); // face = 2
	int s = level->getTile(x, y, z + 1); // face = 3
	int w = level->getTile(x - 1, y, z); // face = 4
	int e = level->getTile(x + 1, y, z); // face = 5
	if (n == id) recalcLockDir(level, x, y, z - 1);
	if (s == id) recalcLockDir(level, x, y, z + 1);
	if (w == id) recalcLockDir(level, x - 1, y, z);
	if (e == id) recalcLockDir(level, x + 1, y, z);
}

void TreasureChestTile::setPlacedBy(Level* level, int x, int y, int z, shared_ptr<LivingEntity> by, shared_ptr<ItemInstance> itemInstance)
{
	int n = level->getTile(x, y, z - 1); // face = 2
	int s = level->getTile(x, y, z + 1); // face = 3
	int w = level->getTile(x - 1, y, z); // face = 4
	int e = level->getTile(x + 1, y, z); // face = 5

	int facing = 0;
	int dir = (Mth::floor(by->yRot * 4 / (360) + 0.5)) & 3;

	if (dir == 0) facing = Facing::NORTH;
	if (dir == 1) facing = Facing::EAST;
	if (dir == 2) facing = Facing::SOUTH;
	if (dir == 3) facing = Facing::WEST;

	if (n != id && s != id && w != id && e != id)
	{
		level->setData(x, y, z, facing, Tile::UPDATE_ALL);
	}
	else
	{
		if ((n == id || s == id) && (facing == Facing::WEST || facing == Facing::EAST))
		{
			if (n == id) level->setData(x, y, z - 1, facing, Tile::UPDATE_ALL);
			else level->setData(x, y, z + 1, facing, Tile::UPDATE_ALL);
			level->setData(x, y, z, facing, Tile::UPDATE_ALL);
		}
		if ((w == id || e == id) && (facing == Facing::NORTH || facing == Facing::SOUTH))
		{
			if (w == id) level->setData(x - 1, y, z, facing, Tile::UPDATE_ALL);
			else level->setData(x + 1, y, z, facing, Tile::UPDATE_ALL);
			level->setData(x, y, z, facing, Tile::UPDATE_ALL);
		}
	}

	if (itemInstance->hasCustomHoverName())
	{
		dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x, y, z))->setCustomName(itemInstance->getHoverName());
	}

}

void TreasureChestTile::recalcLockDir(Level* level, int x, int y, int z)
{
	if (level->isClientSide)
	{
		return;
	}

	int n = level->getTile(x, y, z - 1); // face = 2
	int s = level->getTile(x, y, z + 1); // face = 3
	int w = level->getTile(x - 1, y, z); // face = 4
	int e = level->getTile(x + 1, y, z); // face = 5

	// Long!
	int lockDir = 4;
	if (n == id || s == id)
	{
		int w2 = level->getTile(x - 1, y, n == id ? z - 1 : z + 1);
		int e2 = level->getTile(x + 1, y, n == id ? z - 1 : z + 1);

		lockDir = 5;

		int otherDir = -1;
		if (n == id) otherDir = level->getData(x, y, z - 1);
		else otherDir = level->getData(x, y, z + 1);
		if (otherDir == 4) lockDir = 4;

		if ((Tile::solid[w] || Tile::solid[w2]) && !Tile::solid[e] && !Tile::solid[e2]) lockDir = 5;
		if ((Tile::solid[e] || Tile::solid[e2]) && !Tile::solid[w] && !Tile::solid[w2]) lockDir = 4;
	}
	else if (w == id || e == id)
	{
		int n2 = level->getTile(w == id ? x - 1 : x + 1, y, z - 1);
		int s2 = level->getTile(w == id ? x - 1 : x + 1, y, z + 1);

		lockDir = 3;
		int otherDir = -1;
		if (w == id) otherDir = level->getData(x - 1, y, z);
		else otherDir = level->getData(x + 1, y, z);
		if (otherDir == 2) lockDir = 2;

		if ((Tile::solid[n] || Tile::solid[n2]) && !Tile::solid[s] && !Tile::solid[s2]) lockDir = 3;
		if ((Tile::solid[s] || Tile::solid[s2]) && !Tile::solid[n] && !Tile::solid[n2]) lockDir = 2;
	}
	else
	{
		lockDir = 3;
		if (Tile::solid[n] && !Tile::solid[s]) lockDir = 3;
		if (Tile::solid[s] && !Tile::solid[n]) lockDir = 2;
		if (Tile::solid[w] && !Tile::solid[e]) lockDir = 5;
		if (Tile::solid[e] && !Tile::solid[w]) lockDir = 4;
	}

	level->setData(x, y, z, lockDir, Tile::UPDATE_ALL);
}

bool TreasureChestTile::mayPlace(Level* level, int x, int y, int z)
{
	int chestCount = 0;

	if (level->getTile(x - 1, y, z) == id) chestCount++;
	if (level->getTile(x + 1, y, z) == id) chestCount++;
	if (level->getTile(x, y, z - 1) == id) chestCount++;
	if (level->getTile(x, y, z + 1) == id) chestCount++;

	if (chestCount >= 1) return false;

	/*if (isFullChest(level, x - 1, y, z)) return false;
	if (isFullChest(level, x + 1, y, z)) return false;
	if (isFullChest(level, x, y, z - 1)) return false;
	if (isFullChest(level, x, y, z + 1)) return false;*/
	return true;

}

void TreasureChestTile::neighborChanged(Level* level, int x, int y, int z, int type)
{
	BaseEntityTile::neighborChanged(level, x, y, z, type);
	shared_ptr<ChestTileEntity>(cte) = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x, y, z));
	if (cte != NULL) cte->clearCache();
}

void TreasureChestTile::onRemove(Level* level, int x, int y, int z, int id, int data)
{
	shared_ptr<Container> container = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x, y, z));
	if (container != NULL)
	{
		for (unsigned int i = 0; i < container->getContainerSize(); i++)
		{
			shared_ptr<ItemInstance> item = container->getItem(i);
			if (item != NULL)
			{
				float xo = random->nextFloat() * 0.8f + 0.1f;
				float yo = random->nextFloat() * 0.8f + 0.1f;
				float zo = random->nextFloat() * 0.8f + 0.1f;

				while (item->count > 0)
				{
					int count = random->nextInt(21) + 10;
					if (count > item->count) count = item->count;
					item->count -= count;

					shared_ptr<ItemInstance> newItem = std::make_shared<ItemInstance>(item->id, count, item->getAuxValue());
					newItem->set4JData(item->get4JData());
					shared_ptr<ItemEntity> itemEntity = std::make_shared<ItemEntity>(level, x + xo, y + yo, z + zo, newItem);
					float pow = 0.05f;
					itemEntity->xd = static_cast<float>(random->nextGaussian()) * pow;
					itemEntity->yd = static_cast<float>(random->nextGaussian()) * pow + 0.2f;
					itemEntity->zd = static_cast<float>(random->nextGaussian()) * pow;
					if (item->hasTag())
					{
						itemEntity->getItem()->setTag(static_cast<CompoundTag*>(item->getTag()->copy()));
					}

					level->addEntity(itemEntity);
				}

				// 4J Stu - Fix for duplication glitch
				container->setItem(i, NULL);
			}
		}
		level->updateNeighbourForOutputSignal(x, y, z, id);
	}
	BaseEntityTile::onRemove(level, x, y, z, id, data);
}

// 4J-PB - Adding a TestUse for tooltip display
bool TreasureChestTile::TestUse()
{
	return true;
}

// 4J-PB - changing to 1.5 equivalent
bool TreasureChestTile::use(Level* level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly/*=false*/) // 4J added soundOnly param
{
	if (soundOnly) return true;

	shared_ptr<ItemInstance> usedItem = player->getCarriedItem();
	shared_ptr<ChestTileEntity> entity = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x, y, z));

	if (level->isClientSide)
	{
		return true;
	}

	if (usedItem != NULL && usedItem->id == Item::moonDungeonKey_Id && entity != NULL && entity->isLocked) {
		entity->isLocked = false;
		if (!player->abilities.instabuild) {
			usedItem->remove(1);
		}
		if (usedItem->count == 0) {
			usedItem = NULL;
		}
		//level->levelEvent(player, LevelEvent::SOUND_CLICK, x, y, z, 0);
		level->playSound(x, y, z, eSoundType_TILE_TREASURE_UNLOCK, 1.0F, 1.0F, false);
		return true;
	}

	shared_ptr<Container> container = getContainer(level, x, y, z);

	if (container != NULL)
	{
		if (!entity->isLocked) {
			player->openContainer(container);
		}
		else {
			level->playSound(x, y, z, eSoundType_TILE_TREASURE_LOCKED, 1.0F, 1.0F, false);
		}
	}

	return true;
}

shared_ptr<Container> TreasureChestTile::getContainer(Level* level, int x, int y, int z)
{
	shared_ptr<Container> container = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x, y, z));
	if (container == NULL) return NULL;

	if (level->isSolidBlockingTile(x, y + 1, z)) return NULL;

	if (level->getTile(x - 1, y, z) == id && (level->isSolidBlockingTile(x - 1, y + 1, z))) return NULL;
	if (level->getTile(x + 1, y, z) == id && (level->isSolidBlockingTile(x + 1, y + 1, z))) return NULL;
	if (level->getTile(x, y, z - 1) == id && (level->isSolidBlockingTile(x, y + 1, z - 1))) return NULL;
	if (level->getTile(x, y, z + 1) == id && (level->isSolidBlockingTile(x, y + 1, z + 1))) return NULL;

	if (level->getTile(x - 1, y, z) == id) container = std::make_shared<CompoundContainer>(IDS_CHEST_LARGE, dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x - 1, y, z)), container);
	if (level->getTile(x + 1, y, z) == id) container = std::make_shared<CompoundContainer>(IDS_CHEST_LARGE, container, dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x + 1, y, z)));
	if (level->getTile(x, y, z - 1) == id) container = std::make_shared<CompoundContainer>(IDS_CHEST_LARGE, dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x, y, z - 1)), container);
	if (level->getTile(x, y, z + 1) == id) container = std::make_shared<CompoundContainer>(IDS_CHEST_LARGE, container, dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x, y, z + 1)));

	return container;
}

shared_ptr<TileEntity> TreasureChestTile::newTileEntity(Level* level)
{
	MemSect(50);
	shared_ptr<TileEntity> retval = std::make_shared<ChestTileEntity>(true, true);
	static_cast<ChestTileEntity*>(retval.get())->setCustomName(L"Moon Treasure Chest");
	MemSect(0);
	return retval;
}

bool TreasureChestTile::isSignalSource()
{
	return false;
}

int TreasureChestTile::getSignal(LevelSource* level, int x, int y, int z, int dir)
{
	return Redstone::SIGNAL_NONE;
}

int TreasureChestTile::getDirectSignal(LevelSource* level, int x, int y, int z, int dir)
{
	return Redstone::SIGNAL_NONE;
}

bool TreasureChestTile::hasAnalogOutputSignal()
{
	return false;
}

int TreasureChestTile::getAnalogOutputSignal(Level* level, int x, int y, int z, int dir)
{
	return AbstractContainerMenu::getRedstoneSignalFromContainer(getContainer(level, x, y, z));
}

void TreasureChestTile::registerIcons(IconRegister* iconRegister)
{
	// Register wood as the chest's icon, because it's used by the particles
	// when destroying the chest
	icon = iconRegister->registerIcon(L"planks_oak");
}