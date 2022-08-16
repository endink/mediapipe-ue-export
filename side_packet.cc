#include "side_packet.h"

void* mp_SidePacket__new() {
	try
	{
		auto sp = new SidePacket();
		return &sp;
	}
	catch (...)
	{
		return nullptr;
	}
}

void mp_SidePacket__delete(void* side_packet) { delete (SidePacket*)side_packet; }

int mp_SidePacket__emplace__PKc_Rp(void* side_packet, const char* key, void* packet)
{
	TRY
		((SidePacket*)side_packet)->emplace(std::string(key), std::move(*((mediapipe::Packet*)packet)));
	CATCH_EXCEPTION
}

void* mp_SidePacket__at__PKc(void* side_packet, const char* key)
{
	try
	{
		auto packet = ((SidePacket*)side_packet)->at(std::string(key));
		auto p = new mediapipe::Packet{ packet };
		return p;
	}
	catch(...)
	{
		return nullptr;
	}
}

int mp_SidePacket__erase__PKc(void* side_packet, const char* key, int* count_out) {
	TRY
		*count_out = ((SidePacket*)side_packet)->erase(std::string(key));
	CATCH_EXCEPTION
}

void mp_SidePacket__clear(void* side_packet) { ((SidePacket*)side_packet)->clear(); }

int mp_SidePacket__size(void* side_packet) { return ((SidePacket*)side_packet)->size(); }

