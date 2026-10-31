#include "stdafx.h"
#include "Container.h"
#include "AbstractContainerMenu.h"
#include "CraftingContainer.h"
#include "CoalItem.h"
#include "Item.h"
#include "ItemInstance.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.item.crafting.h"

SpaceRecipes* SpaceRecipes::instance = NULL;

void SpaceRecipes::staticCtor()
{
	SpaceRecipes::instance = new SpaceRecipes();

}

void SpaceRecipes::_init()
{
	// 4J Jev: instance = new SpaceRecipes();
	recipies = new SpaceRecipyList();
}

SpaceRecipes::SpaceRecipes()
{
	int iCount = 0;
	_init();

	// 4J Stu - These just don't work with our crafting menu
	//recipies->push_back(new ArmorDyeRecipe());
	//recipies->add(new MapCloningRecipe());
	//recipies->add(new MapExtendingRecipe());
	//recipies->add(new FireworksRecipe());

	addShapedRecipy(new ItemInstance(Item::rocket, 1), //
		L"ssscicicicig",
		L"HNH", //
		L"HHH", //
		L"FEF", //

		L'H', Item::compressedHeavyDuty, L'N', Item::noseCone, L'F', Item::rocketFins, L'E', Item::rocketEngine,
		L'R');

	addShapedRecipy(new ItemInstance(Item::rocketFins, 1), //
		L"ssscicig",
		L" S ", //
		L"HSH", //
		L"H H", //

		L'S', Item::compressedSteel, L'H', Item::compressedHeavyDuty,
		L'R');

	addShapedRecipy(new ItemInstance(Item::noseCone, 1), //
		L"sssctcig",
		L" R ", //
		L" H ", //
		L"H H", //

		L'R', Tile::redstoneTorch_on, L'H', Item::compressedHeavyDuty,
		L'R');

	addShapedRecipy(new ItemInstance(Item::rocketEngine, 1), //
		L"sssctcicicig",
		L" FB", //
		L"HTH", //
		L"HHH", //

		L'B', Tile::button, L'F', Item::flintAndSteel, L'T', Item::tinCanister, L'H', Item::compressedHeavyDuty,
		L'R');

	addShapedRecipy(new ItemInstance(Item::buggy, 1), //
		L"ssscicicicig",
		L"WHW", //
		L"HSH", //
		L"WBW", //

		L'H', Item::compressedHeavyDuty, L'W', Item::buggyWheel, L'S', Item::buggySeat, L'B', Item::buggyBlueprint,
		L'R');

	addShapedRecipy(new ItemInstance(Item::buggySeat, 1), //
		L"ssscicig",
		L"  S", //
		L" IS", //
		L"SSS", //

		L'S', Item::compressedSteel, L'I', Item::compressedIron,
		L'R');

	addShapedRecipy(new ItemInstance(Item::buggyWheel, 1), //
		L"ssscicig",
		L" L ", //
		L"LSL", //
		L" L ", //

		L'S', Item::compressedSteel, L'L', Item::leather,
		L'R');

	addShapedRecipy(new ItemInstance(Item::buggyStorage, 1), //
		L"ssscicictg",
		L"SSS", //
		L"ICI", //
		L"SSS", //

		L'S', Item::compressedSteel, L'I', Item::compressedIron, L'C', Tile::chest,
		L'R');

	addShapedRecipy(new ItemInstance(Item::compressedIron, 1), //
		L"scig",
		L"II ",
		L'I', Item::ironIngot,
		L'C');

	addShapedRecipy(new ItemInstance(Item::compressedCopper, 1), //
		L"scig",
		L"II ",
		L'I', Item::copperIngot,
		L'C');

	addShapedRecipy(new ItemInstance(Item::compressedTin, 1), //
		L"scig",
		L"II ",
		L'I', Item::tinIngot,
		L'C');

	addShapedRecipy(new ItemInstance(Item::compressedAluminium, 1), //
		L"scig",
		L"II ",
		L'I', Item::aluminiumIngot,
		L'C');

	addShapedRecipy(new ItemInstance(Item::compressedSteel, 1), //
		L"scicig",
		L"CIC", 

		L'I', Item::compressedIron, L'C', Item::coal,
		L'C');

	addShapedRecipy(new ItemInstance(Item::compressedSteel, 1), //
		L"sciczg",
		L"CIC",

		L'I', Item::compressedIron, L'C', new ItemInstance(Item::coal, 1, CoalItem::CHAR_COAL),
		L'C');

	addShapedRecipy(new ItemInstance(Item::compressedBronze, 1), //
		L"scicig",
		L"CT",

		L'T', Item::compressedTin, L'C', Item::compressedCopper,
		L'C');

	addShapedRecipy(new ItemInstance(Item::compressedHeavyDuty, 2), //
		L"sscicicig",
		L"SAB",
		L"SAB",

		L'S', Item::compressedSteel, L'A', Item::compressedAluminium, L'B', Item::compressedBronze,
		L'C');

	addShapedRecipy(new ItemInstance(Item::frequencyModule, 1), //
		L"ssscicicicig",
		L" I ",
		L"IMI",
		L"RWR",

		L'I', Item::compressedIron, L'R', Item::redStone, L'W', Item::basicWafer, L'M', Item::repeater,
		L'A');

	addShapedRecipy(new ItemInstance(Item::oxygenMask, 1), //
		L"sssctcig",
		L"PPP",
		L"PIP",
		L"PPP",

		L'P', Tile::thinGlass, L'I', Item::helmet_iron,
		L'A');

	addShapedRecipy(new ItemInstance(Item::oxygenGear, 1), //
		L"ssscicictcig",
		L"SIS",
		L"PTP",
		L"P P",

		L'S', Item::compressedSteel, L'I', Item::compressedIron, L'P', Tile::thinGlass, L'T', Item::tinCanister,
		L'A');

	addShapedRecipy(new ItemInstance(Item::oxygenTank, 1), //
		L"sssciczcig",
		L"RRR",
		L"TTT",
		L"SSS",

		L'S', Item::compressedSteel, L'R', new ItemInstance(Tile::wool, 1, 14), L'T', Item::tinCanister,
		L'A');

	addShapedRecipy(new ItemInstance(Item::basicWafer, 3), //
		L"sscicictcig",
		L"DST",
		L" SR",

		L'S', Item::rawSilicon, L'R', Item::redStone, L'T', Tile::redstoneTorch_on, L'D', Item::diamond,
		L'M');

	addShapedRecipy(new ItemInstance(Item::advancedWafer, 1), //
		L"sscicicicig",
		L"DST",
		L" SR",

		L'S', Item::rawSilicon, L'R', Item::redStone, L'T', Item::repeater, L'D', Item::diamond,
		L'M');

	addShapedRecipy(new ItemInstance(Item::tinCanister, 2), //
		L"ssscig",
		L"T T",
		L"T T",
		L"TTT",

		L'T', Item::tinIngot,
		L'M');

	addShapedRecipy(new ItemInstance(Item::dehydratedApple, 1), //
		L"sscicig",
		L"CF ",
		L"F  ",

		L'C', Item::tinCanister, L'F', Item::apple,
		L'M');

	addShapedRecipy(new ItemInstance(Item::dehydratedCarrot, 1), //
		L"sscicig",
		L"CF ",
		L"F  ",

		L'C', Item::tinCanister, L'F', Item::carrots,
		L'M');

	addShapedRecipy(new ItemInstance(Item::dehydratedMelon, 1), //
		L"sscicig",
		L"CF ",
		L"F  ",

		L'C', Item::tinCanister, L'F', Item::melon,
		L'M');

	addShapedRecipy(new ItemInstance(Item::dehydratedPotato, 1), //
		L"sscicig",
		L"CF ",
		L"F  ",

		L'C', Item::tinCanister, L'F', Item::potato,
		L'M');

	addShapedRecipy(new ItemInstance(Item::cannedBeef, 1), //
		L"sscicig",
		L"CF ",
		L"F  ",

		L'C', Item::tinCanister, L'F', Item::beef_raw,
		L'M');

	addShapedRecipy(new ItemInstance(Item::fuelCanister, 1), //
		L"ssscictcicig",
		L" O ",
		L"TSS",
		L" C ",

		L'O', Item::oilBucket, L'T', Tile::redstoneTorch_on, L'S', Item::compressedSteel, L'C', Item::tinCanister,
		L'M');

	// Sort so the largest recipes get checked first!
	/* 4J-PB - TODO
	Collections.sort(recipies, new Comparator<SpaceRecipy>()
	{
	public: int compare(SpaceRecipy r0, SpaceRecipy r1)
			{

				// shapeless recipes are put in the back of the list
				if (r0 instanceof SpaceShapelessRecipy && r1 instanceof SpaceShapedRecipy)
				{
					return 1;
				}
				if (r1 instanceof SpaceShapelessRecipy && r0 instanceof SpaceShapedRecipy)
				{
					return -1;
				}

				if (r1.size() < r0.size()) return -1;
				if (r1.size() > r0.size()) return 1;
				return 0;
			}
	});
	*/

	// 4J-PB removed System.out.println(recipies->size() + L" recipes");

	// 4J-PB - build the array of ingredients required per recipe
	buildRecipeIngredientsArray();
}

// 4J-PB - this function has been substantially changed due to the differences with a va_list of classes in C++ and Java
SpaceShapedRecipy* SpaceRecipes::addShapedRecipy(ItemInstance* result, ...)
{
	wstring map = L"";
	int p = 0;
	int width = 0;
	int height = 0;
	int group = SpaceShapedRecipy::eGroupType_Space_Misc;
	va_list vl;
	wchar_t* wchTypes;
	wchar_t* pwchString;
	wstring wString;
	wstring* wStringA;
	ItemInstance* pItemInstance;
	Tile* pTile;
	Item* pItem;
	wchar_t wchFrom;
	int iCount;
	ItemInstance** ids = NULL;

	myMap* mappings = new unordered_map<wchar_t, ItemInstance*>();

	va_start(vl, result);
	// 4J-PB - second argument is a list of the types
	// s - string
	// w - string array
	// a - char *
	// c - char
	// z - ItemInstance *
	// i - Item *
	// t - Tile *
	// g - group [wt] - which group does the item created by the recipe belong in. Set a default until all recipes have a group

	wchTypes = va_arg(vl, wchar_t*);

	for (int i = 0; wchTypes[i] != L'\0'; ++i)
	{
		if (wchTypes[i + 1] == L'\0' && wchTypes[i] != L'g')
		{
			app.DebugPrintf("Missing group type\n");
		}

		switch (wchTypes[i])
		{
		case L'a':
			pwchString = va_arg(vl, wchar_t*);
			wString = pwchString;
			height++;
			width = static_cast<int>(wString.length());
			map += wString;
			break;
		case L's':
			pwchString = va_arg(vl, wchar_t*);
			wString = pwchString;
			height++;
			width = static_cast<int>(wString.length());
			map += wString;
			break;
		case L'w':
			wStringA = va_arg(vl, wstring*);
			iCount = 0;
			do
			{
				wString = wStringA[iCount++];
				if (!wString.empty())
				{
					height++;
					width = static_cast<int>(wString.length());
					map += wString;
				}
			} while (!wString.empty());

			break;
		case L'c':
			wchFrom = va_arg(vl, wchar_t);
			break;
		case L'z':
			pItemInstance = va_arg(vl, ItemInstance*);
			mappings->insert(myMap::value_type(wchFrom, pItemInstance));
			break;
		case L'i':
			pItem = va_arg(vl, Item*);
			pItemInstance = new ItemInstance(pItem, 1, ANY_AUX_VALUE);
			mappings->insert(myMap::value_type(wchFrom, pItemInstance));
			break;
		case L't':
			pTile = va_arg(vl, Tile*);
			pItemInstance = new ItemInstance(pTile, 1, ANY_AUX_VALUE);
			mappings->insert(myMap::value_type(wchFrom, pItemInstance));
			break;
		case L'g':
			wchFrom = va_arg(vl, wchar_t);
			switch (wchFrom)
			{
				// 			case L'W':
				// 				group=ShapedRecipy::eGroupType_Weapon;
				// 				break;
			case L'R':
				group = SpaceRecipy::eGroupType_Rocket;
				break;
			case L'C':
				group = SpaceRecipy::eGroupType_Compressed;
				break;
			case L'A':
				group = SpaceRecipy::eGroupType_Accessory;
				break;
			case L'M':
			default:
				group = SpaceRecipy::eGroupType_Space_Misc;
				break;

			}
			break;

		}


		ids = new ItemInstance * [width * height];

		for (int j = 0; j < width * height; j++)
		{
			wchar_t ch = map[j];
			myMap::iterator it = mappings->find(ch);
			if (it != mappings->end())
			{
				ids[j] = it->second;
			}
			else
			{
				ids[j] = NULL;
			}
		}
	}

	va_end(vl);

	SpaceShapedRecipy* recipe = new SpaceShapedRecipy(width, height, ids, result, group);
	recipies->push_back(recipe);
	return recipe;
}

/*void SpaceRecipes::addShapelessRecipy(ItemInstance* result, ...)
{
	va_list vl;
	wchar_t* szTypes;
	wstring String;
	ItemInstance* pItemInstance;
	Tile* pTile;
	Item* pItem;
	SpaceRecipy::_eGroupType group = SpaceRecipy::eGroupType_Space_Misc;
	wchar_t wchFrom;
	vector <ItemInstance*>* ingredients = new vector<ItemInstance*>();

	va_start(vl, result);
	// 4J-PB - second argument is a list of the types
	// z - ItemInstance *
	// i - Item *
	// t - Tile *
	szTypes = va_arg(vl, wchar_t*);

	for (int i = 0; szTypes[i] != L'\0'; ++i)
	{
		switch (szTypes[i])
		{
		case L'z':
			pItemInstance = va_arg(vl, ItemInstance*);
			// 4J-PB - original code copies the item instance, copy the pointer isnt the same...
			// TODO
			ingredients->push_back(pItemInstance->copy_not_shared());
			break;
		case L'i':
			pItem = va_arg(vl, Item*);
			pItemInstance = new ItemInstance(pItem);
			ingredients->push_back(pItemInstance);
			break;
		case L't':
			pTile = va_arg(vl, Tile*);
			ingredients->push_back(new ItemInstance(pTile));
			break;
		case L'g':
			wchFrom = va_arg(vl, wchar_t);
			switch (wchFrom)
			{

			case L'R':
				group = SpaceRecipy::eGroupType_Rocket;
				break;
			case L'C':
				group = SpaceRecipy::eGroupType_Compressed;
				break;
			case L'A':
				group = SpaceRecipy::eGroupType_Accessory;
				break;
			case L'M':
			default:
				group = SpaceRecipy::eGroupType_Space_Misc;
				break;

			}
			break;
		}
	}

	recipies->push_back(new SpaceShapelessRecipy(result, ingredients, group));
}*/

shared_ptr<ItemInstance> SpaceRecipes::getItemFor(shared_ptr<CraftingContainer> craftSlots, Level* level, SpaceRecipy* recipesClass /*= nullptr*/)
{
	int count = 0;
	shared_ptr<ItemInstance> first = nullptr;
	shared_ptr<ItemInstance> second = nullptr;
	for (int i = 0; i < craftSlots->getContainerSize(); i++)
	{
		shared_ptr<ItemInstance> item = craftSlots->getItem(i);
		if (item != NULL)
		{
			if (count == 0) first = item;
			if (count == 1) second = item;
			count++;
		}
	}

	if (count == 2 && first->id == second->id && first->count == 1 && second->count == 1 && Item::items[first->id]->canBeDepleted())
	{
		Item* item = Item::items[first->id];
		int remaining1 = item->getMaxDamage() - first->getDamageValue();
		int remaining2 = item->getMaxDamage() - second->getDamageValue();
		int remaining = (remaining1 + remaining2) + item->getMaxDamage() * 5 / 100;
		int resultDamage = item->getMaxDamage() - remaining;
		if (resultDamage < 0) resultDamage = 0;
		return std::make_shared<ItemInstance>(first->id, 1, resultDamage);
	}

	if (recipesClass != NULL)
	{
		if (recipesClass->matches(craftSlots, level)) return recipesClass->assemble(craftSlots);
	}
	else
	{
		AUTO_VAR(itEnd, recipies->end());
		for (AUTO_VAR(it, recipies->begin()); it != itEnd; it++)
		{
			SpaceRecipy *r = *it; 
			if (r->matches(craftSlots, level)) return r->assemble(craftSlots);
		}
	}
	return nullptr;
}

vector <SpaceRecipy*>* SpaceRecipes::getRecipies()
{
	return recipies;
}

// 4J-PB - added to deal with Xb0x 'crafting'
shared_ptr<ItemInstance> SpaceRecipes::getItemForRecipe(SpaceRecipy* r)
{
	return r->assemble(nullptr);
}

// 4J-PB - build the required ingredients for SpaceRecipes
void SpaceRecipes::buildRecipeIngredientsArray(void)
{
	//RecipyList *SpaceRecipes = ((Recipes *)Recipes::getInstance())->getRecipies();

	int iRecipeC = static_cast<int>(recipies->size());

	m_pRecipeIngredientsRequired = new SpaceRecipy::INGREDIENTS_REQUIRED[iRecipeC];

	int iCount = 0;
	AUTO_VAR(itEndRec, recipies->end());
	for (AUTO_VAR(it, recipies->begin()); it != itEndRec; it++)
	{
		SpaceRecipy *recipe = *it; 
		recipe->requires(&m_pRecipeIngredientsRequired[iCount++]);
	}

	//printf("Total recipes in buildRecipeIngredientsArray - %d",iCount);
}

SpaceRecipy::INGREDIENTS_REQUIRED* SpaceRecipes::getRecipeIngredientsArray(void)
{
	return m_pRecipeIngredientsRequired;
}