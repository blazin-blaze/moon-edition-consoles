#pragma once
#include "Model.h"
class RocketModel : public Model
{

private:
	ModelPart* bb_main;
	ModelPart* cube_r1;
	ModelPart* cube_r2;
	ModelPart* cube_r3;
	ModelPart* cube_r4;
	ModelPart* cube_r5;
	ModelPart* cube_r6;
	ModelPart* cube_r7;
	ModelPart* cube_r8;
	ModelPart* cube_r9;

public:
	RocketModel();
	virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
	virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim = 0);
};
