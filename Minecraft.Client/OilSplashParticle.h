#pragma once
#include "OilDropParticle.h"

class OilSplashParticle : public OilDropParticle
{
public:
	virtual eINSTANCEOF GetType() { return eType_OILSPLASHPARTICLE; }
	OilSplashParticle(Level* level, double x, double y, double z, double xa, double ya, double za);
};