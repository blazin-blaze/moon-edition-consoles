#include "stdafx.h"
#include "net.minecraft.world.level.tile.h"
#include "PickaxeItem.h"

TileArray PickaxeItem::diggables;

void PickaxeItem::staticCtor()
{
	PickaxeItem::diggables = TileArray( PICKAXE_DIGGABLES);
	diggables.data[0] = Tile::cobblestone;
	diggables.data[1] = Tile::stoneSlab;
	diggables.data[2] = Tile::stoneSlabHalf;
	diggables.data[3] = Tile::stone;
	diggables.data[4] = Tile::sandStone;
	diggables.data[5] = Tile::mossyCobblestone;
	diggables.data[6] = Tile::ironOre;
	diggables.data[7] = Tile::ironBlock;
	diggables.data[8] = Tile::coalOre;
	diggables.data[9] = Tile::goldBlock;
	diggables.data[10] = Tile::goldOre;
	diggables.data[11] = Tile::diamondOre;
	diggables.data[12] = Tile::diamondBlock;
	diggables.data[13] = Tile::ice;
	diggables.data[14] = Tile::netherRack;
	diggables.data[15] = Tile::lapisOre;
	diggables.data[16] = Tile::lapisBlock;
	diggables.data[17] = Tile::redStoneOre;
	diggables.data[18] = Tile::redStoneOre_lit;
	diggables.data[19] = Tile::rail;
	diggables.data[20] = Tile::detectorRail;
	diggables.data[21] = Tile::goldenRail;
	diggables.data[21] = Tile::activatorRail;
	diggables.data[22] = Tile::sapphireOre;
	diggables.data[23] = Tile::moonQuartzOre;
	diggables.data[24] = Tile::titaniumOre;
	diggables.data[25] = Tile::copperOre;
	diggables.data[26] = Tile::tinOre;
	diggables.data[27] = Tile::aluminiumOre;
	diggables.data[28] = Tile::siliconOre;
	diggables.data[29] = Tile::moonStone;
	diggables.data[30] = Tile::titaniumBlock;
	diggables.data[31] = Tile::sapphireBlock;
	diggables.data[32] = Tile::copperBlock;
	diggables.data[33] = Tile::tinBlock;
	diggables.data[34] = Tile::aluminiumBlock;
	diggables.data[35] = Tile::siliconBlock;
	diggables.data[36] = Tile::moonDungeonBricks;
	diggables.data[37] = Tile::moonDungeonMossyBricks;
	diggables.data[38] = Tile::moonDungeonCrackedBricks;
	diggables.data[39] = Tile::quartzDoor;
	diggables.data[40] = Tile::quartzFence;
	diggables.data[41] = Tile::quartzFenceGate;
	diggables.data[42] = Tile::quartzBookshelf;
	diggables.data[43] = Tile::spaceWorkbench;
}

PickaxeItem::PickaxeItem(int id, const Tier *tier) : DiggerItem(id, 2, tier, &diggables)
{
}

bool PickaxeItem::canDestroySpecial(Tile *tile)
{
	if (tile == Tile::obsidian) return tier->getLevel() == 3;
	if (tile == Tile::titaniumOre || tile == Tile::titaniumBlock) return tier->getLevel() == 3;
	if (tile == Tile::siliconOre || tile == Tile::siliconBlock) return tier->getLevel() >= 2;
	if (tile == Tile::sapphireOre || tile == Tile::sapphireBlock) return tier->getLevel() >= 2;
	if (tile == Tile::tinOre || tile == Tile::tinBlock) return tier->getLevel() >= 1;
	if (tile == Tile::copperOre || tile == Tile::copperBlock) return tier->getLevel() >= 1;
	if (tile == Tile::aluminiumOre || tile == Tile::aluminiumBlock) return tier->getLevel() >= 1;
	if (tile == Tile::moonQuartzOre) return tier->getLevel() >= 1;
	if (tile == Tile::diamondBlock || tile == Tile::diamondOre) return tier->getLevel() >= 2;
	if (tile == Tile::emeraldBlock || tile == Tile::emeraldOre) return tier->getLevel() >= 2;
	if (tile == Tile::goldBlock || tile == Tile::goldOre) return tier->getLevel() >= 2;
	if (tile == Tile::ironBlock || tile == Tile::ironOre) return tier->getLevel() >= 1;
	if (tile == Tile::lapisBlock || tile == Tile::lapisOre) return tier->getLevel() >= 1;
	if (tile == Tile::redStoneOre || tile == Tile::redStoneOre_lit) return tier->getLevel() >= 2;
	if (tile->material == Material::stone) return true;
	if (tile->material == Material::metal) return true;
	if (tile->material == Material::heavyMetal) return true;
	return false;
}

// 4J - brought forward from 1.2.3
float PickaxeItem::getDestroySpeed(shared_ptr<ItemInstance> itemInstance, Tile *tile)
{
	if (tile != NULL && (tile->material == Material::metal || tile->material == Material::heavyMetal || tile->material == Material::stone || tile->material == Material::quartz))
	{
		return speed;
	}
	return DiggerItem::getDestroySpeed(itemInstance, tile);
}
