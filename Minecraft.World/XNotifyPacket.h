#pragma once
using namespace std;

#include "Packet.h"

class XNotifyPacket : public Packet, public enable_shared_from_this<XNotifyPacket>
{

public:

	static const int MOON_ACHIEVEMENT;
	static const int OXYGEN_SETUP_ACHIEVEMENT;
	static const int MOON_CHEESE_ACHIEVEMENT;
	static const int MOON_BUGGY_ACHIEVEMENT;
	static const int MOON_SURVIVAL_ACHIEVEMENT;

	int type; 

	XNotifyPacket();
	XNotifyPacket(int type);

	virtual void read(DataInputStream *dis);
	virtual void write(DataOutputStream *dos);
	virtual void handle(PacketListener *listener);
	virtual int getEstimatedSize();

public:
	static shared_ptr<Packet> create() { return shared_ptr<Packet>(new XNotifyPacket()); }
	virtual int getId() { return 211; }
};