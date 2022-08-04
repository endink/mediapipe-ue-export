#ifndef LANDMARK_PACKET_H_
#define LANDMARK_PACKET_H_
#include "ump_commons.h"

extern "C" {

	MP_API int mp_Packet__GetLandmarkList(void* packet, SerializedProto* value_out);
	MP_API int mp_Packet__GetLandmarkListVector(void* packet, SerializedProtoArray* value_out);
	MP_API int mp_Packet__GetNormalizedLandmarkList(void* packet, SerializedProto* value_out);
	MP_API int mp_Packet__GetNormalizedLandmarkListVector(void* packet, SerializedProtoArray* value_out);

}  // extern "C"

#endif  // LANDMARK_PACKET_H_