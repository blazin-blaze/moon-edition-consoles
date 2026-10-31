#include "stdafx.h"
#include "MoonStoneTile.h"
#include "Level.h"

MoonStoneTile::MoonStoneTile(int id) : Tile(id, Material::stone)
{
}

int MoonStoneTile::getResource(int data, Random* random, int playerBonusLevel)
{
	return Tile::cobblestone_Id;
}

int MoonStoneTile::getSpawnResourcesAuxValue(int data)
{
	return 1;
}