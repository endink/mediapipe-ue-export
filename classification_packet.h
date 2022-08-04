#ifndef CLASSIFICATION_PACKET_H
#define CLASSIFICATION_PACKET_H

#include "ump_commons.h"

extern "C" {
	MP_API int mp_Packet__GetClassificationListVector(void* packet, SerializedProtoArray* value_out);
}  // extern "C"

#endif  // CLASSIFICATION_PACKET_H
