#pragma once

#include "Packet.h"

class LevelRocketSoundPacket : public Packet, public enable_shared_from_this<LevelRocketSoundPacket>
{
public:
	static const float LOCATION_ACCURACY;

private:
	int x;
	int y;
	int z;
	int rocketId;

public:
	LevelRocketSoundPacket();
	LevelRocketSoundPacket(double x, double y, double z, int rocketId);

	virtual void read(DataInputStream* dis);
	virtual void write(DataOutputStream* dos);
	double getX();
	double getY();
	double getZ();
	int getRocketId();
	virtual void handle(PacketListener* listener);
	virtual int getEstimatedSize();

public:
	static shared_ptr<Packet> create() { return shared_ptr<Packet>(new LevelRocketSoundPacket()); }
	virtual int getId() { return 212; }
};