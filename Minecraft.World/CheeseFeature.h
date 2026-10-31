#pragma once
#include "Feature.h"
#include "Material.h"

class Level;

class CheeseFeature : public Feature
{
private:
	int tile;
	int radius;

public:
	CheeseFeature(int radius);

	virtual bool place(Level* level, Random* random, int x, int y, int z);
}; 