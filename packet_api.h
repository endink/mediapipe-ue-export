#ifndef  UMP_PACKET_H
#define UMP_PACKET_H

#include "ump_commons.h"
#include "ump_packet.h"

class PacketAPI : public IPacketAPI
{
public:
	PacketAPI() {}
	virtual int GetBoolean(void* packet, bool* value_out) override;
	virtual int GetLandmarkList(void* packet, SerializedProto* value_out) override;
	virtual int GetLandmarkListVector(void* packet, SerializedProtoArray* value_out) override;
	virtual int GetNormalizedLandmarkList(void* packet, SerializedProto* value_out) override;
	virtual int GetNormalizedLandmarkListVector(void* packet, SerializedProtoArray* value_out) override;
	virtual int GetFaceGeometry(void* packet, SerializedProto* value_out) override;
	virtual int GetFaceGeometryVector(void* packet, SerializedProtoArray* value_out) override;
	virtual int GetClassificationList(void* packet, SerializedProto* value_out) override;
	virtual int GetClassificationListVector(void* packet, SerializedProtoArray* value_out) override;

};

#endif // ! UMP_PACKET_H
