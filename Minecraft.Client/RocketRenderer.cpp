#include "stdafx.h"
#include "..\Minecraft.World\net.minecraft.world.level.tile.h"
#include "RocketModel.h"
#include "ModelPart.h"
#include "TextureAtlas.h"
#include "RocketRenderer.h"
#include "..\Minecraft.World\Rocket.h"

ResourceLocation RocketRenderer::ROCKET_LOCATION = ResourceLocation(TN_ITEM_ROCKET);
ResourceLocation RocketRenderer::ROCKET_FUEL_LOCATION = ResourceLocation(TN_ITEM_ROCKET_FUEL);
ResourceLocation RocketRenderer::ROCKET_CAKE_LOCATION = ResourceLocation(TN_ITEM_ROCKET_CAKE);
ResourceLocation RocketRenderer::ROCKET_CAKE_FUEL_LOCATION = ResourceLocation(TN_ITEM_ROCKET_CAKE_FUEL);

RocketRenderer::RocketRenderer() : EntityRenderer()
{
	this->shadowRadius = 0.5f;
	rocketModel = new RocketModel();
}

void RocketRenderer::render(shared_ptr<Entity> mob, double x, double y, double z, float rot, float a)
{
    shared_ptr<Rocket> rocket = dynamic_pointer_cast<Rocket>(mob);

	glPushMatrix();
	glTranslatef(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));

	glRotatef(180 - rot, 0, 1, 0);

	float ss = 12 / 16.0f;
	glScalef(ss, ss, ss);
	glScalef(1 / ss, 1 / ss, 1 / ss);

    bindTexture(rocket);
	glScalef(-1, -1, 1);
	glScalef(1.5f, 1.5f, 1.5f);
	rocketModel->render(rocket, 0, 0, 0, 0, 0, 1 / 16.0F, false);
    glPopMatrix();
}

ResourceLocation* RocketRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
	shared_ptr<Rocket> rocket = dynamic_pointer_cast<Rocket>(mob);
	if (rocket != NULL && rocket->getCake()) {
		if (rocket->getFueled()) return &ROCKET_CAKE_FUEL_LOCATION;
		return &ROCKET_CAKE_LOCATION;
	}
	else if (rocket != NULL && rocket->getFueled()) {
		return &ROCKET_FUEL_LOCATION;
	}
	else {
		return &ROCKET_LOCATION;
	}
}