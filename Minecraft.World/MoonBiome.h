#pragma once
#pragma once
#include "Biome.h"

class MoonBiome : public Biome
{
public:
    MoonBiome(int id);
    void decorate(Level* level, Random* random, int xo, int zo);
};