#pragma once
#include "EntityRenderer.h"

class BuggyModel;

class BuggyRenderer : public EntityRenderer
{
private:
	BuggyModel* buggyModel;
	static ResourceLocation BUGGY_LOCATION;

public:
	BuggyRenderer();
	virtual void render(shared_ptr<Entity> mob, double x, double y, double z, float rot, float a);
	virtual ResourceLocation* getTextureLocation(shared_ptr<Entity> mob);
};