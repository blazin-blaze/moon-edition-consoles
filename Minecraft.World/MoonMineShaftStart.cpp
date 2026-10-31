#include "stdafx.h"
#include "net.minecraft.world.level.levelgen.structure.h"

MoonMineShaftStart::MoonMineShaftStart()
{
	// for reflection
}

MoonMineShaftStart::MoonMineShaftStart(Level* level, Random* random, int chunkX, int chunkZ) : StructureStart(chunkX, chunkZ)
{
	MoonMineShaftPieces::MoonMineShaftRoom* mineShaftRoom = new MoonMineShaftPieces::MoonMineShaftRoom(0, random, (chunkX << 4) + 2, (chunkZ << 4) + 2);
	pieces.push_back(mineShaftRoom);
	mineShaftRoom->addChildren(mineShaftRoom, &pieces, random);

	calculateBoundingBox();
	moveBelowSeaLevel(level, random, 10);
}