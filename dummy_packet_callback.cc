#include "dummy_packet_callback.h"
#include <iostream>

DummyPacketCallback::DummyPacketCallback()
{
}

inline bool DummyPacketCallback::OnUmpPacket(IUmpObserver* observer, void* packet)
{
	auto api = observer->GetPacketAPI();
	SerializedProto proto;
	api->GetNormalizedLandmarkList(packet, &proto);

	std::cout << proto.str << std::endl;
	return true;
}
