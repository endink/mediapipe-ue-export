#ifndef  UMP_PACKET_H
#define UMP_PACKET_H

#include "ump_commons.h"
#include "ump_packet.h"
#include "packet.h"

class PacketAPI : public IPacketAPI
{
public:
	virtual int GetBoolean(void* packet, bool* value_out) override;
	virtual int GetLandmarkList(void* packet, SerializedProto* value_out) override;
	virtual int GetLandmarkListVector(void* packet, SerializedProtoArray* value_out) override;
	virtual int GetNormalizedLandmarkList(void* packet, SerializedProto* value_out) override;
	virtual int GetNormalizedLandmarkListVector(void* packet, SerializedProtoArray* value_out) override;
	virtual int GetFaceGeometry(void* packet, SerializedProto* value_out) override;
	virtual int GetFaceGeometryVector(void* packet, SerializedProtoArray* value_out) override;
	virtual int GetClassificationList(void* packet, SerializedProto* value_out) override;
	virtual int GetClassificationListVector(void* packet, SerializedProtoArray* value_out) override;

	virtual void ClearProtoArray(SerializedProtoArray* proto_array) override;

	virtual void* NewSidePacket() override;
	virtual void DeleteSidePacket(void* side_packet) override;
	virtual int SidePacketAddBoolean(void* side_packet, const char* key, bool value) override;
	virtual int SidePacketAddFloat(void* side_packet, const char* key, float value) override;
	virtual int SidePacketAddInt(void* side_packet, const char* key, int value) override;
};

#endif // ! UMP_PACKET_H
