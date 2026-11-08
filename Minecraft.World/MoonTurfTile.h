#pragma once
#include "Tile.h"

class Random;

class MoonTurfTile : public Tile
{
public:
	MoonTurfTile(int id);
	virtual int getResource(int data, Random* random, int playerBonusLevel);
};