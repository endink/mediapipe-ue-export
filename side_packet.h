#ifndef U_SIDE_PACKET_H
#define U_SIDE_PACKET_H
#include "ump_shared.h"

extern "C" {
	typedef std::map<std::string, mediapipe::Packet> SidePacket;

	MP_API void* mp_SidePacket__new();
	MP_API void mp_SidePacket__delete(void* side_packet);
	MP_API int mp_SidePacket__emplace__PKc_Rp(void* side_packet, const char* key, void* packet);
	MP_API void* mp_SidePacket__at__PKc(void* side_packet, const char* key);
	MP_API int mp_SidePacket__erase__PKc(void* side_packet, const char* key, int* count_out);
	MP_API void mp_SidePacket__clear(void* side_packet);
	MP_API int mp_SidePacket__size(void* side_packet);
}
#endif