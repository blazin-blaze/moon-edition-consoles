#include "stdafx.h"
#include "MoonDimension.h"
#include "FixedBiomeSource.h"
#include "net.minecraft.world.level.levelgen.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.storage.h"
#include "net.minecraft.world.level.tile.h"
#include "..\Minecraft.Client\Minecraft.h"
#include "..\Minecraft.Client\Common\Colours\ColourTable.h"

void MoonDimension::init()
{
	biomeSource = new FixedBiomeSource(Biome::moon, 0.5f, 0);
	id = 2;
	hasCeiling = false;
}

ChunkSource* MoonDimension::createRandomLevelSource() const
{
	return new MoonLevelRandomLevelSource(level, level->getSeed(), true);
}

Vec3* MoonDimension::getFogColor(float td, float a) const
{
	int fogColor = Minecraft::GetInstance()->getColourTable()->getColor(eMinecraftColour_End_Fog_Colour); //0xa080a0;
	float br = Mth::cos(td * PI * 2) * 2 + 0.5f;
	if (br < 0.0f) br = 0.0f;
	if (br > 1.0f) br = 1.0f;

	float r = ((fogColor >> 16) & 0xff) / 255.0f;
	float g = ((fogColor >> 8) & 0xff) / 255.0f;
	float b = ((fogColor) & 0xff) / 255.0f;
	r *= br * 0.0f + 0.15f;
	g *= br * 0.0f + 0.15f;
	b *= br * 0.0f + 0.15f;

	return Vec3::newTemp(r, g, b);
}

bool MoonDimension::hasGround()
{
	return true;
}

bool MoonDimension::mayRespawn() const
{
	return false;
}

bool MoonDimension::isNaturalDimension()
{
	return true;
}

float MoonDimension::getCloudHeight()
{
	return -1;
}

bool MoonDimension::isValidSpawn(int x, int z) const
{
	int topTile = level->getTopTile(x, z);

	if (topTile == 0) return false;

	return Tile::tiles[topTile]->material->blocksMotion();
}

bool MoonDimension::isFoggyAt(int x, int z)
{
	return false;
}

int MoonDimension::getXZSize()
{
	return ceil((float)level->getLevelData()->getXZSize() / level->getLevelData()->getHellScale());
}
