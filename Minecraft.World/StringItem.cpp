#include "stdafx.h"
#include "StringItem.h"
#include "net.minecraft.world.h"

const wstring StringItem::STRING_TEXTURES[] =
{ L"string", L"greenString" };

const unsigned int StringItem::STRING_DESCS[] =
{
	IDS_ITEM_STRING,
	IDS_ITEM_GREEN_STRING
};

const unsigned int StringItem::STRING_USE_DESCS[] =
{
	IDS_DESC_STRING,
	IDS_DESC_GREEN_STRING
};

const int StringItem::NORMAL = 0;
const int StringItem::GREEN = 1;

StringItem::StringItem(int id, Tile* tile) : TilePlanterItem(id, tile) {
	setStackedByData(true);
	icons = nullptr;
}

unsigned int StringItem::getDescriptionId(shared_ptr<ItemInstance> itemInstance)
{
	int stringValue = Mth::clamp(itemInstance->getAuxValue(), 0, 1);
	return STRING_DESCS[stringValue];
}

unsigned int StringItem::getUseDescriptionId(shared_ptr<ItemInstance> itemInstance)
{
	return STRING_USE_DESCS[itemInstance->getAuxValue()];
}

Icon* StringItem::getIcon(int itemAuxValue)
{
	int stringValue = Mth::clamp(itemAuxValue, 0, 1);
	return icons[stringValue];
}

void StringItem::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon *[2];

	for (int i = 0; i < 2; i++)
	{
		icons[i] = iconRegister->registerIcon(STRING_TEXTURES[i]);
	}
}