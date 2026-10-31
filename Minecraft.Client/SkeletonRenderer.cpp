#include "stdafx.h"
#include "SkeletonRenderer.h"
#include "SkeletonModel.h"
#include "../Minecraft.World/Skeleton.h"
#include "ModelPart.h"
#include "OxygenSetupModel.h"

ResourceLocation SkeletonRenderer::SKELETON_LOCATION = ResourceLocation(TN_MOB_SKELETON);
ResourceLocation SkeletonRenderer::EVOLVED_SKELETON_LOCATION = ResourceLocation(TN_MOB_EVOLVED_SKELETON);
ResourceLocation SkeletonRenderer::WITHER_SKELETON_LOCATION = ResourceLocation(TN_MOB_WITHER_SKELETON);
ResourceLocation SkeletonRenderer::OXYGEN_SETUP_LOCATION = ResourceLocation(TN_MOB_OXYGEN_SETUP);

SkeletonRenderer::SkeletonRenderer() : HumanoidMobRenderer(new SkeletonModel(), .5f)
{
	oxygenSetup = NULL;
}

void SkeletonRenderer::scale(shared_ptr<LivingEntity> mob, float a)
{
    if (mob->instanceof(eTYPE_EVOLVEDSKELETON)) {
        glScalef(1.2f, 1.2f, 1.2f);
    }

    shared_ptr<Skeleton> skeleton = dynamic_pointer_cast<Skeleton>(mob);
    if (skeleton != NULL && skeleton->getSkeletonType() == Skeleton::TYPE_WITHER)
	{
        glScalef(1.2f, 1.2f, 1.2f);
    }
}

void SkeletonRenderer::translateWeaponItem()
{
    glTranslatef(1.5f / 16.0f, 3 / 16.0f, 0);
}

ResourceLocation *SkeletonRenderer::getTextureLocation(shared_ptr<Entity> entity)
{
	if (entity->instanceof(eTYPE_EVOLVEDSKELETON)) {
        return &EVOLVED_SKELETON_LOCATION;
    }

	shared_ptr<Skeleton> skeleton = dynamic_pointer_cast<Skeleton>(entity);

    if (skeleton->getSkeletonType() == Skeleton::TYPE_WITHER)
	{
        return &WITHER_SKELETON_LOCATION;
    }
    return &SKELETON_LOCATION;
}

void SkeletonRenderer::renderSpaceSetup(shared_ptr<LivingEntity> entity, float time, float r, float bob, float yRot, float xRot, float scale, float a) {
	if (entity->dimension == 2 && !(entity->instanceof(eTYPE_EVOLVEDSKELETON))) {
		if (oxygenSetup == NULL) {
			oxygenSetup = new OxygenSetupModel();
		}

		glPushMatrix();

		humanoidModel->body->translateTo(1 / 16.0f);

		bindTexture(&OXYGEN_SETUP_LOCATION);
		float brightness = SharedConstants::TEXTURE_LIGHTING ? 1 : entity->getBrightness(a);
		glColor3f(brightness, brightness, brightness);
		glTranslatef(0.0f, 0.0f, 0.001f);
		if (entity->getArmor(2)) {
			glTranslatef(0.0f, 0.0f, 0.0325f);
		}
		oxygenSetup->render(entity, time, r, bob, yRot, xRot, scale, true);

		glPopMatrix();
	}
}