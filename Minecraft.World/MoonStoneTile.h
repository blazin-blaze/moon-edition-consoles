#pragma once
#include "Tile.h"

class Random;

class MoonStoneTile : public Tile
{
public:
	MoonStoneTile(int id);
	virtual int getResource(int data, Random* random, int playerBonusLevel);
protected:
	virtual int getSpawnResourcesAuxValue(int data);
};