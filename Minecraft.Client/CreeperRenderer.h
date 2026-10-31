#pragma once
#include "MobRenderer.h"

class OxygenSetupModel;

class CreeperRenderer: public MobRenderer
{
private:
	static ResourceLocation POWER_LOCATION;
	static ResourceLocation CREEPER_LOCATION;
	static ResourceLocation OXYGEN_SETUP_LOCATION;
	Model *armorModel;	
	OxygenSetupModel* oxygenSetup;

public:
	CreeperRenderer();

protected:
	virtual void scale(shared_ptr<LivingEntity> _mob, float a);
    virtual int getOverlayColor(shared_ptr<LivingEntity> mob, float br, float a);
    virtual int prepareArmor(shared_ptr<LivingEntity> mob, int layer, float a);
    virtual int prepareArmorOverlay(shared_ptr<LivingEntity> _mob, int layer, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);
	virtual void renderSpaceSetup(shared_ptr<LivingEntity> entity, float time, float r, float bob, float yRot, float xRot, float scale, float a);
	virtual void additionalRendering(shared_ptr<LivingEntity> _mob, float a);
};