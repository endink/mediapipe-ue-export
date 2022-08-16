// Copyright (c) 2021 homuler
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "packet.h"
#include "ump_shared.h"

#include <string>
#include <utility>


void mp_Packet__delete(mediapipe::Packet* packet) { delete packet; }

mediapipe::Packet* mp_Packet__At__Rt(mediapipe::Packet* packet, mediapipe::Timestamp* timestamp) {
 TRY
    // not move but copy
   return new mediapipe::Packet{packet->At(*timestamp)};
 CATCH_ONLY
   return nullptr;
}

bool mp_Packet__IsEmpty(mediapipe::Packet* packet) { return packet->IsEmpty(); }

int mp_Packet__ValidateAsProtoMessageLite(mediapipe::Packet* packet, absl::Status** status_out) {
  TRY
    *status_out = new absl::Status{packet->ValidateAsProtoMessageLite()};
  CATCH_EXCEPTION
}

int mp_Packet__Timestamp(mediapipe::Packet* packet, mediapipe::Timestamp** timestamp_out) {
  TRY
    *timestamp_out = new mediapipe::Timestamp{packet->Timestamp()};
  CATCH_EXCEPTION
}

int mp_Packet__DebugString(mediapipe::Packet* packet, const char** str_out) {
  TRY
    *str_out = strcpy_to_heap(packet->DebugString());
  CATCH_EXCEPTION
}

int mp_Packet__RegisteredTypeName(mediapipe::Packet* packet, const char** str_out) {
  TRY
    *str_out = strcpy_to_heap(packet->RegisteredTypeName());
  CATCH_EXCEPTION
}

int mp_Packet__DebugTypeName(mediapipe::Packet* packet, const char** str_out) {
  TRY
    *str_out = strcpy_to_heap(packet->DebugTypeName());
  CATCH_EXCEPTION
}

// BoolPacket
mediapipe::Packet* mp__MakeBoolPacket__b(bool value) {
  TRY
    return new mediapipe::Packet{mediapipe::MakePacket<bool>(value)};
  CATCH_ONLY
    return nullptr;
}

mediapipe::Packet* mp__MakeBoolPacket_At__b_Rt(bool value, mediapipe::Timestamp* timestamp) {
  TRY
    return new mediapipe::Packet{mediapipe::MakePacket<bool>(value).At(*timestamp)};
  CATCH_ONLY
      return nullptr;
}

int mp_Packet__GetBool(mediapipe::Packet* packet, bool* value_out) {
  TRY
    *value_out = packet->Get<bool>();
  CATCH_EXCEPTION
}

int mp_Packet__ValidateAsBool(mediapipe::Packet* packet, absl::Status** status_out) {
  TRY
    *status_out = new absl::Status{packet->ValidateAsType<bool>()};
  CATCH_EXCEPTION
}

// FloatPacket
mediapipe::Packet* mp__MakeFloatPacket__f(float value) {
  TRY
    return new mediapipe::Packet{mediapipe::MakePacket<float>(value)};
  CATCH_ONLY
      return nullptr;
}

mediapipe::Packet* mp__MakeFloatPacket_At__f_Rt(float value, mediapipe::Timestamp* timestamp) {
  TRY
    return new mediapipe::Packet{mediapipe::MakePacket<float>(value).At(*timestamp)};
  CATCH_ONLY
      return nullptr;
}

int mp_Packet__GetFloat(mediapipe::Packet* packet, float* value_out) {
  TRY
    *value_out = packet->Get<float>();
  CATCH_EXCEPTION
}

int mp_Packet__ValidateAsFloat(mediapipe::Packet* packet, absl::Status** status_out) {
  TRY
    *status_out = new absl::Status{packet->ValidateAsType<float>()};
  CATCH_EXCEPTION
}

// IntPacket
mediapipe::Packet* mp__MakeIntPacket__i(int value) {
  TRY
    return new mediapipe::Packet{mediapipe::MakePacket<int>(value)};
  CATCH_ONLY
      return nullptr;
}

mediapipe::Packet* mp__MakeIntPacket_At__i_Rt(int value, mediapipe::Timestamp* timestamp) {
  TRY
    return new mediapipe::Packet{mediapipe::MakePacket<int>(value).At(*timestamp)};
  CATCH_ONLY
      return nullptr;
}

int mp_Packet__GetInt(mediapipe::Packet* packet, int* value_out) {
    TRY
        *value_out = packet->Get<int>();
    CATCH_EXCEPTION
}

int mp_Packet__ValidateAsInt(mediapipe::Packet* packet, absl::Status** status_out) {
  TRY
    *status_out = new absl::Status{packet->ValidateAsType<int>()};
  CATCH_EXCEPTION
}

// FloatArrayPacket
mediapipe::Packet* mp__MakeFloatArrayPacket__Pf_i(float* value, int size) {
  TRY
    float* array = new float[size];
    std::memcpy(array, value, size * sizeof(float));
    return new mediapipe::Packet{mediapipe::Adopt(reinterpret_cast<float(*)[]>(array))};
    CATCH_ONLY
        return nullptr;
}

mediapipe::Packet* mp__MakeFloatArrayPacket_At__Pf_i_Rt(float* value, int size, mediapipe::Timestamp* timestamp) {
  TRY
        float* array = new float[size];
        std::memcpy(array, value, size * sizeof(float));
        return new mediapipe::Packet{mediapipe::Adopt(reinterpret_cast<float(*)[]>(array)).At(*timestamp)};
    CATCH_ONLY
        return nullptr;
}

int mp_Packet__GetFloatArray(mediapipe::Packet* packet, const float** value_out) {
    TRY
    *value_out = packet->Get<float[]>();
    CATCH_EXCEPTION
}

int mp_Packet__ValidateAsFloatArray(mediapipe::Packet* packet, absl::Status** status_out) {
  TRY
    *status_out = new absl::Status{packet->ValidateAsType<float[]>()};
  CATCH_EXCEPTION
}

// StringPacket
mediapipe::Packet* mp__MakeStringPacket__PKc(const char* str) {
  TRY
    return new mediapipe::Packet{mediapipe::MakePacket<std::string>(std::string(str))};
  CATCH_ONLY
      return nullptr;
}

mediapipe::Packet* mp__MakeStringPacket_At__PKc_Rt(const char* str, mediapipe::Timestamp* timestamp) {
  TRY
    return new mediapipe::Packet{mediapipe::MakePacket<std::string>(std::string(str)).At(*timestamp)};
  CATCH_ONLY
      return nullptr;
}

mediapipe::Packet* mp__MakeStringPacket__PKc_i(const char* str, int size) {
  TRY
    return new mediapipe::Packet{mediapipe::MakePacket<std::string>(std::string(str, size))};
  CATCH_ONLY
      return nullptr;
}

mediapipe::Packet* mp__MakeStringPacket_At__PKc_i_Rt(const char* str, int size, mediapipe::Timestamp* timestamp) {
  TRY
    return new mediapipe::Packet{mediapipe::MakePacket<std::string>(std::string(str, size)).At(*timestamp)};
  CATCH_ONLY
      return nullptr;
}

int mp_Packet__GetString(mediapipe::Packet* packet, const char** value_out) {
  TRY
    *value_out = strcpy_to_heap(packet->Get<std::string>());
 CATCH_EXCEPTION
}

int mp_Packet__GetByteString(mediapipe::Packet* packet, const char** value_out, int* size_out) {
  TRY
    auto& str = packet->Get<std::string>();
    auto length = str.size();
    auto bytes = new char[length];
    memcpy(bytes, str.c_str(), length);

    *value_out = bytes;
    *size_out = length;
    CATCH_EXCEPTION
}

int mp_Packet__ConsumeString(mediapipe::Packet* packet, absl::StatusOr<std::string>** status_or_value_out) {
  TRY
    auto status_or_string = packet->Consume<std::string>();

    if (status_or_string.ok()) {
      *status_or_value_out = new absl::StatusOr<std::string>{std::move(*status_or_string.value().release())};
    } else {
      *status_or_value_out = new absl::StatusOr<std::string>{status_or_string.status()};
    }
    CATCH_EXCEPTION
}

int mp_Packet__ValidateAsString(mediapipe::Packet* packet, absl::Status** status_out) {
  TRY
    *status_out = new absl::Status{packet->ValidateAsType<std::string>()};
  CATCH_EXCEPTION
}


void mp_SidePacket__delete(SidePacket* side_packet) { delete side_packet; }

int mp_SidePacket__emplace__PKc_Rp(SidePacket* side_packet, const char* key, mediapipe::Packet* packet) {
  TRY
    side_packet->emplace(std::string(key), std::move(*packet));
  CATCH_EXCEPTION
}

int mp_SidePacket__at__PKc(SidePacket* side_packet, const char* key, mediapipe::Packet** packet_out) {
  TRY
    auto packet = side_packet->at(std::string(key));
    // copy
    *packet_out = new mediapipe::Packet{packet};
  CATCH_EXCEPTION
}

  int mp_SidePacket__erase__PKc(SidePacket* side_packet, const char* key, int* count_out) {
  TRY
    *count_out = side_packet->erase(std::string(key));
  CATCH_EXCEPTION
}

void mp_SidePacket__clear(SidePacket* side_packet) { side_packet->clear(); }

int mp_SidePacket__size(SidePacket* side_packet) { return side_packet->size(); }
