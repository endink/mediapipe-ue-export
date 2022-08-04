#ifndef  IPACKET_API_H
#define IPACKET_API_H

#include "ump_commons.h"

class IPacketAPI
{
public:
	virtual int GetLandmarkBoolean(void* packet, bool* value_out) = 0;
	virtual int GetLandmarkList(void* packet, SerializedProto* value_out) = 0;
	virtual int GetLandmarkListVector(void* packet, SerializedProtoArray* value_out) = 0;
	virtual int GetNormalizedLandmarkList(void* packet, SerializedProto* value_out) = 0;
	virtual int GetNormalizedLandmarkListVector(void* packet, SerializedProtoArray* value_out) = 0;
	virtual int GetFaceGeometryVector(void* packet, SerializedProtoArray* value_out) = 0;
	virtual int GetClassificationListVector(void* packet, SerializedProtoArray* value_out) = 0;
};

#endif // ! UMP_PACKET_H