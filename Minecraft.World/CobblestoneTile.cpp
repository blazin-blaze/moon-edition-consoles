#include "stdafx.h"
#include "CobblestoneTile.h"
#include "net.minecraft.world.h"

const unsigned int CobblestoneTile::COBBLESTONE_NAMES[COBBLESTONE_NAMES_LENGTH] = { IDS_TILE_STONE_BRICK,
	IDS_TILE_MOON_COBBLESTONE
};

const wstring CobblestoneTile::TEXTURE_NAMES[] = { L"cobblestone", L"moonCobblestone"};

CobblestoneTile::CobblestoneTile(int id) : Tile(id, Material::stone)
{
	icons = nullptr;
}

unsigned int CobblestoneTile::getDescriptionId(int iData)
{
	if (iData < 0 || iData >= COBBLESTONE_NAMES_LENGTH) iData = 0;

	return COBBLESTONE_NAMES[iData];
}

Icon* CobblestoneTile::getTexture(int face, int data)
{
	if (data < 0 || data >= COBBLESTONE_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

int CobblestoneTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

void CobblestoneTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [COBBLESTONE_NAMES_LENGTH];

	for (int i = 0; i < COBBLESTONE_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}