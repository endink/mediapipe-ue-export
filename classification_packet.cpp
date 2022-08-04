#include "classification_packet.h"
#include "packet.h"
#include "mediapipe/framework/formats/classification.pb.h"

MP_API int mp_Packet__GetClassificationListVector(void* packet, SerializedProtoArray* value_out)
{
	auto p = (mediapipe::Packet*)packet;
	return mp_Packet__GetSerializedProtoVector<mediapipe::ClassificationList>(p, value_out);
}
