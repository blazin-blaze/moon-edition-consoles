#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "FrequencyModuleModel.h"
#include "ModelPart.h"

FrequencyModuleModel::FrequencyModuleModel() : Model()
{
	base = (new ModelPart(this))->setTexSize(16,16);
	base->setPos(-5.0F, -6.0F, 2.0F);
	base->texOffs(10,2)->addBox(-1.0F, -2.0F, -2.0F, 2.0F, 1.0F, 1.0F);
	base->texOffs(10,0)->addBox(-1.0F, -2.0F, -6.0F, 2.0F, 1.0F, 1.0F);
	base->texOffs(0,0)->addBox(-1.0F, -2.0F, -5.0F, 2.0F, 4.0F, 3.0F);

	antenna = (new ModelPart(this))->setTexSize(16,16);
	antenna->setPos(-4.0F, -10.0F, -2.0F);
	antenna->texOffs(10,4)->addBox(-1.5F, 0.0F, 0.0F, 1.0F, 2.0F, 1.0F);
	antenna->texOffs(0,7)->addBox(-2.0F, -2.0F, -1.0F, 2.0F, 2.0F, 3.0F);
}

void FrequencyModuleModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
	//setupAnim(time, r, bob, yRot, xRot, scale, entity);

	base->render(scale, usecompiled);
	antenna->render(scale, usecompiled);
}

void FrequencyModuleModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
}
