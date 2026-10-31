#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "OxygenSetupModel.h"
#include "ModelPart.h"

OxygenSetupModel::OxygenSetupModel() : Model()
{
	setup = (new ModelPart(this))->setTexSize(32,32);
	setup->setPos(0.0F, 2.0F, 3.0F);
	setup->texOffs(0,9)->addBox(-3.0F, -2.0F, -1.0F, 6.0F, 3.0F, 2.0F);
	setup->texOffs(16,14)->addBox(3.0F, -1.0F, -1.0F, 1.0F, 4.0F, 2.0F);
	setup->texOffs(0,19)->addBox(-4.0F, -1.0F, -1.0F, 1.0F, 4.0F, 2.0F);

	tank = (new ModelPart(this))->setTexSize(32,32);
	tank->setPos(0.0F, 8.0F, 3.0F);
	tank->texOffs(0,0)->addBox(-3.0F, -4.0F, -1.0F, 6.0F, 5.0F, 4.0F);
	tank->texOffs(0,14)->addBox(-2.0F, -5.0F, -1.0F, 4.0F, 1.0F, 4.0F);
	tank->texOffs(16,9)->addBox(-2.0F, 1.0F, -1.0F, 4.0F, 1.0F, 4.0F);
}

void OxygenSetupModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
	//setupAnim(time, r, bob, yRot, xRot, scale, entity);

	setup->render(scale, usecompiled);
	tank->render(scale, usecompiled);
}

void OxygenSetupModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
}
