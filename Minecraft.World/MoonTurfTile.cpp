#include "stdafx.h"
#include "MoonTurfTile.h"
#include "Level.h"

MoonTurfTile::MoonTurfTile(int id) : Tile(id, Material::moonTurf)
{
}

int MoonTurfTile::getResource(int data, Random* random, int playerBonusLevel)
{
	return Tile::moonDirt_Id;
}