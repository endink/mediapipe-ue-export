// Copyright (c) 2021 homuler
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "protobuf.h"

#include <functional>
#include <string>

void mp_api_SerializedProtoArray__delete(SerializedProto* serialized_proto_vector_data, int size) {
    auto serialized_proto = serialized_proto_vector_data;
    for (auto i = 0; i < size; ++i) {
        delete (serialized_proto++)->str;
    }
    delete[] serialized_proto_vector_data;
}
