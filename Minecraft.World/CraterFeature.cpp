#include "stdafx.h"
#include "CraterFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.biome.h"

CraterFeature::CraterFeature()
{
}

bool CraterFeature::place(Level *level, Random *random, int x, int y, int z)
{
	x -= 8;
	z -= 8;
	while (y > 5 && level->isEmptyTile(x, y, z))
		y--;
	if (y <= 4)
	{
		return false;
	}

	y -= 2;

	//bool grid[16*16*8] = {0};

	LevelGenerationOptions *levelGenOptions = NULL;
	if( app.getLevelGenerationOptions() != NULL )
	{
		levelGenOptions = app.getLevelGenerationOptions();

		int minX = x;
		int minY = y;
		int minZ = z;

		int maxX = x + 24;
		int maxY = y + 8;
		int maxZ = z + 24;

		bool intersects = levelGenOptions->checkIntersects(minX, minY, minZ, maxX, maxY, maxZ);
		if(intersects)
		{
			//app.DebugPrintf("Skipping reeds feature generation as it overlaps a game rule structure\n");
			return false;
		}
	}

	int randomX = 20 + random->nextInt(4);
	int randomZ = 20 + random->nextInt(4);

	double rX = randomX/2.0;
	double rY = 4.0;
	double rZ = randomZ/2.0;

	const int offsets[6][3] = {
		{ 1, 0, 0}, {-1, 0, 0},
		{ 0, 1, 0}, { 0,-1, 0},
		{ 0, 0, 1}, { 0, 0,-1}
	};

	for(int xx = 0; xx < randomX; xx++) {
		for(int yy = 0; yy < 8; yy++) {
			for(int zz = 0; zz < randomZ; zz++) {
				double dx = (xx + 0.5 - rX) / rX;
				double dy = (yy + 0.5 - rY) / rY;
				double dz = (zz + 0.5 - rZ) / rZ;

				//(x/r)^2 + (y/r)^2 + (z/r)^2 <= 1
				if (dx*dx + dy*dy + dz*dz <= 1.0) {
					level->setTileAndData(x + xx, y + yy, z + zz, 0, 0, Tile::UPDATE_CLIENTS);

					/*Checks surrounding blocks to see if they are outside crater; if so:
					 - Check to see they are not moon stone. If they are, replace it with moon dirt.
					 - Scatter cheese ore around the edges.
					*/
					for(int i = 0; i < 6; i++) {
						int xOff = xx + offsets[i][0];
						int yOff = yy + offsets[i][1];
						int zOff = zz + offsets[i][2];

						double dxOff = (xOff + 0.5 - rX) / rX;
						double dyOff = (yOff + 0.5 - rY) / rY;
						double dzOff = (zOff + 0.5 - rZ) / rZ;

						if(dxOff*dxOff + dyOff*dyOff + dzOff*dzOff > 1.0) {
							int tile = level->getTile(x + xOff, y + yOff, z + zOff);
							if(tile == Tile::moonStone_Id || tile == Tile::moonQuartzOre_Id) level->setTileAndData(x + xOff, y + yOff, z + zOff, Tile::moonDirt_Id, 0, Tile::UPDATE_CLIENTS);
							if(random->nextInt(20) == 2 && !level->isEmptyTile(x + xOff, y + yOff, z + zOff)) level->setTileAndData(x + xOff, y + yOff, z + zOff, Tile::cheeseOre_Id, 0, Tile::UPDATE_CLIENTS);
						}
					}
				}
			}
		}
	}

	/*int spots = random->nextInt(4) + 4;
	for (int i = 0; i < spots; i++)
	{
		double xr = random->nextDouble() * 6 + 3;
		double yr = random->nextDouble() * 4 + 2;
		double zr = random->nextDouble() * 6 + 3;

		double xp = random->nextDouble() * (16 - xr - 2) + 1 + xr / 2;
		double yp = random->nextDouble() * (8 - yr - 4) + 2 + yr / 2;
		double zp = random->nextDouble() * (16 - zr - 2) + 1 + zr / 2;

		for (int xx = 1; xx < 15; xx++)
		{
			for (int zz = 1; zz < 15; zz++)
			{
				for (int yy = 1; yy < 7; yy++)
				{
					double xd = ((xx - xp) / (xr / 2));
					double yd = ((yy - yp) / (yr / 2));
					double zd = ((zz - zp) / (zr / 2));
					double d = xd * xd + yd * yd + zd * zd;
					if (d < 1) grid[((xx) * 16 + (zz)) * 8 + (yy)] = true;
				}
			}
		}
	}

	for (int xx = 0; xx < 16; xx++)
	{
		for (int zz = 0; zz < 16; zz++)
		{
			for (int yy = 0; yy < 8; yy++)
			{
				bool check = !grid[((xx) * 16 + (zz)) * 8 + (yy)] && ((xx < 15 && grid[((xx + 1) * 16 + (zz)) * 8 + (yy)])//
					|| (xx > 0 && grid[((xx - 1) * 16 + (zz)) * 8 + (yy)])
					|| (zz < 15 && grid[((xx) * 16 + (zz + 1)) * 8 + (yy)])
					|| (zz > 0 && grid[((xx) * 16 + (zz - 1)) * 8 + (yy)])
					|| (yy < 7 && grid[((xx) * 16 + (zz)) * 8 + (yy + 1)])
					|| (yy > 0 && grid[((xx) * 16 + (zz)) * 8 + (yy - 1)]));

				if (check)
				{
					Material *m = level->getMaterial(x + xx, y + yy, z + zz);
					if (yy >= 4 && m->isLiquid()) return false;
					if (yy < 4 && !m->isSolid()) return false;
					//if (yy < 4 && (!m->isSolid() && level->getTile(x + xx, y + yy, z + zz) != tile)) return false;
				}
			}
		}
	}

	for (int xx = 0; xx < 16; xx++)
	{
		for (int zz = 0; zz < 16; zz++)
		{
			for (int yy = 0; yy < 8; yy++)
			{
				if (grid[((xx) * 16 + (zz)) * 8 + (yy)])
				{
					level->setTileAndData(x + xx, y + yy, z + zz, 0, 0, Tile::UPDATE_CLIENTS);
					//level->setTileAndData(x + xx, y + yy, z + zz, yy >= 4 ? 0 : tile, 0, Tile::UPDATE_CLIENTS);
				}
			}
		}
	}

	for (int xx = 0; xx < 16; xx++)
	{
		for (int zz = 0; zz < 16; zz++)
		{
			for (int yy = 4; yy < 8; yy++)
			{
				if (grid[((xx) * 16 + (zz)) * 8 + (yy)])
				{
					if (level->getTile(x + xx, y + yy - 1, z + zz) == Tile::moonDirt_Id && level->getBrightness(LightLayer::Sky, x + xx, y + yy, z + zz) > 0)
					{
						Biome *b = level->getBiome(x + xx, z + zz);
						level->setTileAndData(x + xx, y + yy - 1, z + zz, Tile::moonTurf_Id, 0, Tile::UPDATE_CLIENTS);
					}
				}
			}
		}
	}*/

	return true;
}