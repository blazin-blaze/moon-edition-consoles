#include "stdafx.h"
#include "BasicTypeContainers.h"
#include "InputOutputStream.h"
#include "net.minecraft.network.packet.h"
#include "LevelRocketSoundMovePacket.h"

const float LevelRocketSoundMovePacket::LOCATION_ACCURACY = 8.0f;

LevelRocketSoundMovePacket::LevelRocketSoundMovePacket()
{
	x = 0;
	y = Integer::MAX_VALUE;
	z = 0;
	rocketId = 0;
}

LevelRocketSoundMovePacket::LevelRocketSoundMovePacket(double x, double y, double z, int rocketId)
{
	this->x = static_cast<int>(x * LOCATION_ACCURACY);
	this->y = static_cast<int>(y * LOCATION_ACCURACY);
	this->z = static_cast<int>(z * LOCATION_ACCURACY);
	this->rocketId = rocketId;
}

void LevelRocketSoundMovePacket::read(DataInputStream* dis)
{
	x = dis->readInt();
	y = dis->readInt();
	z = dis->readInt();
	rocketId = dis->readInt();
}

void LevelRocketSoundMovePacket::write(DataOutputStream* dos)
{
	dos->writeInt(x);
	dos->writeInt(y);
	dos->writeInt(z);
	dos->writeInt(rocketId);
}

double LevelRocketSoundMovePacket::getX()
{
	return x / LOCATION_ACCURACY;
}

double LevelRocketSoundMovePacket::getY()
{
	return y / LOCATION_ACCURACY;
}

double LevelRocketSoundMovePacket::getZ()
{
	return z / LOCATION_ACCURACY;
}

int LevelRocketSoundMovePacket::getRocketId()
{
	return rocketId;
}

void LevelRocketSoundMovePacket::handle(PacketListener* listener)
{
	listener->handleRocketSoundMoveEvent(shared_from_this());
}

int LevelRocketSoundMovePacket::getEstimatedSize()
{
	return 4 * 6;
}