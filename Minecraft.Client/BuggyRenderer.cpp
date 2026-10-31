#include "stdafx.h"
#include "..\Minecraft.World\net.minecraft.world.level.tile.h"
#include "BuggyModel.h"
#include "ModelPart.h"
#include "TextureAtlas.h"
#include "BuggyRenderer.h"
#include "..\Minecraft.World\Buggy.h"

ResourceLocation BuggyRenderer::BUGGY_LOCATION = ResourceLocation(TN_MOB_BUGGY);

BuggyRenderer::BuggyRenderer() : EntityRenderer()
{
	this->shadowRadius = 0.0f;
	buggyModel = new BuggyModel();
}

void BuggyRenderer::render(shared_ptr<Entity> mob, double x, double y, double z, float rot, float a)
{
    shared_ptr<Buggy> buggy = dynamic_pointer_cast<Buggy>(mob);

	glPushMatrix();

	glTranslatef(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	
	glRotatef(180 - rot, 0, 1, 0);

	/*float ss = 12 / 16.0f;
	glScalef(ss, ss, ss);
	glScalef(1 / ss, 1 / ss, 1 / ss);*/

	glScalef(2.0f, 2.0f, 2.0f);

    bindTexture(buggy);
	glScalef(-1, -1, 1);
	glTranslatef(0, -1.5f, 0);

	buggyModel->satellite->yRot = sinf((buggy->tickCount + a) * 0.1f) * (15.0f * (2 * acos(0.0)) / 180.0f);
	buggyModel->wheel1->xRot = buggy->getWheelRot();
	buggyModel->wheel2->xRot = buggy->getWheelRot();
	buggyModel->wheel3->xRot = buggy->getWheelRot();
	buggyModel->wheel4->xRot = buggy->getWheelRot();

	buggyModel->storage->visible = buggy->getHasStorage();

	buggyModel->render(buggy, 0, 0, 0, 0, 0, 1 / 16.0F, false);
    glPopMatrix();
}

ResourceLocation* BuggyRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
	return &BUGGY_LOCATION;
}