#include "stdafx.h"
#include "..\Minecraft.World\JavaMath.h"
#include "FierySmokeParticle.h"

void FierySmokeParticle::init(Level* level, double x, double y, double z, double xa, double ya, double za, float scale)
{
    xd *= 0.1f;
    yd *= 0.1f;
    zd *= 0.1f;
    xd += xa;
    yd += ya;
    zd += za;

    //rCol = gCol = bCol = (float) (Math::random() * 0.3f);

    float br = Math::random();
    unsigned int colour = Minecraft::GetInstance()->getColourTable()->getColor(eMinecraftColour_Particle_Note_04);
    int r = (colour >> 16) & 0xFF;
    int g = (colour >> 8) & 0xFF;
    int b = colour & 0xFF;
    rCol = (r / 255.0f) * br;
    gCol = (g / 255.0f) * br;
    bCol = (b / 255.0f) * br;

    size *= 0.75f;
    size *= scale;
    oSize = size;

    lifetime = static_cast<int>(8 / (Math::random() * 0.8 + 0.2));
    lifetime = static_cast<int>(lifetime * scale);
    noPhysics = false;
}

FierySmokeParticle::FierySmokeParticle(Level* level, double x, double y, double z, double xa, double ya, double za) : Particle(level, x, y, z, 0, 0, 0)
{
    init(level, x, y, z, xa, ya, za, 1);
}

FierySmokeParticle::FierySmokeParticle(Level* level, double x, double y, double z, double xa, double ya, double za, float scale) : Particle(level, x, y, z, 0, 0, 0)
{
    init(level, x, y, z, xa, ya, za, scale);
}

void FierySmokeParticle::render(Tesselator* t, float a, float xa, float ya, float za, float xa2, float za2)
{
    float l = ((age + a) / lifetime) * 32;
    if (l < 0) l = 0;
    if (l > 1) l = 1;

    size = oSize * l;
    Particle::render(t, a, xa, ya, za, xa2, za2);
}

void FierySmokeParticle::tick()
{
    xo = x;
    yo = y;
    zo = z;

    if (age++ >= lifetime) remove();

    setMiscTex(7 - age * 8 / lifetime);

    yd += 0.004;
    move(xd, yd, zd);
    if (y == yo)
    {
        xd *= 1.1;
        zd *= 1.1;
    }
    xd *= 0.96f;
    yd *= 0.96f;
    zd *= 0.96f;

    if (onGround)
    {
        xd *= 0.7f;
        zd *= 0.7f;
    }
}

int FierySmokeParticle::getLightColor(float a)
{
    float l = (age + a) / lifetime;
    if (l < 0) l = 0;
    if (l > 1) l = 1;
    int br = Particle::getLightColor(a);

    int br1 = (br) & 0xff;
    int br2 = (br >> 16) & 0xff;
    br1 += static_cast<int>(l * 15 * 16);
    if (br1 > 15 * 16) br1 = 15 * 16;
    return br1 | br2 << 16;
}

float FierySmokeParticle::getBrightness(float a)
{
    return 1;
}