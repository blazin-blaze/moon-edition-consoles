#include "stdafx.h"
#include <iostream>
#include "InputOutputStream.h"
#include "net.minecraft.world.item.h"
#include "PacketListener.h"
#include "SpaceCraftItemPacket.h"



SpaceCraftItemPacket::~SpaceCraftItemPacket()
{
}

SpaceCraftItemPacket::SpaceCraftItemPacket()
{
	recipe = -1;
	uid = 0;
}

SpaceCraftItemPacket::SpaceCraftItemPacket(int recipe, short uid)
{
	this->recipe = recipe;
	this->uid = uid;
}

void SpaceCraftItemPacket::handle(PacketListener* listener)
{
	listener->handleSpaceCraftItem(shared_from_this());
}

void SpaceCraftItemPacket::read(DataInputStream* dis) //throws IOException
{
	uid = dis->readShort();
	recipe = dis->readInt();
}

void SpaceCraftItemPacket::write(DataOutputStream* dos) // throws IOException
{
	dos->writeShort(uid);
	dos->writeInt(recipe);
}

int SpaceCraftItemPacket::getEstimatedSize()
{
	return 2 + 4;
}