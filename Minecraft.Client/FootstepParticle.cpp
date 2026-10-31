#include "stdafx.h"
#include "FootstepParticle.h"
#include "Textures.h"
#include "Tesselator.h"
#include "..\Minecraft.World\Mth.h"
#include "..\Minecraft.World\net.minecraft.world.level.h"
#include "ResourceLocation.h"
#include "..\Minecraft.World\LevelData.h"

ResourceLocation FootstepParticle::FOOTPRINT_LOCATION = ResourceLocation(TN_MISC_FOOTSTEP);

FootstepParticle::FootstepParticle(Textures *textures, Level *level, double x, double y, double z) : Particle(level, x, y, z, 0, 0, 0)
{
	// 4J added initialisers
	life = 0;
	lifeTime = 0;

	this->textures = textures;
	xd = yd = zd = 0;
	lifeTime = 3000;

}

void FootstepParticle::render(Tesselator *t, float a, float xa, float ya, float za, float xa2, float za2)
{
    float time = ((life + a) / lifeTime);

    float alpha = 2 - time * 2;
    if (alpha > 1) alpha = 1;
    alpha = alpha * 0.2f;

    float r = 2 / 16.0f;

    float xx = (float)(x - xOff);
    float yy = (float)(y - yOff) + 0.01f;
    float zz = (float)(z - zOff);

	int bx = Mth::floor(x);
    int by = Mth::floor(y);
    int bz = Mth::floor(z);

	glPushMatrix();

    textures->bindTexture(&FOOTPRINT_LOCATION);

	glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	 if (SharedConstants::TEXTURE_LIGHTING)
    {
        int col = this->getLightColor(a);
        int u = col % 65536;
        int v = col / 65536;
        glMultiTexCoord2f(GL_TEXTURE1, u / 1.0f, v / 1.0f);
        glColor4f(1, 1, 1, 0.5f - time);
    }
    else
    {
        float br = this->getBrightness(a);
        glColor4f(br, br, br, 0.5f - time);
    }

    float y1 = yy;
    float y2 = yy + 0.001f;

    t->begin();
    t->vertexUV(xx - r, yy, zz + r, 0, 1);

    t->vertexUV(xx + r, yy, zz + r, 1, 1);

    t->vertexUV(xx + r, yy, zz - r, 1, 0);

    t->vertexUV(xx - r, yy, zz - r, 0, 0);

    t->end();

    glPopMatrix();

}

void FootstepParticle::tick()
{
    life++;
	 if (this->level->getTile(Mth::floor(x), Mth::floor(y) - 1, Mth::floor(z)) == 0) {
        life = lifeTime;
    }
    if (life == lifeTime) remove();
}

int FootstepParticle::getParticleTexture()
{
	return ParticleEngine::ENTITY_PARTICLE_TEXTURE;
}