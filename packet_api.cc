#include "packet_api.h"
#include "landmarks_packet.h"
#include "face_geometry_packet.h"
#include "classification_packet.h"
#include "packet.h"
#include "ump_shared.h"

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

void PacketAPI::ClearProtoArray(SerializedProtoArray* proto_array)
{
	if(proto_array != nullptr)
	{
		DeleteSerializedProtoArray(proto_array->data, proto_array->size);
	};
}

void* PacketAPI::NewSidePacket()
{
	return new SidePacket();
}

void PacketAPI::DeleteSidePacket(void* side_packet)
{
	mp_SidePacket__delete((SidePacket*)side_packet);
}


int PacketAPI::SidePacketAddInt(void* side_packet, const char* key, int value)
{
	TRY
		SidePacket* sp = (SidePacket*)side_packet;
		mediapipe::Packet* p = mp__MakeIntPacket__i(value);
		if (p != nullptr)
		{
			mp_SidePacket__emplace__PKc_Rp(sp, key, p);
			mp_Packet__delete(p);
			return 0;
		}
	CATCH_ONLY
		return -1;
}

int PacketAPI::SidePacketAddFloat(void* side_packet, const char* key, float value)
{
	TRY
		SidePacket* sp = (SidePacket*)side_packet;
		mediapipe::Packet* p = mp__MakeFloatPacket__f(value);
		if (p != nullptr)
		{
			mp_SidePacket__emplace__PKc_Rp(sp, key, p);
			mp_Packet__delete(p);
			return 0;
		}
	CATCH_ONLY
		return -1;
}

int PacketAPI::SidePacketAddBoolean(void* side_packet, const char* key, bool value)
{
	TRY
		SidePacket* sp = static_cast<SidePacket*>(side_packet);
		mediapipe::Packet* p = mp__MakeBoolPacket__b(value);
		if (p != nullptr)
		{
			mp_SidePacket__emplace__PKc_Rp(sp, key, p);
			mp_Packet__delete(p);
			return 0;
		}
	CATCH_ONLY
		return -1;
}
