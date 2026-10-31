#pragma once
#include "HumanoidMobRenderer.h"

class OxygenSetupModel;

class SkeletonRenderer : public HumanoidMobRenderer
{
private:
    static ResourceLocation SKELETON_LOCATION;
	static ResourceLocation EVOLVED_SKELETON_LOCATION;
    static ResourceLocation WITHER_SKELETON_LOCATION;
	 static ResourceLocation OXYGEN_SETUP_LOCATION;

    OxygenSetupModel* oxygenSetup;

public:
	SkeletonRenderer();

protected:
    virtual void scale(shared_ptr<LivingEntity> mob, float a);
    void translateWeaponItem();
    virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> entity);
	virtual void renderSpaceSetup(shared_ptr<LivingEntity> entity, float time, float r, float bob, float yRot, float xRot, float scale, float a);
};