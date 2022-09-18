// Copyright (c) 2021 homuler
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#ifndef MEDIAPIPE_API_EXTERNAL_PROTOBUF_H_
#define MEDIAPIPE_API_EXTERNAL_PROTOBUF_H_

#include <iomanip>
#include <sstream>
#include <vector>

#include "ump_commons.h"
#include "mediapipe/framework/port/parse_text_proto.h"

template <typename T>
struct StructArray {
    T* data;
    int size;
};

template <class T>
inline void SerializeProto(const T& proto, SerializedProto* serialized_proto) {
    auto str = proto.SerializeAsString();
    auto size = str.size();
    auto bytes = new char[size + 1];
    memcpy(bytes, str.c_str(), size);

    serialized_proto->str = bytes;
    serialized_proto->length = static_cast<int>(size);
}

template <class T>
inline void SerializeProtoVector(const std::vector<T>& proto_vec, SerializedProtoArray* serialized_proto_vector) {
    auto vec_size = proto_vec.size();
    auto data = new SerializedProto[vec_size];

    for (auto i = 0; i < vec_size; ++i) {
        SerializeProto(proto_vec[i], &data[i]);
    }
    serialized_proto_vector->data = data;
    serialized_proto_vector->size = static_cast<int>(vec_size);
}

template <class T>
inline bool ConvertFromTextFormat(const char* str, SerializedProto* output) {
    T proto;
    auto result = google::protobuf::TextFormat::ParseFromString(str, &proto);

    if (result) {
        SerializeProto(proto, output);
    }
    return result;
}

inline void DeleteSerializedProtoArray(SerializedProto* serialized_proto_vector_data, int size) {
    auto serialized_proto = serialized_proto_vector_data;
    for (auto i = 0; i < size; ++i) {
        delete (serialized_proto++)->str;
    }
    delete[] serialized_proto_vector_data;
}


#endif