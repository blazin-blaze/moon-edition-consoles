#pragma once
#include "MobRenderer.h"

class AlienVillagerModel;

class AlienVillagerRenderer : public MobRenderer
{
private:
	static ResourceLocation ALIEN_VILLAGER_LOCATION;
	static ResourceLocation ALIEN_VILLAGER_SMITH_LOCATION;
	static ResourceLocation ALIEN_VILLAGER_TINKERER_LOCATION;

protected:
	AlienVillagerModel* villagerModel;

public:
	AlienVillagerRenderer();
	virtual void render(shared_ptr<Entity> mob, double x, double y, double z, float rot, float a);
	virtual ResourceLocation* getTextureLocation(shared_ptr<Entity> _mob);

protected:
	virtual int prepareArmor(shared_ptr<LivingEntity> villager, int layer, float a);
	virtual void additionalRendering(shared_ptr<LivingEntity> mob, float a);
	virtual void scale(shared_ptr<LivingEntity> player, float a);
};