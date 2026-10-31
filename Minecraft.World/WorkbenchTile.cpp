#include "stdafx.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"
#include "WorkbenchTile.h"

WorkbenchTile::WorkbenchTile(int id, Material* mat) : Tile(id, mat)
{
	iconTop = NULL;
	iconFront = NULL;
	iconBottom = NULL;
}

WorkbenchTile::WorkbenchTile(int id) : Tile(id, Material::wood)
{
	iconTop = NULL;
	iconFront = NULL;
	iconBottom = NULL;
}

Icon *WorkbenchTile::getTexture(int face, int data)
{
	if (face == Facing::UP) return iconTop;
	if (face == Facing::DOWN) {
		if (this->material == Material::metal) {
			return iconBottom;
		}
		else {
			return Tile::wood->getTexture(face);
		}
	}
	if (face == Facing::NORTH || face == Facing::WEST) return iconFront;
	return icon;
}

void WorkbenchTile::registerIcons(IconRegister *iconRegister)
{
	if(this->material == Material::metal) {
		icon = iconRegister->registerIcon(L"spaceWorkbench_side");
		iconTop = iconRegister->registerIcon(L"spaceWorkbench_top");
		iconFront = iconRegister->registerIcon(L"spaceWorkbench_front");
		iconBottom = iconRegister->registerIcon(L"spaceWorkbench_bottom");
	}
	else {
		icon = iconRegister->registerIcon(L"workbench_side");
		iconTop = iconRegister->registerIcon(L"workbench_top");
		iconFront = iconRegister->registerIcon(L"workbench_front");
		iconBottom = NULL;
	}
}

// 4J-PB - Adding a TestUse for tooltip display
bool WorkbenchTile::TestUse()
{
	return true;
}

bool WorkbenchTile::use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly/*=false*/) // 4J added soundOnly param
{
	if( soundOnly ) return false;
	if (level->isClientSide)
	{
		return true;
	}
	if (this->material == Material::metal) {
		player->startSpaceCrafting(x, y, z);
	}
	else {
		player->startCrafting(x, y, z);
	}
	//player->openFireworks(x, y, z);
	return true;
}