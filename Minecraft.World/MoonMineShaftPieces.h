#pragma once

#include "StructurePiece.h"

class MoonMineShaftPieces
{
private:
	static const int DEFAULT_SHAFT_WIDTH = 3;
	static const int DEFAULT_SHAFT_HEIGHT = 3;
	static const int DEFAULT_SHAFT_LENGTH = 5;

	static const int MAX_DEPTH = 8; // 1.2.3 change

public:
	static void loadStatic();

private:
	static StructurePiece* createRandomShaftPiece(list<StructurePiece*>* pieces, Random* random, int footX, int footY, int footZ, int direction, int genDepth);
	static StructurePiece* generateAndAddPiece(StructurePiece* startPiece, list<StructurePiece*>* pieces, Random* random, int footX, int footY, int footZ, int direction, int depth);

	/**
	*
	*
	*/
public:
	class MoonMineShaftRoom : public StructurePiece
	{
	public:
		static StructurePiece* Create() { return new MoonMineShaftRoom(); }
		virtual EStructurePiece GetType() { return eStructurePiece_MoonMineShaftRoom; }

	private:
		list<BoundingBox*> childEntranceBoxes;

	public:
		MoonMineShaftRoom();
		MoonMineShaftRoom(int genDepth, Random* random, int west, int north);
		~MoonMineShaftRoom();

		virtual void addChildren(StructurePiece* startPiece, list<StructurePiece*>* pieces, Random* random);
		virtual bool postProcess(Level* level, Random* random, BoundingBox* chunkBB);

	protected:
		void addAdditonalSaveData(CompoundTag* tag);
		void readAdditonalSaveData(CompoundTag* tag);
	};

	/**
	*
	*
	*/
	class MoonMineShaftCorridor : public StructurePiece
	{
	public:
		static StructurePiece* Create() { return new MoonMineShaftCorridor(); }
		virtual EStructurePiece GetType() { return eStructurePiece_MoonMineShaftCorridor; }

	private:
		bool hasRails; // was final
		bool spiderCorridor; // was final
		bool hasPlacedSpider;
		int numSections;

	public:
		MoonMineShaftCorridor();

	protected:
		void addAdditonalSaveData(CompoundTag* tag);
		void readAdditonalSaveData(CompoundTag* tag);

	public:
		MoonMineShaftCorridor(int genDepth, Random* random, BoundingBox* corridorBox, int direction);

		static BoundingBox* findCorridorSize(list<StructurePiece*>* pieces, Random* random, int footX, int footY, int footZ, int direction);
		virtual void addChildren(StructurePiece* startPiece, list<StructurePiece*>* pieces, Random* random);

	protected:
		virtual bool createChest(Level* level, BoundingBox* chunkBB, Random* random, int x, int y, int z, WeighedTreasureArray treasure, int numRolls);

	public:
		virtual bool postProcess(Level* level, Random* random, BoundingBox* chunkBB);
	};

	/**
	*
	*
	*/
	class MoonMineShaftCrossing : public StructurePiece
	{
	public:
		static StructurePiece* Create() { return new MoonMineShaftCrossing(); }
		virtual EStructurePiece GetType() { return eStructurePiece_MoonMineShaftCrossing; }

	private:
		int direction;
		bool isTwoFloored;

	public:
		MoonMineShaftCrossing();

	protected:
		void addAdditonalSaveData(CompoundTag* tag);
		void readAdditonalSaveData(CompoundTag* tag);

	public:
		MoonMineShaftCrossing(int genDepth, Random* random, BoundingBox* crossingBox, int direction);

		static BoundingBox* findCrossing(list<StructurePiece*>* pieces, Random* random, int footX, int footY, int footZ, int direction);
		virtual void addChildren(StructurePiece* startPiece, list<StructurePiece*>* pieces, Random* random);
		virtual bool postProcess(Level* level, Random* random, BoundingBox* chunkBB);
	};

	/**
	*
	*
	*/
	class MoonMineShaftStairs : public StructurePiece
	{
	public:
		static StructurePiece* Create() { return new MoonMineShaftStairs(); }
		virtual EStructurePiece GetType() { return eStructurePiece_MoonMineShaftStairs; }

	public:
		MoonMineShaftStairs();
		MoonMineShaftStairs(int genDepth, Random* random, BoundingBox* stairsBox, int direction);

	protected:
		void addAdditonalSaveData(CompoundTag* tag);
		void readAdditonalSaveData(CompoundTag* tag);

	public:
		static BoundingBox* findStairs(list<StructurePiece*>* pieces, Random* random, int footX, int footY, int footZ, int direction);
		virtual void addChildren(StructurePiece* startPiece, list<StructurePiece*>* pieces, Random* random);
		virtual bool postProcess(Level* level, Random* random, BoundingBox* chunkBB);

	};

	/* @formatter:off */
private:
	static WeighedTreasureArray smallTreasureItems;
	/* @formatter:on */

public:
	static void staticCtor();

};