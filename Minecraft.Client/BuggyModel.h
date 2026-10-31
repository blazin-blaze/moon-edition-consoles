#pragma once
#include "Model.h"
class BuggyModel : public Model
{

public:
	ModelPart* bb_main;
	ModelPart* wheel1;
	ModelPart* wheel2;
	ModelPart* wheel3;
	ModelPart* wheel4;
	ModelPart* satellite;
	ModelPart* cube_r1;
	ModelPart* cube_r2;
	ModelPart* arms;
	ModelPart* cube_r3;
	ModelPart* storage;

public:
	BuggyModel();
	virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
	virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim = 0);
};