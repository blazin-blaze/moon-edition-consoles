#include "stdafx.h"
#include "..\Minecraft.World\net.minecraft.world.entity.npc.h"
#include "AlienVillagerModel.h"
#include "AlienVillagerRenderer.h"

ResourceLocation AlienVillagerRenderer::ALIEN_VILLAGER_LOCATION = ResourceLocation(TN_MOB_ALIEN_VILLAGER);
ResourceLocation AlienVillagerRenderer::ALIEN_VILLAGER_SMITH_LOCATION = ResourceLocation(TN_MOB_ALIEN_VILLAGER_SMITH);
ResourceLocation AlienVillagerRenderer::ALIEN_VILLAGER_TINKERER_LOCATION = ResourceLocation(TN_MOB_ALIEN_VILLAGER_TINKERER);

AlienVillagerRenderer::AlienVillagerRenderer() : MobRenderer(new AlienVillagerModel(0), 0.5f)
{
	villagerModel = static_cast<AlienVillagerModel*>(model);
}

int AlienVillagerRenderer::prepareArmor(shared_ptr<LivingEntity> villager, int layer, float a)
{
	return -1;
}

void AlienVillagerRenderer::render(shared_ptr<Entity> mob, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(mob, x, y, z, rot, a);
}

ResourceLocation* AlienVillagerRenderer::getTextureLocation(shared_ptr<Entity> _mob)
{
	shared_ptr<AlienVillager> mob = dynamic_pointer_cast<AlienVillager>(_mob);

	switch (mob->getProfession())
	{
	case AlienVillager::PROFESSION_BUTCHER:
		return &ALIEN_VILLAGER_LOCATION;
	case AlienVillager::PROFESSION_SMITH:
		return &ALIEN_VILLAGER_SMITH_LOCATION;
	case AlienVillager::PROFESSION_TINKERER:
		return &ALIEN_VILLAGER_TINKERER_LOCATION;
	default:
		return &ALIEN_VILLAGER_LOCATION;
	}
}

void AlienVillagerRenderer::additionalRendering(shared_ptr<LivingEntity> mob, float a)
{
	MobRenderer::additionalRendering(mob, a);
}

void AlienVillagerRenderer::scale(shared_ptr<LivingEntity> _mob, float a)
{
	// 4J - original version used generics and thus had an input parameter of type Blaze rather than shared_ptr<Entity>  we have here - 
	// do some casting around instead
	float s = 15 / 16.0f;
	shadowRadius = 0.5f;
	glScalef(s, s, s);
}