#include "stdafx.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.level.levelgen.structure.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.item.h"
#include "JavaMath.h"
#include "WeighedTreasure.h"
#include "MoonMineShaftPieces.h"

WeighedTreasureArray MoonMineShaftPieces::smallTreasureItems;;

void MoonMineShaftPieces::staticCtor()
{
	smallTreasureItems = WeighedTreasureArray(30);

	smallTreasureItems[0] = new WeighedTreasure(Item::sapphire_Id, 0, 1, 12, 7);
	smallTreasureItems[1] = new WeighedTreasure(Item::titaniumIngot_Id, 0, 1, 2, 1);
	smallTreasureItems[2] = new WeighedTreasure(Item::cheese_Id, 0, 1, 15, 10);
	smallTreasureItems[3] = new WeighedTreasure(Item::netherQuartz_Id, 0, 1, 25, 10);
	smallTreasureItems[4] = new WeighedTreasure(Item::oxygenTank_Id, 0, 1, 1, 1);
	smallTreasureItems[5] = new WeighedTreasure(Item::cheeseburger_Id, 0, 1, 2, 7);
	smallTreasureItems[6] = new WeighedTreasure(Item::poutine_Id, 0, 1, 1, 5);
	smallTreasureItems[7] = new WeighedTreasure(Item::apple_gold_Id, 0, 1, 1, 7);
	smallTreasureItems[8] = new WeighedTreasure(Item::apple_gold_Id, 1, 1, 1, 5);
	smallTreasureItems[9] = new WeighedTreasure(Item::nameTag_Id, 0, 1, 1, 7);
	smallTreasureItems[10] = new WeighedTreasure(Item::diamond_Id, 0, 1, 10, 7);
	smallTreasureItems[11] = new WeighedTreasure(Item::sword_diamond_Id, 0, 1, 1, 3);
	smallTreasureItems[12] = new WeighedTreasure(Item::pickAxe_diamond_Id, 0, 1, 1, 3);
	smallTreasureItems[13] = new WeighedTreasure(Item::hatchet_diamond_Id, 0, 1, 1, 3);
	smallTreasureItems[14] = new WeighedTreasure(Item::hoe_diamond_Id, 0, 1, 1, 3);
	smallTreasureItems[15] = new WeighedTreasure(Item::shovel_diamond_Id, 0, 1, 1, 3);
	smallTreasureItems[16] = new WeighedTreasure(Item::dehydratedApple_Id, 0, 1, 3, 10);
	smallTreasureItems[17] = new WeighedTreasure(Item::dehydratedCarrot_Id, 0, 1, 3, 10);
	smallTreasureItems[18] = new WeighedTreasure(Item::dehydratedMelon_Id, 0, 1, 3, 10);
	smallTreasureItems[19] = new WeighedTreasure(Item::dehydratedPotato_Id, 0, 1, 3, 10);
	smallTreasureItems[20] = new WeighedTreasure(Item::cannedBeef_Id, 0, 1, 3, 10);
	smallTreasureItems[21] = new WeighedTreasure(Item::sapphireHelmet_Id, 0, 1, 1, 3);
	smallTreasureItems[22] = new WeighedTreasure(Item::sapphireChestplate_Id, 0, 1, 1, 3);
	smallTreasureItems[23] = new WeighedTreasure(Item::sapphireLeggings_Id, 0, 1, 1, 3);
	smallTreasureItems[24] = new WeighedTreasure(Item::sapphireBoots_Id, 0, 1, 1, 3);
	smallTreasureItems[25] = new WeighedTreasure(Item::sapphireSword_Id, 0, 1, 1, 3);
	smallTreasureItems[26] = new WeighedTreasure(Item::sapphirePickaxe_Id, 0, 1, 1, 3);
	smallTreasureItems[27] = new WeighedTreasure(Item::sapphireHatchet_Id, 0, 1, 1, 3);
	smallTreasureItems[28] = new WeighedTreasure(Item::sapphireShovel_Id, 0, 1, 1, 3);
	smallTreasureItems[29] = new WeighedTreasure(Item::sapphireHoe_Id, 0, 1, 1, 3);
}

void MoonMineShaftPieces::loadStatic()
{
	StructureFeatureIO::setPieceId(eStructurePiece_MoonMineShaftCorridor, MoonMineShaftCorridor::Create, L"MoonMSCorridor");
	StructureFeatureIO::setPieceId(eStructurePiece_MoonMineShaftCrossing, MoonMineShaftCrossing::Create, L"MoonMSCrossing");
	StructureFeatureIO::setPieceId(eStructurePiece_MoonMineShaftRoom, MoonMineShaftRoom::Create, L"MoonMSRoom");
	StructureFeatureIO::setPieceId(eStructurePiece_MoonMineShaftStairs, MoonMineShaftStairs::Create, L"MoonMSStairs");
}

StructurePiece* MoonMineShaftPieces::createRandomShaftPiece(list<StructurePiece*>* pieces, Random* random, int footX, int footY, int footZ, int direction, int genDepth)
{
	int randomSelection = random->nextInt(100);
	if (randomSelection >= 80)
	{
		BoundingBox* crossingBox = MoonMineShaftCrossing::findCrossing(pieces, random, footX, footY, footZ, direction);
		if (crossingBox != nullptr)
		{
			return new MoonMineShaftCrossing(genDepth, random, crossingBox, direction);
		}
	}
	else if (randomSelection >= 70)
	{
		BoundingBox* stairsBox = MoonMineShaftStairs::findStairs(pieces, random, footX, footY, footZ, direction);
		if (stairsBox != nullptr)
		{
			return new MoonMineShaftPieces::MoonMineShaftStairs(genDepth, random, stairsBox, direction);
		}
	}
	else
	{
		BoundingBox* corridorBox = MoonMineShaftCorridor::findCorridorSize(pieces, random, footX, footY, footZ, direction);
		if (corridorBox != nullptr)
		{
			return new MoonMineShaftCorridor(genDepth, random, corridorBox, direction);
		}
	}

	return nullptr;
}

StructurePiece* MoonMineShaftPieces::generateAndAddPiece(StructurePiece* startPiece, list<StructurePiece*>* pieces, Random* random, int footX, int footY, int footZ, int direction, int depth)
{
	if (depth > MAX_DEPTH)
	{
		return nullptr;
	}
	if (abs(footX - startPiece->getBoundingBox()->x0) > 5 * 16 || abs(footZ - startPiece->getBoundingBox()->z0) > 5 * 16)
	{
		return nullptr;
	}

	StructurePiece* newPiece = createRandomShaftPiece(pieces, random, footX, footY, footZ, direction, depth + 1);
	if (newPiece != nullptr)
	{
		MemSect(50);
		pieces->push_back(newPiece);
		MemSect(0);
		newPiece->addChildren(startPiece, pieces, random);
	}
	return newPiece;
}

MoonMineShaftPieces::MoonMineShaftRoom::MoonMineShaftRoom()
{
	// for reflection
}

MoonMineShaftPieces::MoonMineShaftRoom::MoonMineShaftRoom(int genDepth, Random* random, int west, int north) : StructurePiece(genDepth)
{
	boundingBox = new BoundingBox(west, 50, north, west + 7 + random->nextInt(6), 54 + random->nextInt(6), north + 7 + random->nextInt(6));
}

MoonMineShaftPieces::MoonMineShaftRoom::~MoonMineShaftRoom()
{
	for (list<BoundingBox*>::iterator it = childEntranceBoxes.begin(); it != childEntranceBoxes.end(); ++it)
	{
		delete *it;
	}
}

void MoonMineShaftPieces::MoonMineShaftRoom::addChildren(StructurePiece* startPiece, list<StructurePiece*>* pieces, Random* random)
{

	int depth = getGenDepth();

	int pos;

	int heightSpace = boundingBox->getYSpan() - DEFAULT_SHAFT_HEIGHT - 1;
	if (heightSpace <= 0)
	{
		heightSpace = 1;
	}

	// northern exits
	pos = 0;
	while (pos < boundingBox->getXSpan())
	{
		pos += random->nextInt(boundingBox->getXSpan());
		if ((pos + DEFAULT_SHAFT_WIDTH) > boundingBox->getXSpan())
		{
			break;
		}
		StructurePiece* child = generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 + pos, boundingBox->y0 + random->nextInt(heightSpace) + 1, boundingBox->z0 - 1, Direction::NORTH, depth);
		if (child != nullptr)
		{
			BoundingBox* childBox = child->getBoundingBox();
			childEntranceBoxes.push_back(new BoundingBox(childBox->x0, childBox->y0, boundingBox->z0, childBox->x1, childBox->y1, boundingBox->z0 + 1));
		}
		pos += DEFAULT_SHAFT_WIDTH + 1;
	}
	// southern exits
	pos = 0;
	while (pos < boundingBox->getXSpan())
	{
		pos += random->nextInt(boundingBox->getXSpan());
		if ((pos + DEFAULT_SHAFT_WIDTH) > boundingBox->getXSpan())
		{
			break;
		}
		StructurePiece* child = generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 + pos, boundingBox->y0 + random->nextInt(heightSpace) + 1, boundingBox->z1 + 1, Direction::SOUTH, depth);
		if (child != nullptr)
		{
			BoundingBox* childBox = child->getBoundingBox();
			childEntranceBoxes.push_back(new BoundingBox(childBox->x0, childBox->y0, boundingBox->z1 - 1, childBox->x1, childBox->y1, boundingBox->z1));
		}
		pos += DEFAULT_SHAFT_WIDTH + 1;
	}
	// western exits
	pos = 0;
	while (pos < boundingBox->getZSpan())
	{
		pos += random->nextInt(boundingBox->getZSpan());
		if ((pos + DEFAULT_SHAFT_WIDTH) > boundingBox->getZSpan())
		{
			break;
		}
		StructurePiece* child = generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 - 1, boundingBox->y0 + random->nextInt(heightSpace) + 1, boundingBox->z0 + pos, Direction::WEST, depth);
		if (child != nullptr)
		{
			BoundingBox* childBox = child->getBoundingBox();
			childEntranceBoxes.push_back(new BoundingBox(boundingBox->x0, childBox->y0, childBox->z0, boundingBox->x0 + 1, childBox->y1, childBox->z1));
		}
		pos += DEFAULT_SHAFT_WIDTH + 1;
	}
	// eastern exits
	pos = 0;
	while (pos < boundingBox->getZSpan())
	{
		pos += random->nextInt(boundingBox->getZSpan());
		if ((pos + DEFAULT_SHAFT_WIDTH) > boundingBox->getZSpan())
		{
			break;
		}
		StructurePiece* child = generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 + 1, boundingBox->y0 + random->nextInt(heightSpace) + 1, boundingBox->z0 + pos, Direction::EAST, depth);
		if (child != nullptr)
		{
			BoundingBox* childBox = child->getBoundingBox();
			childEntranceBoxes.push_back(new BoundingBox(boundingBox->x1 - 1, childBox->y0, childBox->z0, boundingBox->x1, childBox->y1, childBox->z1));
		}
		pos += DEFAULT_SHAFT_WIDTH + 1;
	}
}

bool MoonMineShaftPieces::MoonMineShaftRoom::postProcess(Level* level, Random* random, BoundingBox* chunkBB)
{
	if (edgesLiquid(level, chunkBB))
	{
		return false;
	}

	// floor
	generateBox(level, chunkBB, boundingBox->x0, boundingBox->y0, boundingBox->z0, boundingBox->x1, boundingBox->y0, boundingBox->z1, Tile::moonDirt_Id, 0, true);

	// room air
	generateBox(level, chunkBB, boundingBox->x0, boundingBox->y0 + 1, boundingBox->z0, boundingBox->x1, min(boundingBox->y0 + 3, boundingBox->y1), boundingBox->z1, 0, 0, false);
	for (list<BoundingBox*>::iterator it = childEntranceBoxes.begin(); it != childEntranceBoxes.end(); ++it)
	{
		BoundingBox* entranceBox = *it;
		generateBox(level, chunkBB, entranceBox->x0, entranceBox->y1 - (DEFAULT_SHAFT_HEIGHT - 1), entranceBox->z0, entranceBox->x1, entranceBox->y1, entranceBox->z1, 0, 0, false);
	}
	generateUpperHalfSphere(level, chunkBB, boundingBox->x0, boundingBox->y0 + 4, boundingBox->z0, boundingBox->x1, boundingBox->y1, boundingBox->z1, 0, false);

	return true;
}

void MoonMineShaftPieces::MoonMineShaftRoom::addAdditonalSaveData(CompoundTag* tag)
{
	ListTag<IntArrayTag>* entrances = new ListTag<IntArrayTag>(L"Entrances");
	for (list<BoundingBox*>::iterator it = childEntranceBoxes.begin(); it != childEntranceBoxes.end(); ++it)
	{
		BoundingBox* bb = *it;
		entrances->add(bb->createTag(L""));
	}
	tag->put(L"Entrances", entrances);
}

void MoonMineShaftPieces::MoonMineShaftRoom::readAdditonalSaveData(CompoundTag* tag)
{
	ListTag<IntArrayTag>* entrances = (ListTag<IntArrayTag> *) tag->getList(L"Entrances");
	for (int i = 0; i < entrances->size(); i++)
	{
		childEntranceBoxes.push_back(new BoundingBox(entrances->get(i)->data));
	}
}

MoonMineShaftPieces::MoonMineShaftCorridor::MoonMineShaftCorridor()
{
	// for reflection
}

void MoonMineShaftPieces::MoonMineShaftCorridor::addAdditonalSaveData(CompoundTag* tag)
{
	tag->putBoolean(L"hr", hasRails);
	tag->putBoolean(L"sc", spiderCorridor);
	tag->putBoolean(L"hps", hasPlacedSpider);
	tag->putInt(L"Num", numSections);
}

void MoonMineShaftPieces::MoonMineShaftCorridor::readAdditonalSaveData(CompoundTag* tag)
{
	hasRails = tag->getBoolean(L"hr");
	spiderCorridor = tag->getBoolean(L"sc");
	hasPlacedSpider = tag->getBoolean(L"hps");
	numSections = tag->getInt(L"Num");
}

MoonMineShaftPieces::MoonMineShaftCorridor::MoonMineShaftCorridor(int genDepth, Random* random, BoundingBox* corridorBox, int direction)
	: StructurePiece(genDepth)
{
	orientation = direction;
	boundingBox = corridorBox;
	hasRails = random->nextInt(3) == 0;
	hasPlacedSpider = false;
	spiderCorridor = !hasRails && random->nextInt(23) == 0;
	//debug
	//spiderCorridor = !hasRails ;//&& random->nextInt(23) == 0;

	if (orientation == Direction::NORTH || orientation == Direction::SOUTH)
	{
		numSections = corridorBox->getZSpan() / DEFAULT_SHAFT_LENGTH;
	}
	else
	{
		numSections = corridorBox->getXSpan() / DEFAULT_SHAFT_LENGTH;
	}
}

BoundingBox* MoonMineShaftPieces::MoonMineShaftCorridor::findCorridorSize(list<StructurePiece*>* pieces, Random* random, int footX, int footY, int footZ, int direction)
{

	BoundingBox* box = new BoundingBox(footX, footY, footZ, footX, footY + (DEFAULT_SHAFT_HEIGHT - 1), footZ);

	int corridorLength = random->nextInt(3) + 2;
	while (corridorLength > 0)
	{
		int blockLength = corridorLength * DEFAULT_SHAFT_LENGTH;

		switch (direction)
		{
		case Direction::NORTH:
			box->x1 = footX + (DEFAULT_SHAFT_WIDTH - 1);
			box->z0 = footZ - (blockLength - 1);
			break;
		case Direction::SOUTH:
			box->x1 = footX + (DEFAULT_SHAFT_WIDTH - 1);
			box->z1 = footZ + (blockLength - 1);
			break;
		case Direction::WEST:
			box->x0 = footX - (blockLength - 1);
			box->z1 = footZ + (DEFAULT_SHAFT_WIDTH - 1);
			break;
		case Direction::EAST:
			box->x1 = footX + (blockLength - 1);
			box->z1 = footZ + (DEFAULT_SHAFT_WIDTH - 1);
			break;
		}

		if (StructurePiece::findCollisionPiece(pieces, box) != nullptr)
		{
			corridorLength--;
		}
		else
		{
			break;
		}
	}

	if (corridorLength > 0)
	{
		return box;
	}
	delete box;
	// unable to place corridor here
	return nullptr;
}

void MoonMineShaftPieces::MoonMineShaftCorridor::addChildren(StructurePiece* startPiece, list<StructurePiece*>* pieces, Random* random)
{
	int depth = getGenDepth();
	int endSelection = random->nextInt(4);
	switch (orientation)
	{
	case Direction::NORTH:
		if (endSelection <= 1)
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x0, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z0 - 1, orientation, depth);
		}
		else if (endSelection == 2)
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 - 1, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z0, Direction::WEST, depth);
		}
		else
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 + 1, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z0, Direction::EAST, depth);
		}
		break;
	case Direction::SOUTH:
		if (endSelection <= 1)
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x0, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z1 + 1, orientation, depth);
		}
		else if (endSelection == 2)
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 - 1, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z1 - DEFAULT_SHAFT_WIDTH, Direction::WEST, depth);
		}
		else
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 + 1, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z1 - DEFAULT_SHAFT_WIDTH, Direction::EAST, depth);
		}
		break;
	case Direction::WEST:
		if (endSelection <= 1)
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 - 1, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z0, orientation, depth);
		}
		else if (endSelection == 2)
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x0, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z0 - 1, Direction::NORTH, depth);
		}
		else
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x0, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z1 + 1, Direction::SOUTH, depth);
		}
		break;
	case Direction::EAST:
		if (endSelection <= 1)
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 + 1, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z0, orientation, depth);
		}
		else if (endSelection == 2)
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 - DEFAULT_SHAFT_WIDTH, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z0 - 1, Direction::NORTH, depth);
		}
		else
		{
			generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 - DEFAULT_SHAFT_WIDTH, boundingBox->y0 - 1 + random->nextInt(3), boundingBox->z1 + 1, Direction::SOUTH, depth);
		}
		break;
	}

	// generate cross sections using higher depth
	if (depth < MAX_DEPTH)
	{
		if (orientation == Direction::NORTH || orientation == Direction::SOUTH)
		{
			for (int z = boundingBox->z0 + 3; (z + DEFAULT_SHAFT_WIDTH) <= boundingBox->z1; z += DEFAULT_SHAFT_LENGTH)
			{
				int selection = random->nextInt(5);
				if (selection == 0)
				{
					generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 - 1, boundingBox->y0, z, Direction::WEST, depth + 1);
				}
				else if (selection == 1)
				{
					generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 + 1, boundingBox->y0, z, Direction::EAST, depth + 1);
				}
			}
		}
		else
		{
			for (int x = boundingBox->x0 + 3; (x + DEFAULT_SHAFT_WIDTH) <= boundingBox->x1; x += DEFAULT_SHAFT_LENGTH)
			{
				int selection = random->nextInt(5);
				if (selection == 0)
				{
					generateAndAddPiece(startPiece, pieces, random, x, boundingBox->y0, boundingBox->z0 - 1, Direction::NORTH, depth + 1);
				}
				else if (selection == 1)
				{
					generateAndAddPiece(startPiece, pieces, random, x, boundingBox->y0, boundingBox->z1 + 1, Direction::SOUTH, depth + 1);
				}
			}
		}
	}
}

bool MoonMineShaftPieces::MoonMineShaftCorridor::createChest(Level* level, BoundingBox* chunkBB, Random* random, int x, int y, int z, WeighedTreasureArray treasure, int numRolls)
{
	int worldX = getWorldX(x, z);
	int worldY = getWorldY(y);
	int worldZ = getWorldZ(x, z);

	if (chunkBB->isInside(worldX, worldY, worldZ))
	{
		if (level->getTile(worldX, worldY, worldZ) == 0)
		{
			level->setTileAndData(worldX, worldY, worldZ, Tile::treasureChest_Id, 0, Tile::UPDATE_CLIENTS);
			shared_ptr<ChestTileEntity> chest = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(worldX, worldY, worldZ));
			if (chest != nullptr) {
				WeighedTreasure::addChestItems(random, treasure, chest, numRolls);
			}
			return true;
		}
	}

	return false;
}

bool MoonMineShaftPieces::MoonMineShaftCorridor::postProcess(Level* level, Random* random, BoundingBox* chunkBB)
{
	if (edgesLiquid(level, chunkBB))
	{
		return false;
	}

	const int x0 = 0;
	const int x1 = DEFAULT_SHAFT_WIDTH - 1;
	const int y0 = 0;
	const int y1 = DEFAULT_SHAFT_HEIGHT - 1;
	const int length = (numSections * DEFAULT_SHAFT_LENGTH) - 1;

	// corridor air
	generateBox(level, chunkBB, x0, 0, y0, x1, y1 - 1, length, 0, 0, false);
	generateMaybeBox(level, chunkBB, random, .8f, x0, y1, y0, x1, y1, length, 0, 0, false);

	if (spiderCorridor)
	{
		generateMaybeBox(level, chunkBB, random, .6f, x0, 0, y0, x1, y1 - 1, length, Tile::greenWeb_Id, 0, false);
	}

	// place a support in every section
	for (int section = 0; section < numSections; section++)
	{

		int z = 2 + section * DEFAULT_SHAFT_LENGTH;

		// 4J-PB - Bringing forward the changes in 1.2.3
		generateBox(level, chunkBB, x0, y0, z, x0, y1 - 1, z, Tile::quartzFence_Id, 0, false);
		generateBox(level, chunkBB, x1, y0, z, x1, y1 - 1, z, Tile::quartzFence_Id, 0, false);
		if (random->nextInt(4) == 0)
		{
			generateBox(level, chunkBB, x0, y1, z, x0, y1, z, Tile::quartzBlock_Id, 0, false);
			generateBox(level, chunkBB, x1, y1, z, x1, y1, z, Tile::quartzBlock_Id, 0, false);
		}
		else
		{
			generateBox(level, chunkBB, x0, y1, z, x1, y1, z, Tile::quartzBlock_Id, 0, false);
		}
		maybeGenerateBlock(level, chunkBB, random, .1f, x0, y1, z - 1, Tile::greenWeb_Id, 0);
		maybeGenerateBlock(level, chunkBB, random, .1f, x1, y1, z - 1, Tile::greenWeb_Id, 0);
		maybeGenerateBlock(level, chunkBB, random, .1f, x0, y1, z + 1, Tile::greenWeb_Id, 0);
		maybeGenerateBlock(level, chunkBB, random, .1f, x1, y1, z + 1, Tile::greenWeb_Id, 0);
		maybeGenerateBlock(level, chunkBB, random, .05f, x0, y1, z - 2, Tile::greenWeb_Id, 0);
		maybeGenerateBlock(level, chunkBB, random, .05f, x1, y1, z - 2, Tile::greenWeb_Id, 0);
		maybeGenerateBlock(level, chunkBB, random, .05f, x0, y1, z + 2, Tile::greenWeb_Id, 0);
		maybeGenerateBlock(level, chunkBB, random, .05f, x1, y1, z + 2, Tile::greenWeb_Id, 0);

		maybeGenerateBlock(level, chunkBB, random, .05f, x0 + 1, y1, z - 1, Tile::glowstoneTorch_Id, 0);
		maybeGenerateBlock(level, chunkBB, random, .05f, x0 + 1, y1, z + 1, Tile::glowstoneTorch_Id, 0);

		if (random->nextInt(100) == 0)
		{
			createChest(level, chunkBB, random, x1, y0, z - 1, WeighedTreasure::addToTreasure(smallTreasureItems, Item::enchantedBook->createForRandomTreasure(random)), 3 + random->nextInt(4));
		}
		if (random->nextInt(100) == 0)
		{
			createChest(level, chunkBB, random, x0, y0, z + 1, WeighedTreasure::addToTreasure(smallTreasureItems, Item::enchantedBook->createForRandomTreasure(random)), 3 + random->nextInt(4));
		}

		if (spiderCorridor && !hasPlacedSpider)
		{
			int y = getWorldY(y0), newZ = z - 1 + random->nextInt(3);
			int x = getWorldX(x0 + 1, newZ);
			newZ = getWorldZ(x0 + 1, newZ);
			if (chunkBB->isInside(x, y, newZ))
			{
				hasPlacedSpider = true;
				level->setTileAndData(x, y, newZ, Tile::mobSpawner_Id, 0, Tile::UPDATE_CLIENTS);
				shared_ptr<MobSpawnerTileEntity> entity = dynamic_pointer_cast<MobSpawnerTileEntity>(level->getTileEntity(x, y, newZ));
				if (entity != nullptr) entity->getSpawner()->setEntityId(L"EvolvedSpider");
			}
		}
	}

	// prevent air floating
	for (int x = x0; x <= x1; x++)
	{
		for (int z = 0; z <= length; z++)
		{
			int block = getBlock(level, x, -1, z, chunkBB);
			if (block == 0)
			{
				placeBlock(level, Tile::quartzBlock_Id, 0, x, -1, z, chunkBB);
			}
		}
	}

	if (hasRails)
	{
		for (int z = 0; z <= length; z++)
		{
			int floor = getBlock(level, x0 + 1, y0 - 1, z, chunkBB);
			if (floor > 0 && Tile::solid[floor])
			{
				maybeGenerateBlock(level, chunkBB, random, .7f, x0 + 1, y0, z, Tile::rail_Id, getOrientationData(Tile::rail_Id, BaseRailTile::DIR_FLAT_Z));
			}
		}
	}

	return true;
}

MoonMineShaftPieces::MoonMineShaftCrossing::MoonMineShaftCrossing()
{
	// for reflection
}

void MoonMineShaftPieces::MoonMineShaftCrossing::addAdditonalSaveData(CompoundTag* tag)
{
	tag->putBoolean(L"tf", isTwoFloored);
	tag->putInt(L"D", direction);
}

void MoonMineShaftPieces::MoonMineShaftCrossing::readAdditonalSaveData(CompoundTag* tag)
{
	isTwoFloored = tag->getBoolean(L"tf");
	direction = tag->getInt(L"D");
}

MoonMineShaftPieces::MoonMineShaftCrossing::MoonMineShaftCrossing(int genDepth, Random* random, BoundingBox* crossingBox, int direction)
	: StructurePiece(genDepth), direction(direction), isTwoFloored(crossingBox->getYSpan() > DEFAULT_SHAFT_HEIGHT)
{
	boundingBox = crossingBox;
}

BoundingBox* MoonMineShaftPieces::MoonMineShaftCrossing::findCrossing(list<StructurePiece*>* pieces, Random* random, int footX, int footY, int footZ, int direction)
{

	BoundingBox* box = new BoundingBox(footX, footY, footZ, footX, footY + (DEFAULT_SHAFT_HEIGHT - 1), footZ);

	if (random->nextInt(4) == 0)
	{
		box->y1 += DEFAULT_SHAFT_HEIGHT + 1; // two-floored
	}

	switch (direction)
	{
	case Direction::NORTH:
		box->x0 = footX - 1;
		box->x1 = footX + DEFAULT_SHAFT_WIDTH;
		box->z0 = footZ - (DEFAULT_SHAFT_WIDTH + 1);
		break;
	case Direction::SOUTH:
		box->x0 = footX - 1;
		box->x1 = footX + DEFAULT_SHAFT_WIDTH;
		box->z1 = footZ + (DEFAULT_SHAFT_WIDTH + 1);
		break;
	case Direction::WEST:
		box->x0 = footX - (DEFAULT_SHAFT_WIDTH + 1);
		box->z0 = footZ - 1;
		box->z1 = footZ + DEFAULT_SHAFT_WIDTH;
		break;
	case Direction::EAST:
		box->x1 = footX + (DEFAULT_SHAFT_WIDTH + 1);
		box->z0 = footZ - 1;
		box->z1 = footZ + DEFAULT_SHAFT_WIDTH;
		break;
	}

	if (StructurePiece::findCollisionPiece(pieces, box) != nullptr)
	{
		delete box;
		return nullptr;
	}

	return box;
}

void MoonMineShaftPieces::MoonMineShaftCrossing::addChildren(StructurePiece* startPiece, list<StructurePiece*>* pieces, Random* random)
{

	int depth = getGenDepth();
	// crossings are coming from a direction and will generate children
	// in the
	// remaining three directions
	switch (direction)
	{
	case Direction::NORTH:
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 + 1, boundingBox->y0, boundingBox->z0 - 1, Direction::NORTH, depth);
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 - 1, boundingBox->y0, boundingBox->z0 + 1, Direction::WEST, depth);
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 + 1, boundingBox->y0, boundingBox->z0 + 1, Direction::EAST, depth);
		break;
	case Direction::SOUTH:
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 + 1, boundingBox->y0, boundingBox->z1 + 1, Direction::SOUTH, depth);
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 - 1, boundingBox->y0, boundingBox->z0 + 1, Direction::WEST, depth);
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 + 1, boundingBox->y0, boundingBox->z0 + 1, Direction::EAST, depth);
		break;
	case Direction::WEST:
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 + 1, boundingBox->y0, boundingBox->z0 - 1, Direction::NORTH, depth);
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 + 1, boundingBox->y0, boundingBox->z1 + 1, Direction::SOUTH, depth);
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 - 1, boundingBox->y0, boundingBox->z0 + 1, Direction::WEST, depth);
		break;
	case Direction::EAST:
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 + 1, boundingBox->y0, boundingBox->z0 - 1, Direction::NORTH, depth);
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 + 1, boundingBox->y0, boundingBox->z1 + 1, Direction::SOUTH, depth);
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 + 1, boundingBox->y0, boundingBox->z0 + 1, Direction::EAST, depth);
		break;
	}

	if (isTwoFloored)
	{
		if (random->nextBoolean()) generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 + 1, boundingBox->y0 + DEFAULT_SHAFT_HEIGHT + 1, boundingBox->z0 - 1, Direction::NORTH, depth);
		if (random->nextBoolean()) generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 - 1, boundingBox->y0 + DEFAULT_SHAFT_HEIGHT + 1, boundingBox->z0 + 1, Direction::WEST, depth);
		if (random->nextBoolean()) generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 + 1, boundingBox->y0 + DEFAULT_SHAFT_HEIGHT + 1, boundingBox->z0 + 1, Direction::EAST, depth);
		if (random->nextBoolean()) generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 + 1, boundingBox->y0 + DEFAULT_SHAFT_HEIGHT + 1, boundingBox->z1 + 1, Direction::SOUTH, depth);
	}
}

bool MoonMineShaftPieces::MoonMineShaftCrossing::postProcess(Level* level, Random* random, BoundingBox* chunkBB)
{
	if (edgesLiquid(level, chunkBB))
	{
		return false;
	}

	// corridor air
	if (isTwoFloored)
	{
		generateBox(level, chunkBB, boundingBox->x0 + 1, boundingBox->y0, boundingBox->z0, boundingBox->x1 - 1, boundingBox->y0 + DEFAULT_SHAFT_HEIGHT - 1, boundingBox->z1, 0, 0, false);
		generateBox(level, chunkBB, boundingBox->x0, boundingBox->y0, boundingBox->z0 + 1, boundingBox->x1, boundingBox->y0 + DEFAULT_SHAFT_HEIGHT - 1, boundingBox->z1 - 1, 0, 0, false);
		generateBox(level, chunkBB, boundingBox->x0 + 1, boundingBox->y1 - (DEFAULT_SHAFT_HEIGHT - 1), boundingBox->z0, boundingBox->x1 - 1, boundingBox->y1, boundingBox->z1, 0, 0, false);
		generateBox(level, chunkBB, boundingBox->x0, boundingBox->y1 - (DEFAULT_SHAFT_HEIGHT - 1), boundingBox->z0 + 1, boundingBox->x1, boundingBox->y1, boundingBox->z1 - 1, 0, 0, false);
		generateBox(level, chunkBB, boundingBox->x0 + 1, boundingBox->y0 + DEFAULT_SHAFT_HEIGHT, boundingBox->z0 + 1, boundingBox->x1 - 1, boundingBox->y0 + DEFAULT_SHAFT_HEIGHT, boundingBox->z1 - 1, 0, 0, false);
	}
	else
	{
		generateBox(level, chunkBB, boundingBox->x0 + 1, boundingBox->y0, boundingBox->z0, boundingBox->x1 - 1, boundingBox->y1, boundingBox->z1, 0, 0, false);
		generateBox(level, chunkBB, boundingBox->x0, boundingBox->y0, boundingBox->z0 + 1, boundingBox->x1, boundingBox->y1, boundingBox->z1 - 1, 0, 0, false);
	}

	// support pillars
	generateBox(level, chunkBB, boundingBox->x0 + 1, boundingBox->y0, boundingBox->z0 + 1, boundingBox->x0 + 1, boundingBox->y1, boundingBox->z0 + 1, Tile::quartzBlock_Id, 0, false);
	generateBox(level, chunkBB, boundingBox->x0 + 1, boundingBox->y0, boundingBox->z1 - 1, boundingBox->x0 + 1, boundingBox->y1, boundingBox->z1 - 1, Tile::quartzBlock_Id, 0, false);
	generateBox(level, chunkBB, boundingBox->x1 - 1, boundingBox->y0, boundingBox->z0 + 1, boundingBox->x1 - 1, boundingBox->y1, boundingBox->z0 + 1, Tile::quartzBlock_Id, 0, false);
	generateBox(level, chunkBB, boundingBox->x1 - 1, boundingBox->y0, boundingBox->z1 - 1, boundingBox->x1 - 1, boundingBox->y1, boundingBox->z1 - 1, Tile::quartzBlock_Id, 0, false);

	// prevent air floating
	// note: use world coordinates because the corridor hasn't defined
	// orientation
	for (int x = boundingBox->x0; x <= boundingBox->x1; x++)
	{
		for (int z = boundingBox->z0; z <= boundingBox->z1; z++)
		{
			int block = getBlock(level, x, boundingBox->y0 - 1, z, chunkBB);
			if (block == 0)
			{
				placeBlock(level, Tile::quartzBlock_Id, 0, x, boundingBox->y0 - 1, z, chunkBB);
			}
		}
	}

	return true;
}

MoonMineShaftPieces::MoonMineShaftStairs::MoonMineShaftStairs()
{
	// for reflection
}

MoonMineShaftPieces::MoonMineShaftStairs::MoonMineShaftStairs(int genDepth, Random* random, BoundingBox* stairsBox, int direction) : StructurePiece(genDepth)
{
	this->orientation = direction;
	boundingBox = stairsBox;
}


void MoonMineShaftPieces::MoonMineShaftStairs::addAdditonalSaveData(CompoundTag* tag)
{
}

void MoonMineShaftPieces::MoonMineShaftStairs::readAdditonalSaveData(CompoundTag* tag)
{
}

BoundingBox* MoonMineShaftPieces::MoonMineShaftStairs::findStairs(list<StructurePiece*>* pieces, Random* random, int footX, int footY, int footZ, int direction)
{
	// stairs are two steps in, 5x5 steps down, two steps out

	BoundingBox* box = new BoundingBox(footX, footY - 5, footZ, footX, footY + (DEFAULT_SHAFT_HEIGHT - 1), footZ);

	switch (direction)
	{
	case Direction::NORTH:
		box->x1 = footX + (DEFAULT_SHAFT_WIDTH - 1);
		box->z0 = footZ - 8;
		break;
	case Direction::SOUTH:
		box->x1 = footX + (DEFAULT_SHAFT_WIDTH - 1);
		box->z1 = footZ + 8;
		break;
	case Direction::WEST:
		box->x0 = footX - 8;
		box->z1 = footZ + (DEFAULT_SHAFT_WIDTH - 1);
		break;
	case Direction::EAST:
		box->x1 = footX + 8;
		box->z1 = footZ + (DEFAULT_SHAFT_WIDTH - 1);
		break;
	}

	if (StructurePiece::findCollisionPiece(pieces, box) != nullptr)
	{
		delete box;
		return nullptr;
	}

	return box;
}

void MoonMineShaftPieces::MoonMineShaftStairs::addChildren(StructurePiece* startPiece, list<StructurePiece*>* pieces, Random* random)
{
	int depth = getGenDepth();
	// crossings are coming from a direction and will generate children
	// in the
	// remaining three directions
	switch (orientation)
	{
	case Direction::NORTH:
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0, boundingBox->y0, boundingBox->z0 - 1, Direction::NORTH, depth);
		break;
	case Direction::SOUTH:
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0, boundingBox->y0, boundingBox->z1 + 1, Direction::SOUTH, depth);
		break;
	case Direction::WEST:
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x0 - 1, boundingBox->y0, boundingBox->z0, Direction::WEST, depth);
		break;
	case Direction::EAST:
		generateAndAddPiece(startPiece, pieces, random, boundingBox->x1 + 1, boundingBox->y0, boundingBox->z0, Direction::EAST, depth);
		break;
	}

}

bool MoonMineShaftPieces::MoonMineShaftStairs::postProcess(Level* level, Random* random, BoundingBox* chunkBB)
{

	if (edgesLiquid(level, chunkBB))
	{
		return false;
	}

	// upper floor
	generateBox(level, chunkBB, 0, 5, 0, (DEFAULT_SHAFT_WIDTH - 1), 5 + (DEFAULT_SHAFT_HEIGHT - 1), 1, 0, 0, false);
	// lower floor
	generateBox(level, chunkBB, 0, 0, 7, (DEFAULT_SHAFT_WIDTH - 1), (DEFAULT_SHAFT_HEIGHT - 1), 8, 0, 0, false);
	// stairs
	for (int i = 0; i < 5; i++)
	{
		generateBox(level, chunkBB, 0, 5 - i - ((i < 4) ? 1 : 0), 2 + i, (DEFAULT_SHAFT_WIDTH - 1), 5 + (DEFAULT_SHAFT_HEIGHT - 1) - i, 2 + i, 0, 0, false);
	}

	return true;
}