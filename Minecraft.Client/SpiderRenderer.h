#pragma once
#include "MobRenderer.h"

class OxygenSetupModel;

class SpiderRenderer : public MobRenderer
{
private:
	static ResourceLocation SPIDER_LOCATION;
	static ResourceLocation SPIDER_EYES_LOCATION;
	static ResourceLocation EVOLVED_SPIDER_LOCATION;
	static ResourceLocation EVOLVED_SPIDER_EYES_LOCATION;
	static ResourceLocation OXYGEN_SETUP_LOCATION;

	OxygenSetupModel* oxygenSetup;

public:
	SpiderRenderer();

protected:
	virtual void scale(shared_ptr<LivingEntity> mob, float a);
	virtual float getFlipDegrees(shared_ptr<LivingEntity> spider);
	virtual int prepareArmor(shared_ptr<LivingEntity> _spider, int layer, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);
	virtual void renderSpaceSetup(shared_ptr<LivingEntity> entity, float time, float r, float bob, float yRot, float xRot, float scale, float a);
};