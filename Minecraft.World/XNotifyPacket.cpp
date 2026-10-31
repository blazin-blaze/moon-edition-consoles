#include "stdafx.h"
#include <iostream>
#include "InputOutputStream.h"
#include "PacketListener.h"
#include "XNotifyPacket.h"

const int XNotifyPacket::MOON_ACHIEVEMENT = 0;
const int XNotifyPacket::OXYGEN_SETUP_ACHIEVEMENT = 1;
const int XNotifyPacket::MOON_CHEESE_ACHIEVEMENT = 2;
const int XNotifyPacket::MOON_BUGGY_ACHIEVEMENT = 3;
const int XNotifyPacket::MOON_SURVIVAL_ACHIEVEMENT = 4;

XNotifyPacket::XNotifyPacket()
{
	this->type = 0;
}

XNotifyPacket::XNotifyPacket(int type)
{
	this->type = type;
}

void XNotifyPacket::read(DataInputStream *dis) //throws IOException 
{
	type = dis->readByte();
}

void XNotifyPacket::write(DataOutputStream *dos) //throws IOException 
{
	dos->writeByte(type);
}

void XNotifyPacket::handle(PacketListener *listener) 
{
	listener->handleXNotify(shared_from_this());
}

int XNotifyPacket::getEstimatedSize()
{
	return 2;
}
