#pragma once
#include "Particle.h"

class FierySmokeParticle : public Particle
{
public:
    virtual eINSTANCEOF GetType() { return eType_FIERYSMOKEPARTICLE; }
private:
    void init(Level* level, double x, double y, double z, double xa, double ya, double za, float scale);	// 4J - added
public:
    FierySmokeParticle(Level* level, double x, double y, double z, double xa, double ya, double za);
    float oSize;

    FierySmokeParticle(Level* level, double x, double y, double z, double xa, double ya, double za, float scale);
    virtual void render(Tesselator* t, float a, float xa, float ya, float za, float xa2, float za2);
    virtual void tick();
    virtual int getLightColor(float a);	// 4J - brought forward from 1.8.2
    virtual float getBrightness(float a);
};