#include "stdafx.h"
#include "net.minecraft.world.level.levelgen.structure.h"
#include "JavaMath.h"
#include "Mth.h"

const wstring MoonMineShaftFeature::OPTION_CHANCE = L"chance";

MoonMineShaftFeature::MoonMineShaftFeature()
{
	chance = 0.01;
}

wstring MoonMineShaftFeature::getFeatureName()
{
	return L"MoonMineshaft";
}

MoonMineShaftFeature::MoonMineShaftFeature(unordered_map<wstring, wstring> options)
{
	chance = 0.01;

	for (unordered_map<wstring, wstring>::iterator it = options.begin(); it != options.end(); ++it)
	{
		if (it->first.compare(OPTION_CHANCE) == 0)
		{
			chance = Mth::getDouble(it->second, chance);
		}
	}
}

bool MoonMineShaftFeature::isFeatureChunk(int x, int z, bool bIsSuperflat)
{
	bool forcePlacement = false;
	LevelGenerationOptions* levelGenOptions = app.getLevelGenerationOptions();
	if (levelGenOptions != nullptr)
	{
		forcePlacement = levelGenOptions->isFeatureChunk(x, z, eFeature_MoonMineshaft);
	}

	return forcePlacement || (random->nextDouble() < chance && random->nextInt(80) < max(abs(x), abs(z)));
}

StructureStart* MoonMineShaftFeature::createStructureStart(int x, int z)
{
	app.AddTerrainFeaturePosition(eTerrainFeature_MoonMineshaft, x, z);

	return new MoonMineShaftStart(level, random, x, z);
}