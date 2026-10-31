#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "BuggyModel.h"
#include "ModelPart.h"

BuggyModel::BuggyModel() : Model()
{
	satellite = (new ModelPart(this))->setTexSize(128, 128);
	satellite->setPos(-6.0F, 6.5F, 7.0F);
	satellite->texOffs(37, 50)->addBox(-3.5F, -3.5F, -1.0F, 7.0F, 7.0F, 1.0F);
	satellite->texOffs(18, 71)->addBox(2.5F, -2.5F, 0.0F, 1.0F, 5.0F, 1.0F);
	satellite->texOffs(0, 72)->addBox(-2.5F, -3.5F, 0.0F, 5.0F, 1.0F, 1.0F);
	satellite->texOffs(38, 72)->addBox(-2.5F, 2.5F, 0.0F, 5.0F, 1.0F, 1.0F);
	satellite->texOffs(58, 70)->addBox(1.5F, 1.5F, 0.0F, 1.0F, 1.0F, 1.0F);
	satellite->texOffs(60, 60)->addBox(-2.5F, 1.5F, 0.0F, 1.0F, 1.0F, 1.0F);
	satellite->texOffs(60, 52)->addBox(-2.5F, -2.5F, 0.0F, 1.0F, 1.0F, 1.0F);
	satellite->texOffs(61, 44)->addBox(1.5F, -2.5F, 0.0F, 1.0F, 1.0F, 1.0F);
	satellite->texOffs(62, 34)->addBox(-3.5F, -2.5F, 0.0F, 1.0F, 5.0F, 1.0F);

	wheel1 = (new ModelPart(this))->setTexSize(128, 128);
	wheel1->setPos(6.0F, 21.0F, 11.0F);
	wheel1->texOffs(36,38)->addBox(0.0F, -3.0F, -3.0F, 2.0F, 6.0F, 6.0F);
	wheel1->texOffs(9,10)->addBox(-1.0F, -2.0F, -3.0F, 1.0F, 4.0F, 1.0F);
	wheel1->texOffs(12,1)->addBox(-1.0F, -2.0F, 2.0F, 1.0F, 4.0F, 1.0F);
	wheel1->texOffs(12,1)->addBox(2.0F, -2.0F, 2.0F, 1.0F, 4.0F, 1.0F);
	wheel1->texOffs(1,13)->addBox(2.0F, -3.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel1->texOffs(1,13)->addBox(2.0F, 2.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel1->texOffs(1,13)->addBox(-1.0F, 2.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel1->texOffs(1,13)->addBox(-1.0F, -3.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel1->texOffs(9,10)->addBox(2.0F, -2.0F, -3.0F, 1.0F, 4.0F, 1.0F);

	wheel2 = (new ModelPart(this))->setTexSize(128, 128);
	wheel2->setPos(10.0F, 21.0F, -11.0F);
	wheel2->texOffs(20,38)->addBox(-1.0F, -3.0F, -3.0F, 2.0F, 6.0F, 6.0F);
	wheel2->texOffs(9, 10)->addBox(1.0F, -2.0F, -3.0F, 1.0F, 4.0F, 1.0F);
	wheel2->texOffs(12,1)->addBox(1.0F, -2.0F, 2.0F, 1.0F, 4.0F, 1.0F);
	wheel2->texOffs(12,1)->addBox(-2.0F, -2.0F, 2.0F, 1.0F, 4.0F, 1.0F);
	wheel2->texOffs(9, 10)->addBox(-2.0F, -2.0F, -3.0F, 1.0F, 4.0F, 1.0F);
	wheel2->texOffs(1,13)->addBox(-2.0F, -3.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel2->texOffs(1,13)->addBox(1.0F, -3.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel2->texOffs(1,13)->addBox(1.0F, 2.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel2->texOffs(1,13)->addBox(-2.0F, 2.0F, -2.0F, 1.0F, 1.0F, 4.0F);

	wheel3 = (new ModelPart(this))->setTexSize(128, 128);
	wheel3->setPos(-10.0F, 21.0F, -11.0F);
	wheel3->texOffs(0,50)->addBox(-1.0F, -3.0F, -3.0F, 2.0F, 6.0F, 6.0F);
	wheel3->texOffs(9, 10)->addBox(-2.0F, -2.0F, -3.0F, 1.0F, 4.0F, 1.0F);
	wheel3->texOffs(12,1)->addBox(-2.0F, -2.0F, 2.0F, 1.0F, 4.0F, 1.0F);
	wheel3->texOffs(9, 10)->addBox(1.0F, -2.0F, -3.0F, 1.0F, 4.0F, 1.0F);
	wheel3->texOffs(12,1)->addBox(1.0F, -2.0F, 2.0F, 1.0F, 4.0F, 1.0F);
	wheel3->texOffs(1,13)->addBox(1.0F, -3.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel3->texOffs(1,13)->addBox(1.0F, 2.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel3->texOffs(1,13)->addBox(-2.0F, 2.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel3->texOffs(1,13)->addBox(-2.0F, -3.0F, -2.0F, 1.0F, 1.0F, 4.0F);

	wheel4 = (new ModelPart(this))->setTexSize(128, 128);
	wheel4->setPos(-7.0F, 21.0F, 11.0F);
	wheel4->texOffs(16,50)->addBox(-1.0F, -3.0F, -3.0F, 2.0F, 6.0F, 6.0F);
	wheel4->texOffs(9, 10)->addBox(-2.0F, -2.0F, -3.0F, 1.0F, 4.0F, 1.0F);
	wheel4->texOffs(12,1)->addBox(-2.0F, -2.0F, 2.0F, 1.0F, 4.0F, 1.0F);
	wheel4->texOffs(12,1)->addBox(1.0F, -2.0F, 2.0F, 1.0F, 4.0F, 1.0F);
	wheel4->texOffs(9, 10)->addBox(1.0F, -2.0F, -3.0F, 1.0F, 4.0F, 1.0F);
	wheel4->texOffs(1,13)->addBox(-2.0F, -3.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel4->texOffs(1,13)->addBox(-2.0F, 2.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel4->texOffs(1,13)->addBox(1.0F, 2.0F, -2.0F, 1.0F, 1.0F, 4.0F);
	wheel4->texOffs(1,13)->addBox(1.0F, -3.0F, -2.0F, 1.0F, 1.0F, 4.0F);

	bb_main = (new ModelPart(this))->setTexSize(128, 128);
	bb_main->setPos(0.0F, 24.0F, 0.0F);
	bb_main->texOffs(0,0)->addBox(-8.0F, -4.0F, -14.0F, 16.0F, 2.0F, 20.0F);
	bb_main->texOffs(0, 22)->addBox(-5.0F, -4.0F, 7.0F, 10.0F, 2.0F, 7.0F);
	bb_main->texOffs(0, 31)->addBox(-4.0F, -5.0F, -3.0F, 8.0F, 1.0F, 6.0F);
	bb_main->texOffs(34, 22)->addBox(-7.0F, -4.0F, 6.0F, 14.0F, 2.0F, 1.0F);
	bb_main->texOffs(28, 31)->addBox(-4.0F, -5.0F, 7.0F, 8.0F, 1.0F, 6.0F);
	bb_main->texOffs(50, 25)->addBox(-3.0F, -5.0F, 13.0F, 6.0F, 1.0F, 2.0F);
	bb_main->texOffs(32, 50)->addBox(-6.5F, -20.0F, 5.0F, 1.0F, 16.0F, 1.0F);

	cube_r1 = (new ModelPart(this))->setTexSize(128, 128);
	cube_r1->setPos(1.0F, 20.0F, -5.0F);
	cube_r1->xRot = 0.1309F;
	cube_r1->yRot = 0.0F;
	cube_r1->zRot = 0.0F;
	cube_r1->texOffs(0,38)->addBox(-5.0F, -9.0F, 0.0F, 8.0F, 10.0F, 2.0F);

	cube_r2 = (new ModelPart(this))->setTexSize(128, 128);
	cube_r2->setPos(1.0F, 12.0F, -6.0F);
	cube_r2->xRot = 0.0873F;
	cube_r1->yRot = 0.0F;
	cube_r1->zRot = 0.0F;
	cube_r2->texOffs(34, 25)->addBox(-4.0F, -4.0F, 0.0F, 6.0F, 4.0F, 2.0F);

	arms = (new ModelPart(this))->setTexSize(128, 128);
	arms->setPos(4.0F, 19.0F, 3.0F);
	arms->texOffs(1,1)->addBox(-1.0F, -4.0F, -1.0F, 1.0F, 4.0F, 1.0F);
	arms->texOffs(1,1)->addBox(-8.0F, -4.0F, -1.0F, 1.0F, 4.0F, 1.0F);

	cube_r3 = (new ModelPart(this))->setTexSize(128, 128);
	cube_r3->setPos(-2.975F, 15.0F, 2.0F);
	cube_r3->xRot = 1.5708F;
	cube_r3->yRot = 0.0F;
	cube_r3->zRot = 0.0F;
	cube_r3->texOffs(28,71)->addBox(-1.0F, -6.0F, -1.0F, 1.0F, 6.0F, 1.0F);
	cube_r3->texOffs(28,71)->addBox(5.95F, -6.0F, -1.0F, 1.0F, 6.0F, 1.0F);

	storage = (new ModelPart(this))->setTexSize(128, 128);
	storage->setPos(-6.0F, 20.0F, -12.0F);
	storage->texOffs(54, 3)->addBox(-1.0F, -7.0F, -1.0F, 6.0F, 7.0F, 6.0F);
}

void BuggyModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
	//setupAnim(time, r, bob, yRot, xRot, scale, entity);

	bb_main->render(scale, usecompiled);
	wheel1->render(scale, usecompiled);
	wheel2->render(scale, usecompiled);
	wheel3->render(scale, usecompiled);
	wheel4->render(scale, usecompiled);
	satellite->render(scale, usecompiled);
	cube_r1->render(scale, usecompiled);
	cube_r2->render(scale, usecompiled);
	arms->render(scale, usecompiled);
	cube_r3->render(scale, usecompiled);
	storage->render(scale, usecompiled);
}

void BuggyModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
}
