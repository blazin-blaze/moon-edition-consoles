#pragma once
#include "Tile.h"

class ChunkRebuildData;

class CobblestoneTile : public Tile
{
	friend class ChunkRebuildData;
public:
	static const int COBBLESTONE_NAMES_LENGTH = 2;

	static const unsigned int COBBLESTONE_NAMES[COBBLESTONE_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];

private:
	Icon** icons;

public:
	CobblestoneTile(int id);
	virtual unsigned int getDescriptionId(int iData = -1);
	virtual Icon* getTexture(int face, int data);
	virtual int getSpawnResourcesAuxValue(int data);

	void registerIcons(IconRegister* iconRegister);
};