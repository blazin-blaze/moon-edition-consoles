#include "stdafx.h"
#include "net.minecraft.world.entity.monster.h"
#include "MoonBiome.h"
#include "CheeseFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "BiomeDecorator.h"

MoonBiome::MoonBiome(int id) : Biome(id)
{
	enemies.clear();
	friendlies.clear();
	friendlies_chicken.clear();	// 4J added
	friendlies_wolf.clear(); 	// 4J added
	waterFriendlies.clear();
	ambientFriendlies.clear();

	topMaterial = static_cast<byte>(Tile::moonTurf_Id);
	material = static_cast<byte>(Tile::moonDirt_Id);

	enemies.push_back(new MobSpawnerData(eTYPE_SPIDER, 10, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_ZOMBIE, 10, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_SKELETON, 10, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_CREEPER, 10, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_ENDERMAN, 1, 1, 4));
}

void MoonBiome::decorate(Level* level, Random* random, int xo, int zo)
{
	decorator->decorateMoon(level, random, xo, zo);

	/*PIXBeginNamedEvent(0, "Adding cheese");
	CheeseFeature* cheese = new CheeseFeature(3);

	int x = xo + random->nextInt(16) + 8;
	int z = zo + random->nextInt(16) + 8;
	cheese->place(level, random, x, level->getTopSolidBlock(x, z), z);

	PIXEndNamedEvent();*/
}
