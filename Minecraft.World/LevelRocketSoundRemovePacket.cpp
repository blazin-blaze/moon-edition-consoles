#include "stdafx.h"
#include "BasicTypeContainers.h"
#include "InputOutputStream.h"
#include "net.minecraft.network.packet.h"
#include "LevelRocketSoundRemovePacket.h"

LevelRocketSoundRemovePacket::LevelRocketSoundRemovePacket()
{
	rocketId = 0;
}

LevelRocketSoundRemovePacket::LevelRocketSoundRemovePacket(int rocketId)
{
	this->rocketId = rocketId;
}

void LevelRocketSoundRemovePacket::read(DataInputStream* dis)
{
	rocketId = dis->readInt();
}

void LevelRocketSoundRemovePacket::write(DataOutputStream* dos)
{
	dos->writeInt(rocketId);
}

int LevelRocketSoundRemovePacket::getRocketId()
{
	return rocketId;
}

void LevelRocketSoundRemovePacket::handle(PacketListener* listener)
{
	listener->handleRocketSoundRemoveEvent(shared_from_this());
}

int LevelRocketSoundRemovePacket::getEstimatedSize()
{
	return 4 * 6;
}