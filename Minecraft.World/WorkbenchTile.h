#pragma once

#include "Tile.h"

class Player;
class ChunkRebuildData;

class WorkbenchTile : public Tile
{
	friend class Tile;
	friend class ChunkRebuildData;
private:
	Icon *iconTop;
	Icon *iconFront;
	Icon* iconBottom;

protected:
	WorkbenchTile(int id);
	WorkbenchTile(int id, Material* mat);

public:
	Icon *getTexture(int face, int data);
	void registerIcons(IconRegister *iconRegister);

public:
	virtual bool TestUse();
	bool use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly = false); // 4J added soundOnly param
};