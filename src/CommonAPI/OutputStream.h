/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_OUTPUT_STREAM_H_
#define COMMONAPI_OUTPUT_STREAM_H_

#include "ByteBuffer.h"
#include "SerializableStruct.h"
#include "types.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <type_traits>

namespace CommonAPI {

class OutputStream {
 public:
    virtual ~OutputStream() {}
    virtual bool hasError() const = 0;

	virtual OutputStream& writeValue(const bool& boolValue) = 0;

	virtual OutputStream& writeValue(const int8_t& int8Value) = 0;
	virtual OutputStream& writeValue(const int16_t& int16Value) = 0;
	virtual OutputStream& writeValue(const int32_t& int32Value) = 0;
	virtual OutputStream& writeValue(const int64_t& int64Value) = 0;

	virtual OutputStream& writeValue(const uint8_t& uint8Value) = 0;
	virtual OutputStream& writeValue(const uint16_t& uint16Value) = 0;
	virtual OutputStream& writeValue(const uint32_t& uint32Value) = 0;
	virtual OutputStream& writeValue(const uint64_t& uint64Value) = 0;

	virtual OutputStream& writeValue(const float& floatValue) = 0;
	virtual OutputStream& writeValue(const double& doubleValue) = 0;

	virtual OutputStream& writeValue(const std::string& stringValue) = 0;

	virtual OutputStream& writeValue(const ByteBuffer& byteBufferValue) = 0;

 	virtual OutputStream& writeEnumValue(const int8_t& int8BackingTypeValue) = 0;
 	virtual OutputStream& writeEnumValue(const int16_t& int16BackingTypeValue) = 0;
 	virtual OutputStream& writeEnumValue(const int32_t& int32BackingTypeValue) = 0;
 	virtual OutputStream& writeEnumValue(const int64_t& int64BackingTypeValue) = 0;
 	virtual OutputStream& writeEnumValue(const uint8_t& uint8BackingTypeValue) = 0;
 	virtual OutputStream& writeEnumValue(const uint16_t& uint16BackingTypeValue) = 0;
 	virtual OutputStream& writeEnumValue(const uint32_t& uint32BackingTypeValue) = 0;
 	virtual OutputStream& writeEnumValue(const uint64_t& uint64BackingTypeValue) = 0;

 	virtual OutputStream& writeVersionValue(const Version& versionValue) = 0;

 	virtual void beginWriteSerializableStruct(const SerializableStruct& serializableStruct) = 0;
	virtual void endWriteSerializableStruct(const SerializableStruct& serializableStruct) = 0;

    virtual void beginWriteBoolVector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteInt8Vector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteInt16Vector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteInt32Vector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteInt64Vector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteUInt8Vector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteUInt16Vector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteUInt32Vector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteUInt64Vector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteFloatVector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteDoubleVector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteStringVector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteByteBufferVector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteVersionVector(uint32_t sizeOfVector) = 0;
    virtual void beginWriteVectorOfSerializableStructs(uint32_t sizeOfVector) = 0;
    virtual void beginWriteVectorOfVectors(uint32_t sizeOfVector) = 0;
    virtual void beginWriteVectorOfMaps(uint32_t sizeOfVector) = 0;

    virtual void endWriteVector() = 0;

	virtual void beginWriteMap(size_t elementCount) = 0;
	virtual void endWriteMap() = 0;
};


inline OutputStream& operator<<(OutputStream& outputStream, const bool& boolValue) {
	return outputStream.writeValue(boolValue);
}

inline OutputStream& operator<<(OutputStream& outputStream, const int8_t& int8Value) {
	return outputStream.writeValue(int8Value);
}

inline OutputStream& operator<<(OutputStream& outputStream, const int16_t& int16Value) {
	return outputStream.writeValue(int16Value);
}

inline OutputStream& operator<<(OutputStream& outputStream, const int32_t& int32Value) {
	return outputStream.writeValue(int32Value);
}

inline OutputStream& operator<<(OutputStream& outputStream, const int64_t& int64Value) {
	return outputStream.writeValue(int64Value);
}

inline OutputStream& operator<<(OutputStream& outputStream, const uint8_t& uint8Value) {
	return outputStream.writeValue(uint8Value);
}

inline OutputStream& operator<<(OutputStream& outputStream, const uint16_t& uint16Value) {
	return outputStream.writeValue(uint16Value);
}

inline OutputStream& operator<<(OutputStream& outputStream, const uint32_t& uint32Value) {
	return outputStream.writeValue(uint32Value);
}

inline OutputStream& operator<<(OutputStream& outputStream, const uint64_t& uint64Value) {
	return outputStream.writeValue(uint64Value);
}

inline OutputStream& operator<<(OutputStream& outputStream, const float& floatValue) {
	return outputStream.writeValue(floatValue);
}

inline OutputStream& operator<<(OutputStream& outputStream, const double& doubleValue) {
	return outputStream.writeValue(doubleValue);
}

inline OutputStream& operator<<(OutputStream& outputStream, const std::string& stringValue) {
	return outputStream.writeValue(stringValue);
}

inline OutputStream& operator<<(OutputStream& outputStream, const ByteBuffer& byteBufferValue) {
	return outputStream.writeValue(byteBufferValue);
}

inline OutputStream& operator<<(OutputStream& outputStream, const Version& versionValue) {
	return outputStream.writeVersionValue(versionValue);
}

inline OutputStream& operator<<(OutputStream& outputStream, const SerializableStruct& serializableStruct) {
    outputStream.beginWriteSerializableStruct(serializableStruct);
    serializableStruct.writeToOutputStream(outputStream);
    outputStream.endWriteSerializableStruct(serializableStruct);

    return outputStream;
}


template <typename _VectorElementType>
class OutStreamGenericVectorHelper {
 public:
    static void beginWriteVector(OutputStream& outputStream, const std::vector<_VectorElementType>& vectorValue) {
        doBeginWriteVector(outputStream, vectorValue);
    }

 private:
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<bool>& vectorValue) {
        outputStream.beginWriteBoolVector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<int8_t>& vectorValue) {
        outputStream.beginWriteInt8Vector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<int16_t>& vectorValue) {
        outputStream.beginWriteInt16Vector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<int32_t>& vectorValue) {
        outputStream.beginWriteInt32Vector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<int64_t>& vectorValue) {
        outputStream.beginWriteInt64Vector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<uint8_t>& vectorValue) {
        outputStream.beginWriteUInt8Vector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<uint16_t>& vectorValue) {
        outputStream.beginWriteUInt16Vector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<uint32_t>& vectorValue) {
        outputStream.beginWriteUInt32Vector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<uint64_t>& vectorValue) {
        outputStream.beginWriteUInt64Vector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<float>& vectorValue) {
        outputStream.beginWriteFloatVector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<double>& vectorValue) {
        outputStream.beginWriteDoubleVector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<std::string>& vectorValue) {
        outputStream.beginWriteStringVector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<ByteBuffer>& vectorValue) {
        outputStream.beginWriteByteBufferVector(vectorValue.size());
    }
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<Version>& vectorValue) {
        outputStream.beginWriteVersionVector(vectorValue.size());
    }

    template<typename _InnerVectorElementType>
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<std::vector<_InnerVectorElementType>>& vectorValue) {
        outputStream.beginWriteVectorOfVectors(vectorValue.size());
    }

    template<typename _InnerKeyType, typename _InnerValueType>
    static inline void doBeginWriteVector(OutputStream& outputStream, const std::vector<std::unordered_map<_InnerKeyType, _InnerValueType>>& vectorValue) {
        outputStream.beginWriteVectorOfMaps(vectorValue.size());
    }
};


template <typename _VectorElementType, bool _IsSerializableStruct = false>
struct OutputStreamSerializableStructVectorHelper: public OutStreamGenericVectorHelper<_VectorElementType> {
};

template <typename _VectorElementType>
struct OutputStreamSerializableStructVectorHelper<_VectorElementType, true> {
    static void beginWriteVector(OutputStream& outputStream, const std::vector<_VectorElementType>& vectorValue) {
        outputStream.beginWriteVectorOfSerializableStructs(vectorValue.size());
    }
};


template <typename _VectorElementType>
struct OutputStreamVectorHelper: OutputStreamSerializableStructVectorHelper<_VectorElementType,
                                                                            std::is_base_of<SerializableStruct, _VectorElementType>::value> {
};


/**
 * Handles all writing of vectors to a given #OutputStream. The given vector may contain any types that are
 * defined as basic types within the context of CommonAPI, as well as any types derived from those basic types.
 *
 * @tparam _ElementType The type of the elements that are contained in the given vector.
 * @param val The vector that is to be written into the given stream
 * @param outputStream The stream which is to be written into
 * @return The given outputStream to allow for successive writing.
 * @see operator<<(OutputStream&, const _BasicType&)
 * @see SerializableStruct
 * @see SerializableVariant
 */
template<typename _VectorElementType>
OutputStream& operator<<(OutputStream& outputStream, const std::vector<_VectorElementType>& vectorValue) {
    OutputStreamVectorHelper<_VectorElementType>::beginWriteVector(outputStream, vectorValue);
    const size_t elementCount = vectorValue.size();

    for (size_t i = 0; i < elementCount; i++) {
        outputStream << vectorValue[i];

        if (outputStream.hasError())
            break;
    }

    outputStream.endWriteVector();
	return outputStream;
}


template<typename _KeyType, typename _ValueType>
OutputStream& operator<<(OutputStream& outputStream, const std::unordered_map<_KeyType, _ValueType>& mapValue) {
	typedef typename std::unordered_map<_KeyType, _ValueType>::const_iterator MapConstIterator;

	const size_t elementCount = mapValue.size();

	outputStream.beginWriteMap(elementCount);

	for (MapConstIterator iter = mapValue.cbegin(); iter != mapValue.cend(); iter++) {
		outputStream << iter->first << iter->second;

		if (outputStream.hasError())
			return outputStream;
	}

	outputStream.endWriteMap();

	return outputStream;
}


} // namespace CommonAPI

#endif // COMMONAPI_OUTPUT_STREAM_H_
