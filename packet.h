// Copyright (c) 2021 homuler
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#ifndef MP_PACKET_H_
#define MP_PACKET_H_

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "mediapipe/framework/packet.h"
#include "ump_commons.h"
#include "ump_shared.h"
#include "protobuf.h"



template <typename T>
inline int mp_Packet__Consume(mediapipe::Packet* packet, absl::StatusOr<T>** status_or_value_out) {
    TRY
        auto status_or_unique_ptr = packet->Consume<T>();

    if (status_or_unique_ptr.ok()) {
        *status_or_value_out = new absl::StatusOr<T>{ std::move(*status_or_unique_ptr.value().release()) };
    }
    else {
        *status_or_value_out = new absl::StatusOr<T>{ status_or_unique_ptr.status() };
    }
    CATCH_EXCEPTION
}



template <typename T>
inline int mp_Packet__GetSerializedProto(mediapipe::Packet* packet, SerializedProto* value_out) {
    TRY
        auto proto = packet->Get<T>();
    SerializeProto(proto, value_out);
    CATCH_EXCEPTION
}

template <typename T>
inline int mp_Packet__GetSerializedProtoVector(mediapipe::Packet* packet, SerializedProtoArray* value_out) {
    TRY
        auto proto_vec = packet->Get<std::vector<T>>();
    SerializeProtoVector(proto_vec, value_out);
    CATCH_EXCEPTION
}

extern "C" {
    typedef std::map<std::string, mediapipe::Packet> SidePacket;

    /** mediapipe::Packet API */
    MP_API void mp_Packet__delete(mediapipe::Packet* packet);
    MP_API int mp_Packet__At__Rt(mediapipe::Packet* packet, mediapipe::Timestamp* timestamp, mediapipe::Packet** packet_out);
    MP_API bool mp_Packet__IsEmpty(mediapipe::Packet* packet);
    MP_API int mp_Packet__ValidateAsProtoMessageLite(mediapipe::Packet* packet, absl::Status** status_out);
    MP_API int mp_Packet__Timestamp(mediapipe::Packet* packet, mediapipe::Timestamp** timestamp_out);
    MP_API int mp_Packet__DebugString(mediapipe::Packet* packet, const char** str_out);
    MP_API int mp_Packet__RegisteredTypeName(mediapipe::Packet* packet, const char** str_out);
    MP_API int mp_Packet__DebugTypeName(mediapipe::Packet* packet, const char** str_out);

    // Boolean
    MP_API mediapipe::Packet* mp__MakeBoolPacket__b(bool value);
    MP_API mediapipe::Packet* mp__MakeBoolPacket_At__b_Rt(bool value, mediapipe::Timestamp* timestamp);
    MP_API int mp_Packet__GetBool(mediapipe::Packet* packet, bool* value_out);
    MP_API int mp_Packet__ValidateAsBool(mediapipe::Packet* packet, absl::Status** status_out);

    // Float
    MP_API mediapipe::Packet* mp__MakeFloatPacket__f(float value);
    MP_API mediapipe::Packet* mp__MakeFloatPacket_At__f_Rt(float value, mediapipe::Timestamp* timestamp);
    MP_API int mp_Packet__GetFloat(mediapipe::Packet* packet, float* value_out);
    MP_API int mp_Packet__ValidateAsFloat(mediapipe::Packet* packet, absl::Status** status_out);

    // Int
    MP_API mediapipe::Packet* mp__MakeIntPacket__i(int value);
    MP_API int mp__MakeIntPacket_At__i_Rt(int value, mediapipe::Timestamp* timestamp, mediapipe::Packet** packet_out);
    MP_API int mp_Packet__GetInt(mediapipe::Packet* packet, int* value_out);
    MP_API int mp_Packet__ValidateAsInt(mediapipe::Packet* packet, absl::Status** status_out);

    // Float Array
    MP_API mediapipe::Packet* mp__MakeFloatArrayPacket__Pf_i(float* value, int size);
    MP_API mediapipe::Packet* mp__MakeFloatArrayPacket_At__Pf_i_Rt(float* value, int size, mediapipe::Timestamp* timestamp);
    MP_API int mp_Packet__GetFloatArray(mediapipe::Packet* packet, const float** value_out);
    MP_API int mp_Packet__ValidateAsFloatArray(mediapipe::Packet* packet, absl::Status** status_out);

    // String
    MP_API mediapipe::Packet* mp__MakeStringPacket__PKc(const char* str);
    MP_API mediapipe::Packet* mp__MakeStringPacket_At__PKc_Rt(const char* str, mediapipe::Timestamp* timestamp);
    MP_API mediapipe::Packet* mp__MakeStringPacket__PKc_i(const char* str, int size);
    MP_API mediapipe::Packet* mp__MakeStringPacket_At__PKc_i_Rt(const char* str, int size, mediapipe::Timestamp* timestamp);
    MP_API int mp_Packet__GetString(mediapipe::Packet* packet, const char** value_out);
    MP_API int mp_Packet__GetByteString(mediapipe::Packet* packet, const char** value_out, int* size_out);
    MP_API int mp_Packet__ConsumeString(mediapipe::Packet* packet, absl::StatusOr<std::string>** status_or_value_out);
    MP_API int mp_Packet__ValidateAsString(mediapipe::Packet* packet, absl::Status** status_out);

    /** SidePacket API */
    MP_API void mp_SidePacket__delete(SidePacket* side_packet);
    MP_API int mp_SidePacket__emplace__PKc_Rp(SidePacket* side_packet, const char* key, mediapipe::Packet* packet);
    MP_API int mp_SidePacket__at__PKc(SidePacket* side_packet, const char* key, mediapipe::Packet** packet_out);
    MP_API int mp_SidePacket__erase__PKc(SidePacket* side_packet, const char* key, int* count_out);
    MP_API void mp_SidePacket__clear(SidePacket* side_packet);
    MP_API int mp_SidePacket__size(SidePacket* side_packet);

}  // extern "C"



#endif  // MEDIAPIPE_API_FRAMEWORK_PACKET_H_
