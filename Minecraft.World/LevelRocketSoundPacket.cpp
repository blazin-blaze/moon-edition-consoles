#include "stdafx.h"
#include "BasicTypeContainers.h"
#include "InputOutputStream.h"
#include "net.minecraft.network.packet.h"
#include "LevelRocketSoundPacket.h"

const float LevelRocketSoundPacket::LOCATION_ACCURACY = 8.0f;

LevelRocketSoundPacket::LevelRocketSoundPacket()
{
	x = 0;
	y = Integer::MAX_VALUE;
	z = 0;
	rocketId = 0;
}

LevelRocketSoundPacket::LevelRocketSoundPacket(double x, double y, double z, int rocketId)
{
	this->x = static_cast<int>(x * LOCATION_ACCURACY);
	this->y = static_cast<int>(y * LOCATION_ACCURACY);
	this->z = static_cast<int>(z * LOCATION_ACCURACY);
	// 4J-PB - Let's make the pitch a float so it doesn't get mangled and make the noteblock people unhappy
	//this->pitch = (int) (pitch * PITCH_ACCURACY);
	this->rocketId = rocketId;

	// 	if (this->pitch < 0) this->pitch = 0;
	// 	if (this->pitch > 255) this->pitch = 255;
}

void LevelRocketSoundPacket::read(DataInputStream* dis)
{
	x = dis->readInt();
	y = dis->readInt();
	z = dis->readInt();
	rocketId = dis->readInt();
}

void LevelRocketSoundPacket::write(DataOutputStream* dos)
{
	dos->writeInt(x);
	dos->writeInt(y);
	dos->writeInt(z);
	dos->writeInt(rocketId);
}

double LevelRocketSoundPacket::getX()
{
	return x / LOCATION_ACCURACY;
}

double LevelRocketSoundPacket::getY()
{
	return y / LOCATION_ACCURACY;
}

double LevelRocketSoundPacket::getZ()
{
	return z / LOCATION_ACCURACY;
}

int LevelRocketSoundPacket::getRocketId()
{
	return rocketId;
}

void LevelRocketSoundPacket::handle(PacketListener* listener)
{
	listener->handleRocketSoundEvent(shared_from_this());
}

int LevelRocketSoundPacket::getEstimatedSize()
{
	return 4 * 6;
}