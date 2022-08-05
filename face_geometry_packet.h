#ifndef FACE_GEOMETRY_PACKET_H
#define FACE_GEOMETRY_PACKET_H
#include "ump_commons.h"

extern "C" {
	MP_API int mp_Packet__GetFaceGeometryVector(void* packet, SerializedProtoArray* value_out);
	MP_API int mp_Packet__GetFaceGeometry(void* packet, SerializedProto* value_out);
}  // extern "C"

#endif  // LANDMARK_PACKET_H_