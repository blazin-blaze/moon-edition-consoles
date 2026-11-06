#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.ai.village.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.effect.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.item.enchantment.h"
#include "net.minecraft.world.item.trading.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.h"
#include "..\Minecraft.Client\Textures.h"
#include "AlienVillager.h"
#include "AbstractContainerMenu.h"

unordered_map<int, pair<int, int> > AlienVillager::MIN_MAX_VALUES;
unordered_map<int, pair<int, int> > AlienVillager::MIN_MAX_PRICES;

void AlienVillager::_init(int profession)
{
	// 4J Stu - This function call had to be moved here from the Entity ctor to ensure that
	// the derived version of the function is called
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	setProfession(profession);
	setSize(.6f, 1.8f);

	villageUpdateInterval = 0;
	inLove = false;
	chasing = false;
	village = weak_ptr<Village>();

	tradingPlayer = weak_ptr<Player>();
	offers = NULL;
	updateMerchantTimer = 0;
	addRecipeOnUpdate = false;
	riches = 0;
	lastPlayerTradeName = L"";
	rewardPlayersOnFirstVillage = false;
	baseRecipeChanceMod = 0.0f;

	getNavigation()->setCanOpenDoors(true);
	getNavigation()->setAvoidWater(true);

	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(1, new AvoidPlayerGoal(this, typeid(Zombie), 8, 0.6, 0.6));
	goalSelector.addGoal(1, new TradeWithPlayerGoal(this));
	goalSelector.addGoal(1, new LookAtTradingPlayerGoal(this));
	goalSelector.addGoal(2, new MoveIndoorsGoal(this));
	goalSelector.addGoal(3, new RestrictOpenDoorGoal(this));
	goalSelector.addGoal(4, new OpenDoorGoal(this, true));
	goalSelector.addGoal(5, new MoveTowardsRestrictionGoal(this, 0.6));
	goalSelector.addGoal(9, new InteractGoal(this, typeid(Player), 3, 1.f));
	goalSelector.addGoal(9, new InteractGoal(this, typeid(AlienVillager), 5, 0.02f));
	goalSelector.addGoal(9, new RandomStrollGoal(this, 0.6));
	goalSelector.addGoal(10, new LookAtPlayerGoal(this, typeid(Mob), 8));
}

AlienVillager::AlienVillager(Level* level) : AgableMob(level)
{
	_init(0);
}

AlienVillager::AlienVillager(Level* level, int profession) : AgableMob(level)
{
	_init(profession);
}

AlienVillager::~AlienVillager()
{
	delete offers;
}

void AlienVillager::registerAttributes()
{
	AgableMob::registerAttributes();

	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.5f);
}

bool AlienVillager::useNewAi()
{
	return true;
}

void AlienVillager::serverAiMobStep()
{
	if (--villageUpdateInterval <= 0)
	{
		level->villages->queryUpdateAround(Mth::floor(x), Mth::floor(y), Mth::floor(z));
		villageUpdateInterval = 70 + random->nextInt(50);

		shared_ptr<Village> _village = level->villages->getClosestVillage(Mth::floor(x), Mth::floor(y), Mth::floor(z), Villages::MaxDoorDist);
		village = _village;
		if (_village == NULL) clearRestriction();
		else
		{
			Pos* center = _village->getCenter();
			restrictTo(center->x, center->y, center->z, static_cast<int>((float)_village->getRadius() * 0.6f));
			if (rewardPlayersOnFirstVillage)
			{
				rewardPlayersOnFirstVillage = false;
				_village->rewardAllPlayers(5);
			}
		}
	}

	if (!isTrading() && updateMerchantTimer > 0)
	{
		updateMerchantTimer--;
		if (updateMerchantTimer <= 0)
		{
			if (addRecipeOnUpdate)
			{
				// improve max uses for all obsolete recipes
				if (offers->size() > 0)
				{
					//for (MerchantRecipe recipe : offers)
					for (vector<MerchantRecipe *>::iterator it = offers->begin(); it != offers->end(); ++it)
					{
						MerchantRecipe* recipe = *it;
						if (recipe->isDeprecated())
						{
							recipe->increaseMaxUses(random->nextInt(6) + random->nextInt(6) + 2);
						}
					}
				}
				addOffers(1);
				addRecipeOnUpdate = false;

				if (village.lock() != NULL && !lastPlayerTradeName.empty())
				{
					level->broadcastEntityEvent(shared_from_this(), EntityEvent::VILLAGER_HAPPY);
					village.lock()->modifyStanding(lastPlayerTradeName, 1);
				}
			}
			addEffect(new MobEffectInstance(MobEffect::regeneration->id, SharedConstants::TICKS_PER_SECOND * 10, 0));
		}
	}

	AgableMob::serverAiMobStep();
}

bool AlienVillager::mobInteract(shared_ptr<Player> player)
{
	// [EB]: Truly dislike this code but I don't see another easy way
	shared_ptr<ItemInstance> item = player->inventory->getSelected();
	bool hasFreqMod = player->inventory->getFrequencyModule();
	bool holdingSpawnEgg = item != NULL && item->id == Item::spawnEgg_Id;

	if (!holdingSpawnEgg && isAlive() && !isTrading() && !isBaby() && hasFreqMod)
	{
		if (!level->isClientSide)
		{
			// note: stop() logic is controlled by trading ai goal
			setTradingPlayer(player);

			// 4J-JEV: AlienVillagers in PC game don't display professions.
			player->openTrading(dynamic_pointer_cast<Merchant>(shared_from_this()), getDisplayName());
		}
		return true;
	}
	if (!hasFreqMod) {
		player->sendMessage(L"You need a Frequency Module to talk to Alien Villagers!");
	}
	return AgableMob::mobInteract(player);
}

void AlienVillager::defineSynchedData()
{
	AgableMob::defineSynchedData();
	entityData->define(DATA_PROFESSION_ID, 0);
}

void AlienVillager::addAdditonalSaveData(CompoundTag* tag)
{
	AgableMob::addAdditonalSaveData(tag);
	tag->putInt(L"Profession", getProfession());
	tag->putInt(L"Riches", riches);
	if (offers != NULL)
	{
		tag->putCompound(L"Offers", offers->createTag());
	}
}

void AlienVillager::readAdditionalSaveData(CompoundTag* tag)
{
	AgableMob::readAdditionalSaveData(tag);
	setProfession(tag->getInt(L"Profession"));
	riches = tag->getInt(L"Riches");
	if (tag->contains(L"Offers"))
	{
		CompoundTag* compound = tag->getCompound(L"Offers");
		delete offers;
		offers = new MerchantRecipeList(compound);
	}
}

bool AlienVillager::removeWhenFarAway()
{
	return false;
}

int AlienVillager::getAmbientSound()
{
	if (isTrading())
	{
		return eSoundType_MOB_VILLAGER_HAGGLE;
	}
	return eSoundType_MOB_VILLAGER_IDLE;
}

int AlienVillager::getHurtSound()
{
	return eSoundType_MOB_VILLAGER_HIT;
}

int AlienVillager::getDeathSound()
{
	return eSoundType_MOB_VILLAGER_DEATH;
}

void AlienVillager::setProfession(int profession)
{
	entityData->set(DATA_PROFESSION_ID, profession);
}

int AlienVillager::getProfession()
{
	return entityData->getInteger(DATA_PROFESSION_ID);
}

bool AlienVillager::isInLove()
{
	return inLove;
}

void AlienVillager::setInLove(bool inLove)
{
	this->inLove = inLove;
}

void AlienVillager::setChasing(bool chasing)
{
	this->chasing = chasing;
}

bool AlienVillager::isChasing()
{
	return chasing;
}

void AlienVillager::setLastHurtByMob(shared_ptr<LivingEntity> mob)
{
	AgableMob::setLastHurtByMob(mob);
	shared_ptr<Village> _village = village.lock();
	if (_village != NULL && mob != NULL)
	{
		_village->addAggressor(mob);

		if (mob->instanceof(eTYPE_PLAYER))
		{
			int amount = -1;
			if (isBaby())
			{
				amount = -3;
			}
			_village->modifyStanding(dynamic_pointer_cast<Player>(mob)->getName(), amount);
			if (isAlive())
			{
				level->broadcastEntityEvent(shared_from_this(), EntityEvent::VILLAGER_ANGRY);
			}
		}
	}
}

void AlienVillager::die(DamageSource* source)
{
	shared_ptr<Village> _village = village.lock();
	if (_village != NULL)
	{
		shared_ptr<Entity> sourceEntity = source->getEntity();
		if (sourceEntity != NULL)
		{
			if (sourceEntity->instanceof(eTYPE_PLAYER))
			{
				_village->modifyStanding(dynamic_pointer_cast<Player>(sourceEntity)->getName(), -2);
			}
			else if (sourceEntity->instanceof(eTYPE_ENEMY))
			{
				_village->resetNoBreedTimer();
			}
		}
		else if (sourceEntity == NULL)
		{
			// if the AlienVillager was killed by the world (such as lava or falling), blame
			// the nearest player by not reproducing for a while
			shared_ptr<Player> nearestPlayer = level->getNearestPlayer(shared_from_this(), 16.0f);
			if (nearestPlayer != NULL)
			{
				_village->resetNoBreedTimer();
			}
		}
	}

	// Make the gui close if the AlienVillager die while trading
	if (AUTO_VAR(currentTrader, tradingPlayer.lock())) 
	{
		if (currentTrader->containerMenu != NULL)
		{
			AUTO_VAR(menu, currentTrader->containerMenu);
			menu->removed(currentTrader);
		}

		tradingPlayer.reset();
	}

	AgableMob::die(source);
}

void AlienVillager::setTradingPlayer(shared_ptr<Player> player)
{
	tradingPlayer = weak_ptr<Player>(player);
}

shared_ptr<Player> AlienVillager::getTradingPlayer()
{
	return tradingPlayer.lock();
}

bool AlienVillager::isTrading()
{
	return tradingPlayer.lock() != NULL;
}

void AlienVillager::notifyTrade(MerchantRecipe* activeRecipe)
{
	activeRecipe->increaseUses();
	ambientSoundTime = -getAmbientSoundInterval();
	playSound(eSoundType_MOB_VILLAGER_YES, getSoundVolume(), getVoicePitch());

	// when the player buys the latest item, we improve the merchant a little while later
	if (activeRecipe->isSame(offers->at(offers->size() - 1)))
	{
		updateMerchantTimer = SharedConstants::TICKS_PER_SECOND * 2;
		addRecipeOnUpdate = true;
		if (tradingPlayer.lock() != NULL)
		{
			lastPlayerTradeName = tradingPlayer.lock()->getName();
		}
		else
		{
			lastPlayerTradeName = L"";
		}
	}

	if (activeRecipe->getBuyAItem()->id == Item::sapphire_Id)
	{
		riches += activeRecipe->getBuyAItem()->count;
	}
}

void AlienVillager::notifyTradeUpdated(shared_ptr<ItemInstance> item)
{
	if (!level->isClientSide && (ambientSoundTime > (-getAmbientSoundInterval() + SharedConstants::TICKS_PER_SECOND)))
	{
		ambientSoundTime = -getAmbientSoundInterval();
		if (item != NULL)
		{
			playSound(eSoundType_MOB_VILLAGER_YES, getSoundVolume(), getVoicePitch());
		}
		else
		{
			playSound(eSoundType_MOB_VILLAGER_NO, getSoundVolume(), getVoicePitch());
		}
	}
}

MerchantRecipeList* AlienVillager::getOffers(shared_ptr<Player> forPlayer)
{
	if (offers == NULL)
	{
		addOffers(1);
	}
	return offers;
}

float AlienVillager::getRecipeChance(float baseChance)
{
	float newChance = baseChance + baseRecipeChanceMod;
	if (newChance > .9f)
	{
		return .9f - (newChance - .9f);
	}
	return newChance;
}

void AlienVillager::addOffers(int addCount)
{
	MerchantRecipeList* newOffers = new MerchantRecipeList();
	switch (getProfession())
	{
	case PROFESSION_BUTCHER:
		addItemForTradeIn(newOffers, Item::wheat_Id, random, getRecipeChance(.7f));
		addItemForTradeIn(newOffers, Item::cheese_Id, random, getRecipeChance(.9f));
		addItemForTradeIn(newOffers, Item::tinCanister_Id, random, getRecipeChance(.7f));

		addItemForPurchase(newOffers, Item::dehydratedApple_Id, random, getRecipeChance(.9f));
		addItemForPurchase(newOffers, Item::dehydratedCarrot_Id, random, getRecipeChance(.9f));
		addItemForPurchase(newOffers, Item::dehydratedMelon_Id, random, getRecipeChance(.9f));
		addItemForPurchase(newOffers, Item::dehydratedPotato_Id, random, getRecipeChance(.9f));
		addItemForPurchase(newOffers, Item::cannedBeef_Id, random, getRecipeChance(.9f));
		addItemForPurchase(newOffers, Item::cheese_Id, random, getRecipeChance(.9f));
		addItemForPurchase(newOffers, Item::bread_Id, random, getRecipeChance(.9f));
		addItemForPurchase(newOffers, Item::cheeseburger_Id, random, getRecipeChance(.5f));
		addItemForPurchase(newOffers, Item::poutine_Id, random, getRecipeChance(.5f));
		addItemForPurchase(newOffers, Tile::cheeseBlock_Id, random, getRecipeChance(.3f));
		addItemForPurchase(newOffers, Item::carrotGolden_Id, random, getRecipeChance(.3f));
		addItemForPurchase(newOffers, Item::apple_gold_Id, random, getRecipeChance(.3f));
		addItemForPurchase(newOffers, Item::cake_Id, random, getRecipeChance(.5f));
		
		break;
	case PROFESSION_SMITH:
		addItemForTradeIn(newOffers, Item::netherQuartz_Id, random, getRecipeChance(.7f));
		addItemForTradeIn(newOffers, Item::ironIngot_Id, random, getRecipeChance(.5f));
		addItemForTradeIn(newOffers, Item::copperIngot_Id, random, getRecipeChance(.5f));
		addItemForTradeIn(newOffers, Item::aluminiumIngot_Id, random, getRecipeChance(.5f));
		addItemForTradeIn(newOffers, Item::tinIngot_Id, random, getRecipeChance(.5f));

		addItemForPurchase(newOffers, Item::moonDungeonKey_Id, random, getRecipeChance(.7f));
		addItemForPurchase(newOffers, Item::buggyBlueprint_Id, random, getRecipeChance(.5f));
		addItemForPurchase(newOffers, Item::regulatorBlueprint_Id, random, getRecipeChance(.5f));
		addItemForPurchase(newOffers, Item::titaniumSword_Id, random, getRecipeChance(.1f));
		addItemForPurchase(newOffers, Item::titaniumHatchet_Id, random, getRecipeChance(.1f));
		addItemForPurchase(newOffers, Item::titaniumPickaxe_Id, random, getRecipeChance(.1f));
		addItemForPurchase(newOffers, Item::titaniumShovel_Id, random, getRecipeChance(.1f));
		addItemForPurchase(newOffers, Item::titaniumHoe_Id, random, getRecipeChance(.1f));
		addItemForPurchase(newOffers, Item::titaniumChestplate_Id, random, getRecipeChance(.1f));
		addItemForPurchase(newOffers, Item::titaniumHelmet_Id, random, getRecipeChance(.1f));
		addItemForPurchase(newOffers, Item::titaniumBoots_Id, random, getRecipeChance(.1f));
		addItemForPurchase(newOffers, Item::titaniumLeggings_Id, random, getRecipeChance(.1f));

		break;
	case PROFESSION_TINKERER:
		addItemForTradeIn(newOffers, Item::rawSilicon_Id, random, getRecipeChance(.7f));
		addItemForTradeIn(newOffers, Item::compressedCopper_Id, random, getRecipeChance(.7f));
		addItemForTradeIn(newOffers, Item::compressedAluminium_Id, random, getRecipeChance(.7f));
		addItemForTradeIn(newOffers, Item::compressedIron_Id, random, getRecipeChance(.7f));

		addItemForPurchase(newOffers, Item::yellowDust_Id, random, getRecipeChance(.3f));
		addItemForPurchase(newOffers, Item::moonDungeonKey_Id, random, getRecipeChance(.7f));
		addItemForPurchase(newOffers, Item::buggyBlueprint_Id, random, getRecipeChance(.5f));
		addItemForPurchase(newOffers, Item::regulatorBlueprint_Id, random, getRecipeChance(.5f));
		addItemForPurchase(newOffers, Item::fuelCanister_Id, random, getRecipeChance(.3f));
		addItemForPurchase(newOffers, Item::oxygenGear_Id, random, getRecipeChance(.2f));
		addItemForPurchase(newOffers, Item::oxygenMask_Id, random, getRecipeChance(.2f));
		addItemForPurchase(newOffers, Item::oxygenTank_Id, random, getRecipeChance(.2f));
		addItemForPurchase(newOffers, Item::rocketFins_Id, random, getRecipeChance(.2f));
		addItemForPurchase(newOffers, Item::rocketEngine_Id, random, getRecipeChance(.2f));
		addItemForPurchase(newOffers, Item::noseCone_Id, random, getRecipeChance(.2f));
		addItemForPurchase(newOffers, Item::compressedHeavyDuty_Id, random, getRecipeChance(.5f));
		
		break;
	}

	if (newOffers->empty())
	{
		addItemForTradeIn(newOffers, Item::netherQuartz_Id, random, 1.0f);
	}

	// shuffle the list to make it more interesting
	std::random_shuffle(newOffers->begin(), newOffers->end());

	if (offers == NULL)
	{
		offers = new MerchantRecipeList();
	}
	for (int i = 0; i < addCount && i < newOffers->size(); i++)
	{
		if (offers->addIfNewOrBetter(newOffers->at(i)))
		{
			// 4J Added so we can delete newOffers
			newOffers->erase(newOffers->begin() + i);
		}
	}
	delete newOffers;
}

void AlienVillager::overrideOffers(MerchantRecipeList* recipeList)
{
}


void AlienVillager::staticCtor()
{
	MIN_MAX_VALUES[Item::wheat_Id] = pair<int, int>(16, 24);
	MIN_MAX_VALUES[Item::cheese_Id] = pair<int, int>(16, 24);
	MIN_MAX_VALUES[Item::tinCanister_Id] = pair<int, int>(2, 4);
	MIN_MAX_VALUES[Item::netherQuartz_Id] = pair<int, int>(16, 24);
	MIN_MAX_VALUES[Item::copperIngot_Id] = pair<int, int>(1, 3);
	MIN_MAX_VALUES[Item::aluminiumIngot_Id] = pair<int, int>(1, 3);
	MIN_MAX_VALUES[Item::ironIngot_Id] = pair<int, int>(3, 4);
	MIN_MAX_VALUES[Item::tinIngot_Id] = pair<int, int>(1, 3);
	MIN_MAX_VALUES[Item::rawSilicon_Id] = pair<int, int>(1, 3);
	MIN_MAX_VALUES[Item::compressedCopper_Id] = pair<int, int>(1, 2);
	MIN_MAX_VALUES[Item::compressedAluminium_Id] = pair<int, int>(1, 2);
	MIN_MAX_VALUES[Item::compressedIron_Id] = pair<int, int>(1, 2);

	MIN_MAX_PRICES[Item::dehydratedApple_Id] = pair<int, int>(1, 3);
	MIN_MAX_PRICES[Item::dehydratedCarrot_Id] = pair<int, int>(1, 3);
	MIN_MAX_PRICES[Item::dehydratedMelon_Id] = pair<int, int>(1, 2);
	MIN_MAX_PRICES[Item::dehydratedPotato_Id] = pair<int, int>(1, 2);
	MIN_MAX_PRICES[Item::cannedBeef_Id] = pair<int, int>(1, 3);
	MIN_MAX_PRICES[Item::cheese_Id] = pair<int, int>(-3, -2);
	MIN_MAX_PRICES[Item::cheeseburger_Id] = pair<int, int>(3, 6);
	MIN_MAX_PRICES[Item::poutine_Id] = pair<int, int>(3, 6);
	MIN_MAX_PRICES[Tile::cheeseBlock_Id] = pair<int, int>(8, 12);
	MIN_MAX_PRICES[Item::bread_Id] = pair<int, int>(-3, -2);
	MIN_MAX_PRICES[Item::carrotGolden_Id] = pair<int, int>(4, 6);
	MIN_MAX_PRICES[Item::apple_gold_Id] = pair<int, int>(10, 15);
	MIN_MAX_PRICES[Item::cake_Id] = pair<int, int>(8, 12);
	MIN_MAX_PRICES[Item::moonDungeonKey_Id] = pair<int, int>(20, 25);
	MIN_MAX_PRICES[Item::titaniumSword_Id] = pair<int, int>(45, 50);
	MIN_MAX_PRICES[Item::titaniumHatchet_Id] = pair<int, int>(45, 50);
	MIN_MAX_PRICES[Item::titaniumPickaxe_Id] = pair<int, int>(45, 50);
	MIN_MAX_PRICES[Item::titaniumShovel_Id] = pair<int, int>(45, 50);
	MIN_MAX_PRICES[Item::titaniumHoe_Id] = pair<int, int>(45, 50);
	MIN_MAX_PRICES[Item::titaniumBoots_Id] = pair<int, int>(45, 50);
	MIN_MAX_PRICES[Item::titaniumHelmet_Id] = pair<int, int>(45, 50);
	MIN_MAX_PRICES[Item::titaniumChestplate_Id] = pair<int, int>(45, 50);
	MIN_MAX_PRICES[Item::titaniumLeggings_Id] = pair<int, int>(45, 50);
	MIN_MAX_PRICES[Item::yellowDust_Id] = pair<int, int>(-3, -2);
	MIN_MAX_PRICES[Item::oxygenGear_Id] = pair<int, int>(25, 35);
	MIN_MAX_PRICES[Item::oxygenMask_Id] = pair<int, int>(25, 35);
	MIN_MAX_PRICES[Item::oxygenTank_Id] = pair<int, int>(25, 35);
	MIN_MAX_PRICES[Item::rocketFins_Id] = pair<int, int>(20, 30);
	MIN_MAX_PRICES[Item::rocketEngine_Id] = pair<int, int>(20, 30);
	MIN_MAX_PRICES[Item::noseCone_Id] = pair<int, int>(20, 30);
	MIN_MAX_PRICES[Item::fuelCanister_Id] = pair<int, int>(18, 28);
	MIN_MAX_PRICES[Item::buggyBlueprint_Id] = pair<int, int>(38, 45);
	MIN_MAX_PRICES[Item::regulatorBlueprint_Id] = pair<int, int>(38, 45);
	MIN_MAX_PRICES[Item::compressedHeavyDuty_Id] = pair<int, int>(7, 13);
}

/**
* Adds a merchant recipe that trades items for a single ruby.
*
* @param list
* @param itemId
* @param random
* @param likelyHood
*/
void AlienVillager::addItemForTradeIn(MerchantRecipeList* list, int itemId, Random* random, float likelyHood)
{
	if (random->nextFloat() < likelyHood)
	{
		list->push_back(new MerchantRecipe(getItemTradeInValue(itemId, random), Item::sapphire));
	}
}

shared_ptr<ItemInstance> AlienVillager::getItemTradeInValue(int itemId, Random* random)
{
	return std::make_shared<ItemInstance>(itemId, getTradeInValue(itemId, random), 0);
}

int AlienVillager::getTradeInValue(int itemId, Random* random)
{
	AUTO_VAR(it,MIN_MAX_VALUES.find(itemId));
	if (it == MIN_MAX_VALUES.end())
	{
		return 1;
	}
	pair<int, int> minMax = it->second;
	if (minMax.first >= minMax.second)
	{
		return minMax.first;
	}
	return minMax.first + random->nextInt(minMax.second - minMax.first);
}

/**
* Adds a merchant recipe that trades rubies for an item. If the cost is
* negative, one ruby will give several of that item.
*
* @param list
* @param itemId
* @param random
* @param likelyHood
*/
void AlienVillager::addItemForPurchase(MerchantRecipeList* list, int itemId, Random* random, float likelyHood)
{
	if (random->nextFloat() < likelyHood)
	{
		int purchaseCost = getPurchaseCost(itemId, random);
		shared_ptr<ItemInstance> rubyItem;
		shared_ptr<ItemInstance> resultItem;
		if (purchaseCost < 0)
		{
			rubyItem = std::make_shared<ItemInstance>(Item::sapphire_Id, 1, 0);
			resultItem = std::make_shared<ItemInstance>(itemId, -purchaseCost, 0);
		}
		else
		{
			rubyItem = std::make_shared<ItemInstance>(Item::sapphire_Id, purchaseCost, 0);
			resultItem = std::make_shared<ItemInstance>(itemId, 1, 0);
		}
		list->push_back(new MerchantRecipe(rubyItem, resultItem));
	}
}

int AlienVillager::getPurchaseCost(int itemId, Random* random)
{
	AUTO_VAR(it,MIN_MAX_PRICES.find(itemId));
	if (it == MIN_MAX_PRICES.end())
	{
		return 1;
	}
	pair<int, int> minMax = it->second;
	if (minMax.first >= minMax.second)
	{
		return minMax.first;
	}
	return minMax.first + random->nextInt(minMax.second - minMax.first);
}

void AlienVillager::handleEntityEvent(byte id)
{
	if (id == EntityEvent::LOVE_HEARTS)
	{
		addParticlesAroundSelf(eParticleType_heart);
	}
	else if (id == EntityEvent::VILLAGER_ANGRY)
	{
		addParticlesAroundSelf(eParticleType_angryVillager);
	}
	else if (id == EntityEvent::VILLAGER_HAPPY)
	{
		addParticlesAroundSelf(eParticleType_happyVillager);
	}
	else
	{
		AgableMob::handleEntityEvent(id);
	}
}

void AlienVillager::addParticlesAroundSelf(ePARTICLE_TYPE particle)
{
	for (int i = 0; i < 5; i++)
	{
		double xa = random->nextGaussian() * 0.02;
		double ya = random->nextGaussian() * 0.02;
		double za = random->nextGaussian() * 0.02;
		level->addParticle(particle, x + random->nextFloat() * bbWidth * 2 - bbWidth, y + 1.0f + random->nextFloat() * bbHeight, z + random->nextFloat() * bbWidth * 2 - bbWidth, xa, ya, za);
	}
}

MobGroupData* AlienVillager::finalizeMobSpawn(MobGroupData* groupData, int extraData /*= 0*/) // 4J Added extraData param
{
	groupData = AgableMob::finalizeMobSpawn(groupData);

	setProfession(level->random->nextInt(PROFESSION_MAX));

	return groupData;
}

void AlienVillager::setRewardPlayersInVillage()
{
	rewardPlayersOnFirstVillage = true;
}

shared_ptr<AgableMob> AlienVillager::getBreedOffspring(shared_ptr<AgableMob> target)
{
	// 4J - added limit to AlienVillagers that can be bred
	if (level->canCreateMore(GetType(), Level::eSpawnType_Breed))
	{
		shared_ptr<AlienVillager> alienVillager = std::make_shared<AlienVillager>(level);
		alienVillager->finalizeMobSpawn(NULL);
		return alienVillager;
	}
	else
	{
		return NULL;
	}
}

bool AlienVillager::canBeLeashed()
{
	return false;
}

wstring AlienVillager::getDisplayName()
{
	if (hasCustomName()) return getCustomName();

	int name = IDS_LUNAR;
	return app.GetString(name);
}

float AlienVillager::getVoicePitch()
{
	return LivingEntity::getVoicePitch() * 0.5f;
}