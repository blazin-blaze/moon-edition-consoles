#include "stdafx.h"
#include "OxygenatorTile.h"
#include "OxygenatorTileEntity.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.inventory.h"
#include "net.minecraft.h"

OxygenatorTile::OxygenatorTile(int id) : BaseEntityTile(id, Material::metal, isSolidRender())
{
	random = new Random();
	iconTop = NULL;
	iconFront = NULL;
	iconBottom = NULL;
	icon = NULL;
}

OxygenatorTile::~OxygenatorTile()
{
	delete random;
}

bool OxygenatorTile::isSolidRender(bool isServerLevel)
{
	return false;
}

shared_ptr<TileEntity> OxygenatorTile::newTileEntity(Level *level)
{
	return shared_ptr<TileEntity>(new OxygenatorTileEntity());
}

bool OxygenatorTile::use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly/*=false*/) // 4J added soundOnly param
{
	if(soundOnly) return false;

	if (level->isClientSide)
	{
		return true;
	}
	shared_ptr<OxygenatorTileEntity> oxygenator = dynamic_pointer_cast<OxygenatorTileEntity>(level->getTileEntity(x, y, z));
	if (oxygenator != NULL) player->openOxygenator(oxygenator);

	return true;
}

void OxygenatorTile::setPlacedBy(Level *level, int x, int y, int z, shared_ptr<LivingEntity> by, shared_ptr<ItemInstance> itemInstance)
{
	if (itemInstance->hasCustomHoverName())
	{
		dynamic_pointer_cast<OxygenatorTileEntity>( level->getTileEntity(x, y, z))->setCustomName(itemInstance->getHoverName());
	}
}

void OxygenatorTile::animateTick(Level *level, int xt, int yt, int zt, Random *random)
{
	double x = xt + 0.4f + random->nextFloat() * 0.2f;
	double y = yt + 0.7f + random->nextFloat() * 0.3f;
	double z = zt + 0.4f + random->nextFloat() * 0.2f;

	shared_ptr<TileEntity> tileEntity = level->getTileEntity(x, y, z);
	if (tileEntity != NULL && ( dynamic_pointer_cast<OxygenatorTileEntity>(tileEntity) != NULL) )
	{
		shared_ptr<OxygenatorTileEntity> container = dynamic_pointer_cast<OxygenatorTileEntity>(tileEntity);
		if(container->canOxygenate()) level->addParticle(eParticleType_smoke, x, y, z, 0, 0, 0);
	}
}

void OxygenatorTile::onRemove(Level *level, int x, int y, int z, int id, int data)
{
	shared_ptr<TileEntity> tileEntity = level->getTileEntity(x, y, z);
	if (tileEntity != NULL && ( dynamic_pointer_cast<OxygenatorTileEntity>(tileEntity) != NULL) )
	{
		shared_ptr<OxygenatorTileEntity> container = dynamic_pointer_cast<OxygenatorTileEntity>(tileEntity);
		for (int i = 0; i < container->getContainerSize(); i++)
		{
			shared_ptr<ItemInstance> item = container->getItem(i);
			if (item != NULL)
			{
				float xo = random->nextFloat() * 0.8f + 0.1f;
				float yo = random->nextFloat() * 0.8f + 0.1f;
				float zo = random->nextFloat() * 0.8f + 0.1f;

				while (item->count > 0)
				{
					int count = random->nextInt(21) + 10;
					if (count > item->count) count = item->count;
					item->count -= count;

					shared_ptr<ItemEntity> itemEntity = shared_ptr<ItemEntity>(new ItemEntity(level, x + xo, y + yo, z + zo, shared_ptr<ItemInstance>( new ItemInstance(item->id, count, item->getAuxValue()))));
					float pow = 0.05f;
					itemEntity->xd = (float) random->nextGaussian() * pow;
					itemEntity->yd = (float) random->nextGaussian() * pow + 0.2f;
					itemEntity->zd = (float) random->nextGaussian() * pow;
					if (item->hasTag())
					{
						itemEntity->getItem()->setTag((CompoundTag *) item->getTag()->copy());
					}
					level->addEntity(itemEntity);
				}
			}
		}
	}
	BaseEntityTile::onRemove(level, x, y, z, id, data);
}

void OxygenatorTile::registerIcons(IconRegister *iconRegister)
{
	BaseEntityTile::registerIcons(iconRegister);
	icon = iconRegister->registerIcon(L"oxygenator_side");
	iconTop = iconRegister->registerIcon(L"oxygenator_top");
	iconFront = iconRegister->registerIcon(L"oxygenator_front");
	iconBottom = iconRegister->registerIcon(L"spaceWorkbench_bottom");
}

Icon *OxygenatorTile::getTexture(int face, int data)
{
	if (face == Facing::UP) return iconTop;
	if (face == Facing::DOWN) return iconBottom;
	if (face == Facing::NORTH || face == Facing::SOUTH) return iconFront;
	return icon;
}