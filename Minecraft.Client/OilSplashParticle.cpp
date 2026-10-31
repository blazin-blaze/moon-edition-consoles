#include "stdafx.h"
#include "OilSplashParticle.h"

OilSplashParticle::OilSplashParticle(Level* level, double x, double y, double z, double xa, double ya, double za) : OilDropParticle(level, x, y, z)
{
    gravity = 0.04f;
    setNextMiscAnimTex();
    if (ya == 0 && (xa != 0 || za != 0))
    {
        this->xd = xa;
        this->yd = ya + 0.1;
        this->zd = za;
    }
}