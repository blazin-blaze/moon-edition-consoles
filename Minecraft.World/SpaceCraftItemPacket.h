#pragma once

// 4J ADDED THIS PACKET

using namespace std;

#include "Packet.h"

class SpaceCraftItemPacket : public Packet, public enable_shared_from_this<SpaceCraftItemPacket>
{
public:
	int recipe;
	short uid;

	SpaceCraftItemPacket();
	~SpaceCraftItemPacket();
	SpaceCraftItemPacket(int recipe, short uid);

	virtual void handle(PacketListener* listener);
	virtual void read(DataInputStream* dis);
	virtual void write(DataOutputStream* dos);
	virtual int getEstimatedSize();

public:
	static shared_ptr<Packet> create() { return shared_ptr<Packet>(new SpaceCraftItemPacket()); }
	virtual int getId() { return 210; }
};