#pragma once
#include "Feature.h"
#include "Material.h"

class CraterFeature : public Feature
{
/*private:
	int tile;
*/
public:
	CraterFeature ();

	virtual bool place(Level *level, Random *random, int x, int y, int z);
};