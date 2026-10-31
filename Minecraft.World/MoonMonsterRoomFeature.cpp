#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.item.h"
#include "WeighedTreasure.h"
#include "MoonMonsterRoomFeature.h"

WeighedTreasure* MoonMonsterRoomFeature::monsterRoomTreasure[MoonMonsterRoomFeature::TREASURE_ITEMS_COUNT] =
{
		new WeighedTreasure(Item::sapphire_Id, 0, 1, 12, 7),
		new WeighedTreasure(Item::titaniumIngot_Id, 0, 1, 2, 1),
		new WeighedTreasure(Item::cheese_Id, 0, 1, 15, 10),
		new WeighedTreasure(Item::netherQuartz_Id, 0, 1, 25, 10),
		new WeighedTreasure(Item::oxygenTank_Id, 0, 1, 1, 1),
		new WeighedTreasure(Item::cheeseburger_Id, 0, 1, 2, 7),
		new WeighedTreasure(Item::poutine_Id, 0, 1, 1, 5),
		new WeighedTreasure(Item::apple_gold_Id, 0, 1, 1, 7),
		new WeighedTreasure(Item::apple_gold_Id, 1, 1, 1, 5),
		new WeighedTreasure(Item::nameTag_Id, 0, 1, 1, 7),
		new WeighedTreasure(Item::diamond_Id, 0, 1, 10, 7),
		new WeighedTreasure(Item::sword_diamond_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::pickAxe_diamond_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::hatchet_diamond_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::hoe_diamond_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::shovel_diamond_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::dehydratedApple_Id, 0, 1, 3, 10),
		new WeighedTreasure(Item::dehydratedCarrot_Id, 0, 1, 3, 10),
		new WeighedTreasure(Item::dehydratedMelon_Id, 0, 1, 3, 10),
		new WeighedTreasure(Item::dehydratedPotato_Id, 0, 1, 3, 10),
		new WeighedTreasure(Item::cannedBeef_Id, 0, 1, 3, 10),
		new WeighedTreasure(Item::sapphireHelmet_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::sapphireChestplate_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::sapphireLeggings_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::sapphireBoots_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::sapphireSword_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::sapphirePickaxe_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::sapphireHatchet_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::sapphireShovel_Id, 0, 1, 1, 3),
		new WeighedTreasure(Item::sapphireHoe_Id, 0, 1, 1, 3),
};

bool MoonMonsterRoomFeature::place(Level* level, Random* random, int x, int y, int z)
{
	int hr = 3;
	int xr = random->nextInt(2) + 2;
	int zr = random->nextInt(2) + 2;

	int holeCount = 0;
	for (int xx = x - xr - 1; xx <= x + xr + 1; xx++)
	{
		for (int yy = y - 1; yy <= y + hr + 1; yy++)
		{
			for (int zz = z - zr - 1; zz <= z + zr + 1; zz++)
			{
				Material* m = level->getMaterial(xx, yy, zz);
				if (yy == y - 1 && !m->isSolid()) return false;
				if (yy == y + hr + 1 && !m->isSolid()) return false;

				if (xx == x - xr - 1 || xx == x + xr + 1 || zz == z - zr - 1 || zz == z + zr + 1)
				{
					if (yy == y && level->isEmptyTile(xx, yy, zz) && level->isEmptyTile(xx, yy + 1, zz))
					{
						holeCount++;
					}
				}

			}
		}
	}

	if (holeCount < 1 || holeCount > 5) return false;

	for (int xx = x - xr - 1; xx <= x + xr + 1; xx++)
	{
		for (int yy = y + hr; yy >= y - 1; yy--)
		{
			for (int zz = z - zr - 1; zz <= z + zr + 1; zz++)
			{

				if (xx == x - xr - 1 || yy == y - 1 || zz == z - zr - 1 || xx == x + xr + 1 || yy == y + hr + 1 || zz == z + zr + 1)
				{
					if (yy >= 0 && !level->getMaterial(xx, yy - 1, zz)->isSolid())
					{
						level->removeTile(xx, yy, zz);
					}
					else if (level->getMaterial(xx, yy, zz)->isSolid())
					{
						if (random->nextInt(8) % 4 == 0)
						{
							level->setTileAndData(xx, yy, zz, Tile::moonDungeonMossyBricks_Id, 0, Tile::UPDATE_CLIENTS);
						}else if (random->nextInt(4) % 2 != 0)
						{
							level->setTileAndData(xx, yy, zz, Tile::moonDungeonCrackedBricks_Id, 0, Tile::UPDATE_CLIENTS);
						}
						else
						{
							level->setTileAndData(xx, yy, zz, Tile::moonDungeonBricks_Id, 0, Tile::UPDATE_CLIENTS);
						}
					}
				}
				else
				{
					level->removeTile(xx, yy, zz);
				}
			}
		}
	}

	for (int cc = 0; cc < 2; cc++)
	{
		for (int i = 0; i < 3; i++)
		{
			int xc = x + random->nextInt(xr * 2 + 1) - xr;
			int yc = y;
			int zc = z + random->nextInt(zr * 2 + 1) - zr;
			if (!level->isEmptyTile(xc, yc, zc)) continue;

			int count = 0;
			if (level->getMaterial(xc - 1, yc, zc)->isSolid()) count++;
			if (level->getMaterial(xc + 1, yc, zc)->isSolid()) count++;
			if (level->getMaterial(xc, yc, zc - 1)->isSolid()) count++;
			if (level->getMaterial(xc, yc, zc + 1)->isSolid()) count++;

			if (count != 1) continue;

			level->setTileAndData(xc, yc, zc, Tile::treasureChest_Id, 0, Tile::UPDATE_CLIENTS);
			WeighedTreasureArray wrapperArray(monsterRoomTreasure, TREASURE_ITEMS_COUNT);
			WeighedTreasureArray treasure = WeighedTreasure::addToTreasure(wrapperArray, Item::enchantedBook->createForRandomTreasure(random));
			shared_ptr<ChestTileEntity> chest = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(xc, yc, zc));
			if (chest != nullptr)
			{
				WeighedTreasure::addChestItems(random, treasure, chest, 8);
			}

			break;
		}
	}


	level->setTileAndData(x, y, z, Tile::mobSpawner_Id, 0, Tile::UPDATE_CLIENTS);
	shared_ptr<MobSpawnerTileEntity> entity = dynamic_pointer_cast<MobSpawnerTileEntity>(level->getTileEntity(x, y, z));
	if (entity != nullptr)
	{
		entity->getSpawner()->setEntityId(randomEntityId(random));
	}

	return true;

}

wstring MoonMonsterRoomFeature::randomEntityId(Random* random)
{
	int id = random->nextInt(4);
	if (id == 0) return wstring(L"EvolvedSkeleton");
	if (id == 1) return wstring(L"EvolvedZombie");
	if (id == 2) return wstring(L"EvolvedZombie");
	if (id == 3) return wstring(L"EvolvedSpider");
	return wstring(L"");
}