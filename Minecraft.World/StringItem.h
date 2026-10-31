#pragma once
using namespace std;

#include "Item.h"
#include "TilePlanterItem.h"
#include "Tile.h"

class StringItem : public TilePlanterItem
{
public:
	static const wstring STRING_TEXTURES[];
	static const unsigned int STRING_DESCS[];
	static const unsigned int STRING_USE_DESCS[];
	static const int NORMAL;
	static const int GREEN;
private:
	Icon** icons;
public:
	StringItem(int itemId, Tile* tile = Tile::tripWire);
	virtual unsigned int getDescriptionId(shared_ptr<ItemInstance> itemInstance);
	virtual unsigned int getUseDescriptionId(shared_ptr<ItemInstance> itemInstance);
	virtual Icon* getIcon(int itemAuxValue);
	void registerIcons(IconRegister* iconRegister);
};