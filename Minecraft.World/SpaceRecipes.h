/*package net.minecraft.world.item.crafting;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.minecraft.world.inventory.CraftingContainer;
import net.minecraft.world.item.CoalItem;
import net.minecraft.world.item.Item;
import net.minecraft.world.item.ItemInstance;
import net.minecraft.world.level.tile.StoneSlabTile;
import net.minecraft.world.level.tile.Tile;
*/

#include "SpaceRecipy.h"

#pragma once
using namespace std;

class CraftingContainer;
class FireTile;

class SpaceShapedRecipy;

typedef unordered_map<wchar_t, ItemInstance*> myMap;

#define ADD_OBJECT(a,b) a.push_back(new SpaceObject(b))

class SpaceObject
{
public:
	union
	{
		Tile* tile;
		FireTile* firetile;
		Item* item;
		MapItem* mapitem;
		ItemInstance* iteminstance;
	};

	SpaceObject() { eType = eTYPE_NOTSET; }
	SpaceObject(Tile* t) { eType = eType_TILE; tile = t; }
	SpaceObject(FireTile* t) { eType = eType_FIRETILE; firetile = t; }
	SpaceObject(Item* i) { eType = eType_ITEM; item = i; }
	SpaceObject(MapItem* i) { eType = eType_MAPITEM; mapitem = i; }
	SpaceObject(ItemInstance* i) { eType = eType_ITEMINSTANCE; iteminstance = i; }

	eINSTANCEOF instanceof() { return eType; }
	eINSTANCEOF GetType() { return eType; };

private:
	eINSTANCEOF eType;
};

class SpaceRecipes
{
public:
	static const int ANY_AUX_VALUE = -1;

private:
	static SpaceRecipes* instance;

	vector <SpaceRecipy*>* recipies;

public:
	static void staticCtor();

public:
	static SpaceRecipes* getInstance()
	{
		return instance;
	}

private:
	void _init(); // 4J add
	SpaceRecipes();

public:
	SpaceShapedRecipy* addShapedRecipy(ItemInstance*, ...);
	//void addShapelessRecipy(ItemInstance* result, ...);

	shared_ptr<ItemInstance> getItemFor(shared_ptr<CraftingContainer> craftSlots, Level* level, SpaceRecipy* recipesClass = NULL); // 4J Added recipesClass param
	vector <SpaceRecipy*>* getRecipies();

	// 4J-PB - Added all below for new Xbox 'crafting'
	shared_ptr<ItemInstance> getItemForRecipe(SpaceRecipy* r);
	SpaceRecipy::INGREDIENTS_REQUIRED* getRecipeIngredientsArray();

private:
	void buildRecipeIngredientsArray();
	SpaceRecipy::INGREDIENTS_REQUIRED* m_pRecipeIngredientsRequired;
};