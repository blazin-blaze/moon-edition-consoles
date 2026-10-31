#pragma once
#include "Particle.h"

class OilDropParticle : public Particle
{
public:
	virtual eINSTANCEOF GetType() { return eType_OILDROPPARTICLE; }
	OilDropParticle(Level* level, double x, double y, double z);
	virtual void tick();
};