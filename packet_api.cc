#include "packet_api.h"
#include "landmarks_packet.h"
#include "face_geometry_packet.h"
#include "classification_packet.h"
#include "packet.h"

int PacketAPI::GetBoolean(void* packet, bool* value_out)
{
	auto p = (mediapipe::Packet*)packet;
	return mp_Packet__GetBool(p, value_out);
}

int PacketAPI::GetLandmarkList(void* packet, SerializedProto* value_out)
{
	return mp_Packet__GetLandmarkList(packet, value_out);
}

int PacketAPI::GetLandmarkListVector(void* packet, SerializedProtoArray* value_out)
{
	return mp_Packet__GetLandmarkListVector(packet, value_out);
}

int PacketAPI::GetNormalizedLandmarkList(void* packet, SerializedProto* value_out)
{
	return mp_Packet__GetNormalizedLandmarkList(packet, value_out);
}

int PacketAPI::GetNormalizedLandmarkListVector(void* packet, SerializedProtoArray* value_out)
{
	return mp_Packet__GetNormalizedLandmarkListVector(packet, value_out);
}

int PacketAPI::GetFaceGeometry(void* packet, SerializedProto* value_out)
{
	return mp_Packet__GetFaceGeometry(packet, value_out);
}

int PacketAPI::GetFaceGeometryVector(void* packet, SerializedProtoArray* value_out)
{
	return mp_Packet__GetFaceGeometryVector(packet, value_out);
}

int PacketAPI::GetClassificationList(void* packet, SerializedProto* value_out)
{
	return mp_Packet__GetClassificationList(packet, value_out);
}

int PacketAPI::GetClassificationListVector(void* packet, SerializedProtoArray* value_out)
{
	return mp_Packet__GetClassificationListVector(packet, value_out);
}