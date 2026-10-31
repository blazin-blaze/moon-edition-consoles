#pragma once

#include "StructureFeature.h"

class MoonMineShaftFeature : public StructureFeature
{
public:
	static const wstring OPTION_CHANCE;

private:
	double chance;

public:
	MoonMineShaftFeature();

	wstring getFeatureName();

	MoonMineShaftFeature(unordered_map<wstring, wstring> options);

protected:
	virtual bool isFeatureChunk(int x, int z, bool bIsSuperflat = false);
	virtual StructureStart* createStructureStart(int x, int z);
};