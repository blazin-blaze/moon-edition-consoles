#pragma once
#include "MobRenderer.h"

class EnderMan;
class EndermanModel;
class OxygenSetupModel;

class EndermanRenderer : public MobRenderer
{
private:
	EndermanModel *model;
	Random random;
	static ResourceLocation ENDERMAN_EYES_LOCATION;
	static ResourceLocation ENDERMAN_LOCATION;
	static ResourceLocation OXYGEN_SETUP_LOCATION;

	OxygenSetupModel* oxygenSetup;

public:
	EndermanRenderer();

	void render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a);
	ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);
	void additionalRendering(shared_ptr<LivingEntity> _mob, float a);
	virtual void renderSpaceSetup(shared_ptr<LivingEntity> entity, float time, float r, float bob, float yRot, float xRot, float scale, float a);

protected:
	int prepareArmor(shared_ptr<LivingEntity> _mob, int layer, float a);
};