#pragma once
#include "BaseEntityTile.h"

class IconRegister;
class ChunkRebuildData;

class OxygenatorTile : public BaseEntityTile
{
	friend ChunkRebuildData;
private:
	Random *random;
	Icon *iconTop;
	Icon *iconFront;
	Icon *iconBottom;
	Icon *icon;

public:
	OxygenatorTile(int id);
	~OxygenatorTile();
	virtual bool isSolidRender(bool isServerLevel = false);
	virtual shared_ptr<TileEntity> newTileEntity(Level *level);
	virtual bool use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly = false); // 4J added soundOnly param
	virtual void setPlacedBy(Level *level, int x, int y, int z, shared_ptr<LivingEntity> by, shared_ptr<ItemInstance> itemInstance);
	virtual void animateTick(Level *level, int xt, int yt, int zt, Random *random);
	virtual void onRemove(Level *level, int x, int y, int z, int id, int data);
	virtual void registerIcons(IconRegister *iconRegister);
	Icon *getTexture(int face, int data);
};