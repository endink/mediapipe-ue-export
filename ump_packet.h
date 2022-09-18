#ifndef  IPACKET_API_H
#define IPACKET_API_H

#include "ump_commons.h"

class IPacketAPI
{
public:
	virtual ~IPacketAPI() = default;
	virtual int GetBoolean(void* packet, bool* value_out) = 0;
	virtual int GetLandmarkList(void* packet, SerializedProto* value_out) = 0;
	virtual int GetLandmarkListVector(void* packet, SerializedProtoArray* value_out) = 0;
	virtual int GetNormalizedLandmarkList(void* packet, SerializedProto* value_out) = 0;
	virtual int GetNormalizedLandmarkListVector(void* packet, SerializedProtoArray* value_out) = 0;
	virtual int GetFaceGeometry(void* packet, SerializedProto* value_out) = 0;
	virtual int GetFaceGeometryVector(void* packet, SerializedProtoArray* value_out) = 0;
	virtual int GetClassificationList(void* packet, SerializedProto* value_out) = 0;
	virtual int GetClassificationListVector(void* packet, SerializedProtoArray* value_out) = 0;

	virtual void ClearProtoArray(SerializedProtoArray* proto_array) = 0;

	virtual void* NewSidePacket() = 0;
	virtual void DeleteSidePacket(void* side_packet) = 0;

	virtual int SidePacketAddBoolean(void* side_packet, const char* key, bool value) = 0;
	virtual int SidePacketAddFloat(void* side_packet, const char* key, float value) = 0;
	virtual int SidePacketAddInt(void* side_packet, const char* key, int value) = 0;
};

#endif // ! UMP_PACKET_H