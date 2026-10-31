#include "stdafx.h"
#include "SpiderRenderer.h"
#include "SpiderModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.monster.h"
#include "OxygenSetupModel.h"
#include "..\Minecraft.World\Level.h"
#include "ModelPart.h"
#include "..\Minecraft.World\net.minecraft.world.level.tile.h"
#include "EntityRenderDispatcher.h"
#include "..\Minecraft.World\net.minecraft.world.item.h"

ResourceLocation SpiderRenderer::SPIDER_LOCATION = ResourceLocation(TN_MOB_SPIDER);
ResourceLocation SpiderRenderer::EVOLVED_SPIDER_LOCATION = ResourceLocation(TN_MOB_EVOLVED_SPIDER);
ResourceLocation SpiderRenderer::SPIDER_EYES_LOCATION = ResourceLocation(TN_MOB_SPIDER_EYES);
ResourceLocation SpiderRenderer::EVOLVED_SPIDER_EYES_LOCATION = ResourceLocation(TN_MOB_EVOLVED_SPIDER_EYES);
ResourceLocation SpiderRenderer::OXYGEN_SETUP_LOCATION = ResourceLocation(TN_MOB_OXYGEN_SETUP);

SpiderRenderer::SpiderRenderer() : MobRenderer(new SpiderModel(), 1.0f)
{
	this->setArmor(new SpiderModel());

	oxygenSetup = NULL;
}

float SpiderRenderer::getFlipDegrees(shared_ptr<LivingEntity> spider)
{
	return 180;
}

int SpiderRenderer::prepareArmor(shared_ptr<LivingEntity> _spider, int layer, float a)
{
	// 4J - dynamic cast required because we aren't using templates/generics in our version
	shared_ptr<Spider> spider = dynamic_pointer_cast<Spider>(_spider);

	if (layer!=0) return -1;
	MemSect(31);
	if (spider->instanceof(eTYPE_EVOLVEDSPIDER)) {
		bindTexture(&EVOLVED_SPIDER_EYES_LOCATION);
	}
	else {
		bindTexture(&SPIDER_EYES_LOCATION);
	}
	MemSect(0);
	// 4J - changes brought forward from 1.8.2
	float br = 1.0f; // was (1-spider->getBrightness(1))*0.5f;
	glEnable(GL_BLEND);
	// 4J Stu - We probably don't need to do this on 360 either (as we force it back on the renderer)
	// However we do want it off for other platforms that don't force it on in the render lib CBuff handling
	// Several texture packs have fully transparent bits that break if this is off
#ifdef _XBOX
	glDisable(GL_ALPHA_TEST);
#endif
	// 4J - changes brought forward from 1.8.2
	glBlendFunc(GL_ONE, GL_ONE);
	if (spider->isInvisible())	glDepthMask(false);
	else						glDepthMask(true);

	if (SharedConstants::TEXTURE_LIGHTING)
	{
		// 4J - was 0xf0f0 but that looks like it is a mistake - maybe meant to be 0xf000f0 to enable both sky & block lighting? choosing 0x00f0 here instead
		// as most likely replicates what the java game does, without breaking our lighting (which doesn't like UVs out of the 0 to 255 range)
		int col = 0x00f0;			
		int u = col % 65536;
		int v = col / 65536;

		glMultiTexCoord2f(GL_TEXTURE1, u / 1.0f, v / 1.0f);
		glColor4f(1, 1, 1, 1);
	}
	// 4J - this doesn't seem right - surely there should be an else in here?
	glColor4f(1, 1, 1, br);
	return 1;
}

ResourceLocation *SpiderRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
	if (mob->instanceof(eTYPE_EVOLVEDSPIDER)) {
		return &EVOLVED_SPIDER_LOCATION;
	}
	else {
		return &SPIDER_LOCATION;
	}
}

void SpiderRenderer::scale(shared_ptr<LivingEntity> mob, float a)
{
	if (mob->instanceof(eTYPE_EVOLVEDSPIDER))
	{
		// don't render the pumpkin of skulls for the skins with that disabled
		// 4J-PB - need to disable rendering armour/skulls/pumpkins for some special skins (Daleks)
		glScalef(1.2f, 1.2f, 1.2f);
	}
}

void SpiderRenderer::renderSpaceSetup(shared_ptr<LivingEntity> entity, float time, float r, float bob, float yRot, float xRot, float scale, float a) {
	if (entity->dimension == 2 && !entity->instanceof(eTYPE_EVOLVEDSPIDER)) {

		SpiderModel* spiderModel = dynamic_cast<SpiderModel*>(model);
		shared_ptr<ItemInstance> headGear = entity->getArmor(3);
		if (headGear != NULL)
		{
			glPushMatrix();

			spiderModel->head->translateTo(1 / 16.0f);

			if (headGear->getItem()->id < 256) {

				if (Tile::tiles[headGear->id] != NULL && TileRenderer::canRender(Tile::tiles[headGear->id]->getRenderShape()))
				{
					float s = 10 / 16.0f;
					glTranslatef(-0 / 16.0f, 0 / 16.0f, -4 / 16.0f);
					glRotatef(90, 0, 1, 0);
					glScalef(s, -s, -s);
				}

				this->entityRenderDispatcher->itemInHandRenderer->renderItem(entity, headGear, 0);
			}

			glPopMatrix();
		}

		if (oxygenSetup == NULL) {
			oxygenSetup = new OxygenSetupModel();
		}

		glPushMatrix();

		spiderModel->body1->translateTo(1 / 16.0f);

		glTranslatef(0.0f, -0.1f, -.35f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		
		bindTexture(&OXYGEN_SETUP_LOCATION);
		float brightness = SharedConstants::TEXTURE_LIGHTING ? 1 : entity->getBrightness(a);
		glColor3f(brightness, brightness, brightness);
		oxygenSetup->render(entity, time, r, bob, yRot, xRot, scale, true);

		glPopMatrix();
	}
}