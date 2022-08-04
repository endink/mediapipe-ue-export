#include "landmarks_packet.h"
#include "packet.h"
#include "mediapipe/framework/formats/landmark.pb.h"

int mp_Packet__GetLandmarkList(void* packet, SerializedProto* value_out)
{
	auto p = (mediapipe::Packet*)packet;
	return mp_Packet__GetSerializedProto<mediapipe::LandmarkList>(p, value_out);
}

int mp_Packet__GetLandmarkListVector(void* packet, SerializedProtoArray* value_out)
{
	auto p = (mediapipe::Packet*)packet;
	return mp_Packet__GetSerializedProtoVector<mediapipe::LandmarkList>(p, value_out);
}

int mp_Packet__GetNormalizedLandmarkList(void* packet, SerializedProto* value_out)
{
	auto p = (mediapipe::Packet*)packet;
	return mp_Packet__GetSerializedProto<mediapipe::NormalizedLandmarkList>(p, value_out);
}

int mp_Packet__GetNormalizedLandmarkListVector(void* packet, SerializedProtoArray* value_out)
{
	auto p = (mediapipe::Packet*)packet;
	return mp_Packet__GetSerializedProtoVector<mediapipe::NormalizedLandmarkList>(p, value_out);
}
