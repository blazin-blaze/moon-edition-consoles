#pragma once
#include "Tile.h"
#include "Material.h"

class Random;

class BookshelfTile : public Tile
{
public:
    BookshelfTile(int id);
	 BookshelfTile(int id, Material* mat);

    virtual Icon *getTexture(int face, int data);
    virtual int getResourceCount(Random *random);
	virtual int getResource(int data, Random *random,int playerBonusLevel);
};