#pragma once

#include "StructureStart.h"

class MoonMineShaftStart : public StructureStart
{
public:
	static StructureStart* Create() { return new MoonMineShaftStart(); }
	virtual EStructureStart GetType() { return eStructureStart_MoonMineShaftStart; }

public:
	MoonMineShaftStart();
	MoonMineShaftStart(Level* level, Random* random, int chunkX, int chunkZ);
};