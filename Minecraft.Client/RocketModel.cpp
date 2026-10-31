#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "RocketModel.h"
#include "ModelPart.h"

RocketModel::RocketModel() : Model()
{
	bb_main = (new ModelPart(this))->setTexSize(64, 64);
	bb_main->setPos(0.0F, 0.0F, 0.0F);
	bb_main->texOffs(25, 25)->addBox(-1.0F, -4.0F, -7.0F, 2.0F, 4.0F, 1.0F);
	bb_main->texOffs(40, 20)->addBox(-1.0F, -6.0F, -6.0F, 2.0F, 3.0F, 1.0F);
	bb_main->texOffs(0, 51)->addBox(-1.0F, -7.0F, -5.0F, 2.0F, 2.0F, 4.0F);
	bb_main->texOffs(56, 54)->addBox(-1.0F, -4.0F, 6.0F, 2.0F, 4.0F, 1.0F);
	bb_main->texOffs(46, 20)->addBox(-1.0F, -6.0F, 5.0F, 2.0F, 3.0F, 1.0F);
	bb_main->texOffs(12, 51)->addBox(-1.0F, -7.0F, 1.0F, 2.0F, 2.0F, 4.0F);
	bb_main->texOffs(48, 24)->addBox(-3.0F, -24.0F, -5.0F, 6.0F, 6.0F, 2.0F);

	cube_r1 = (new ModelPart(this))->setTexSize(64, 64);
	cube_r1->setPos(4.0F, -32.0F, 0.0F);
	cube_r1->xRot = 0.0F;
	cube_r1->yRot = -1.5708F;
	cube_r1->zRot = 0.0F;
	cube_r1->texOffs(48, 54)->addBox(-1.0F, -5.0F, 3.0F, 2.0F, 5.0F, 2.0F);
	cube_r1->texOffs(44, 44)->addBox(-2.0F, 0.0F, 2.0F, 4.0F, 1.0F, 4.0F);
	cube_r1->texOffs(0, 44)->addBox(-3.0F, 1.0F, 1.0F, 6.0F, 1.0F, 6.0F);
	cube_r1->texOffs(24, 24)->addBox(-4.0F, 2.0F, 0.0F, 8.0F, 1.0F, 8.0F);
	cube_r1->texOffs(0, 12)->addBox(-5.0F, 23.0F, -1.0F, 10.0F, 2.0F, 10.0F);
	cube_r1->texOffs(0, 0)->addBox(-5.0F, 3.0F, -1.0F, 10.0F, 2.0F, 10.0F);
	cube_r1->texOffs(40, 0)->addBox(-5.0F, 5.0F, -1.0F, 8.0F, 18.0F, 2.0F);
	cube_r1->texOffs(24, 53)->addBox(-1.0F, 25.0F, -1.0F, 2.0F, 2.0F, 4.0F);

	cube_r2 = (new ModelPart(this))->setTexSize(64, 64);
	cube_r2->setPos(0.0F, -16.0F, -4.0F);
	cube_r2->xRot = 0.0F;
	cube_r2->yRot = 3.1416F;
	cube_r2->zRot = 0.0F;
	cube_r2->texOffs(44, 49)->addBox(-3.0F, -11.0F, -1.0F, 6.0F, 3.0F, 2.0F);
	cube_r2->texOffs(44, 33)->addBox(-3.0F, -2.0F, -1.0F, 6.0F, 9.0F, 2.0F);

	cube_r3 = (new ModelPart(this))->setTexSize(64, 64);
	cube_r3->setPos(-4.0F, -7.0F, 2.0F);
	cube_r3->xRot = 0.0F;
	cube_r3->yRot = -1.5708F;
	cube_r3->zRot = 0.0F;
	cube_r3->texOffs(0, 24)->addBox(-7.0F, -20.0F, -1.0F, 10.0F, 18.0F, 2.0F);

	cube_r4 = (new ModelPart(this))->setTexSize(64, 64);
	cube_r4->setPos(0.0F, -7.0F, 4.0F);
	cube_r4->xRot = 0.0F;
	cube_r4->yRot = 3.1416F;
	cube_r4->zRot = 0.0F;
	cube_r4->texOffs(24, 33)->addBox(-5.0F, -20.0F, -1.0F, 8.0F, 18.0F, 2.0F);

	cube_r5 = (new ModelPart(this))->setTexSize(64, 64);
	cube_r5->setPos(-5.0F, -5.0F, 0.0F);
	cube_r5->xRot = 0.0F;
	cube_r5->yRot = -1.5708F;
	cube_r5->zRot = 0.0F;
	cube_r5->texOffs(36, 54)->addBox(-1.0F, -2.0F, -4.0F, 2.0F, 2.0F, 4.0F);

	cube_r6 = (new ModelPart(this))->setTexSize(64, 64);
	cube_r6->setPos(-6.0F, -3.0F, 0.0F);
	cube_r6->xRot = 0.0F;
	cube_r6->yRot = -1.5708F;
	cube_r6->zRot = 0.0F;
	cube_r6->texOffs(18, 57)->addBox(-1.0F, -3.0F, -1.0F, 2.0F, 3.0F, 1.0F);

	cube_r7 = (new ModelPart(this))->setTexSize(64, 64);
	cube_r7->setPos(6.0F, 0.0F, 0.0F);
	cube_r7->xRot = 0.0F;
	cube_r7->yRot = -1.5708F;
	cube_r7->zRot = 0.0F;
	cube_r7->texOffs(6, 57)->addBox(-1.0F, -4.0F, -1.0F, 2.0F, 4.0F, 1.0F);

	cube_r8 = (new ModelPart(this))->setTexSize(64, 64);
	cube_r8->setPos(5.0F, -3.0F, 0.0F);
	cube_r8->xRot = 0.0F;
	cube_r8->yRot = -1.5708F;
	cube_r8->zRot = 0.0F;
	cube_r8->texOffs(12, 57)->addBox(-1.0F, -3.0F, -1.0F, 2.0F, 3.0F, 1.0F);

	cube_r9 = (new ModelPart(this))->setTexSize(64, 64);
	cube_r9->setPos(-7.0F, 0.0F, 0.0F);
	cube_r9->xRot = 0.0F;
	cube_r9->yRot = -1.5708F;
	cube_r9->zRot = 0.0F;
	cube_r9->texOffs(0, 57)->addBox(-1.0F, -4.0F, -1.0F, 2.0F, 4.0F, 1.0F);
}

void RocketModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    //setupAnim(time, r, bob, yRot, xRot, scale, entity);

	bb_main->render(scale, usecompiled);
	cube_r1->render(scale, usecompiled);
	cube_r2->render(scale, usecompiled);
	cube_r3->render(scale, usecompiled);
	cube_r4->render(scale, usecompiled);
	cube_r5->render(scale, usecompiled);
	cube_r6->render(scale, usecompiled);
	cube_r7->render(scale, usecompiled);
	cube_r8->render(scale, usecompiled);
	cube_r9->render(scale, usecompiled);
}

void RocketModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
}

