/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "InputStream.h"

namespace CommonAPI {

InputStream& operator>>(InputStream& inputStream, bool& boolValue) {
    return inputStream.readValue(boolValue);
}

InputStream& operator>>(InputStream& inputStream, int8_t& int8Value) {
    return inputStream.readValue(int8Value);
}

InputStream& operator>>(InputStream& inputStream, int16_t& int16Value) {
    return inputStream.readValue(int16Value);
}

InputStream& operator>>(InputStream& inputStream, int32_t& int32Value) {
    return inputStream.readValue(int32Value);
}

InputStream& operator>>(InputStream& inputStream, int64_t& int64Value) {
    return inputStream.readValue(int64Value);
}

InputStream& operator>>(InputStream& inputStream, uint8_t& uint8Value) {
    return inputStream.readValue(uint8Value);
}

InputStream& operator>>(InputStream& inputStream, uint16_t& uint16Value) {
    return inputStream.readValue(uint16Value);
}

InputStream& operator>>(InputStream& inputStream, uint32_t& uint32Value) {
    return inputStream.readValue(uint32Value);
}

InputStream& operator>>(InputStream& inputStream, uint64_t& uint64Value) {
    return inputStream.readValue(uint64Value);
}

InputStream& operator>>(InputStream& inputStream, float& floatValue) {
    return inputStream.readValue(floatValue);
}

InputStream& operator>>(InputStream& inputStream, double& doubleValue) {
    return inputStream.readValue(doubleValue);
}

InputStream& operator>>(InputStream& inputStream, std::string& stringValue) {
    return inputStream.readValue(stringValue);
}

InputStream& operator>>(InputStream& inputStream, Version& versionValue) {
    return inputStream.readVersionValue(versionValue);
}

InputStream& operator>>(InputStream& inputStream, SerializableStruct& serializableStruct) {
    inputStream.beginReadSerializableStruct(serializableStruct);
    serializableStruct.readFromInputStream(inputStream);
    inputStream.endReadSerializableStruct(serializableStruct);

    return inputStream;
}

InputStream& operator>>(InputStream& inputStream, SerializableVariant& serializableVariant) {
    inputStream.readSerializableVariant(serializableVariant);
    return inputStream;
}

}
