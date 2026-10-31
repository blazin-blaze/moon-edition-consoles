#pragma once
#include "Particle.h"

class OilBubbleParticle : public Particle
{
public:
	virtual eINSTANCEOF GetType() { return eType_OILBUBBLEPARTICLE; }
	OilBubbleParticle(Level* level, double x, double y, double z, double xa, double ya, double za);
	virtual void tick();
};