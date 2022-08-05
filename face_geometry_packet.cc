#include "face_geometry_packet.h"
#include "packet.h"
#include "mediapipe/modules/face_geometry/protos/face_geometry.pb.h"

MP_API int mp_Packet__GetFaceGeometryVector(void* packet, SerializedProtoArray* value_out)
{
	auto p = (mediapipe::Packet*)packet;
	return mp_Packet__GetSerializedProtoVector<mediapipe::face_geometry::FaceGeometry>(p, value_out);
}

MP_API int mp_Packet__GetFaceGeometry(void* packet, SerializedProto* value_out)
{
	auto p = (mediapipe::Packet*)packet;
	return mp_Packet__GetSerializedProto<mediapipe::face_geometry::FaceGeometry>(p, value_out);
}
