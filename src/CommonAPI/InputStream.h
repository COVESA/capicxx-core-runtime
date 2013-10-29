/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_INPUT_STREAM_H_
#define COMMONAPI_INPUT_STREAM_H_

#include "ByteBuffer.h"
#include "SerializableStruct.h"
#include "SerializableVariant.h"
#include "types.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <type_traits>

namespace CommonAPI {

class InputStream {
 public:
    virtual ~InputStream() { }
    virtual bool hasError() const = 0;

 	virtual InputStream& readValue(bool& boolValue) = 0;

 	virtual InputStream& readValue(int8_t& int8Value) = 0;
 	virtual InputStream& readValue(int16_t& int16Value) = 0;
 	virtual InputStream& readValue(int32_t& int32Value) = 0;
 	virtual InputStream& readValue(int64_t& int64Value) = 0;

 	virtual InputStream& readValue(uint8_t& uint8Value) = 0;
 	virtual InputStream& readValue(uint16_t& uint16Value) = 0;
 	virtual InputStream& readValue(uint32_t& uint32Value) = 0;
 	virtual InputStream& readValue(uint64_t& uint64Value) = 0;

 	virtual InputStream& readValue(float& floatValue) = 0;
 	virtual InputStream& readValue(double& doubleValue) = 0;

 	virtual InputStream& readValue(std::string& stringValue) = 0;

 	virtual InputStream& readValue(ByteBuffer& byteBufferValue) = 0;

 	virtual InputStream& readEnumValue(int8_t& int8BackingTypeValue) = 0;
 	virtual InputStream& readEnumValue(int16_t& int16BackingTypeValue) = 0;
 	virtual InputStream& readEnumValue(int32_t& int32BackingTypeValue) = 0;
 	virtual InputStream& readEnumValue(int64_t& int64BackingTypeValue) = 0;
 	virtual InputStream& readEnumValue(uint8_t& uint8BackingTypeValue) = 0;
 	virtual InputStream& readEnumValue(uint16_t& uint16BackingTypeValue) = 0;
 	virtual InputStream& readEnumValue(uint32_t& uint32BackingTypeValue) = 0;
 	virtual InputStream& readEnumValue(uint64_t& uint64BackingTypeValue) = 0;

 	template <typename _EnumBackingType, typename _EnumType>
 	inline InputStream& readEnumValue(_EnumType& enumValue);

 	virtual InputStream& readVersionValue(Version& versionValue) = 0;

 	virtual void beginReadSerializableStruct(const SerializableStruct& serializableStruct) = 0;
 	virtual void endReadSerializableStruct(const SerializableStruct& serializableStruct) = 0;

 	virtual void beginReadSerializablePolymorphicStruct(uint32_t& serialId) = 0;
 	virtual void endReadSerializablePolymorphicStruct(const uint32_t& serialId) = 0;

    virtual void readSerializableVariant(SerializableVariant& serializableVariant) = 0;

    virtual char* readRawData(const size_t numBytesToRead) = 0;

    virtual void beginReadBoolVector() = 0;
    virtual void beginReadInt8Vector() = 0;
    virtual void beginReadInt16Vector() = 0;
    virtual void beginReadInt32Vector() = 0;
    virtual void beginReadInt64Vector() = 0;
    virtual void beginReadUInt8Vector() = 0;
    virtual void beginReadUInt16Vector() = 0;
    virtual void beginReadUInt32Vector() = 0;
    virtual void beginReadUInt64Vector() = 0;
    virtual void beginReadFloatVector() = 0;
    virtual void beginReadDoubleVector() = 0;
    virtual void beginReadStringVector() = 0;
    virtual void beginReadByteBufferVector() = 0;
    virtual void beginReadVersionVector() = 0;

    virtual void beginReadInt8EnumVector() = 0;
    virtual void beginReadInt16EnumVector() = 0;
    virtual void beginReadInt32EnumVector() = 0;
    virtual void beginReadInt64EnumVector() = 0;
    virtual void beginReadUInt8EnumVector() = 0;
    virtual void beginReadUInt16EnumVector() = 0;
    virtual void beginReadUInt32EnumVector() = 0;
    virtual void beginReadUInt64EnumVector() = 0;

    virtual void beginReadVectorOfSerializableStructs() = 0;
    virtual void beginReadVectorOfSerializableVariants() = 0;
    virtual void beginReadVectorOfVectors() = 0;
    virtual void beginReadVectorOfMaps() = 0;

    virtual bool hasMoreVectorElements() = 0;
    virtual void endReadVector() = 0;

 	virtual void beginReadMap() = 0;
 	virtual bool hasMoreMapElements() = 0;
 	virtual void endReadMap() = 0;
 	virtual void beginReadMapElement() = 0;
 	virtual void endReadMapElement() = 0;
};

template <typename _EnumBackingType, typename _EnumType>
InputStream& InputStream::readEnumValue(_EnumType& enumValue) {
	_EnumBackingType enumBackingValue;

	readEnumValue(enumBackingValue);
	enumValue = static_cast<_EnumType>(enumBackingValue);

	return *this;
}


inline InputStream& operator>>(InputStream& inputStream, bool& boolValue) {
	return inputStream.readValue(boolValue);
}

inline InputStream& operator>>(InputStream& inputStream, int8_t& int8Value) {
	return inputStream.readValue(int8Value);
}

inline InputStream& operator>>(InputStream& inputStream, int16_t& int16Value) {
	return inputStream.readValue(int16Value);
}

inline InputStream& operator>>(InputStream& inputStream, int32_t& int32Value) {
	return inputStream.readValue(int32Value);
}

inline InputStream& operator>>(InputStream& inputStream, int64_t& int64Value) {
	return inputStream.readValue(int64Value);
}

inline InputStream& operator>>(InputStream& inputStream, uint8_t& uint8Value) {
	return inputStream.readValue(uint8Value);
}

inline InputStream& operator>>(InputStream& inputStream, uint16_t& uint16Value) {
	return inputStream.readValue(uint16Value);
}

inline InputStream& operator>>(InputStream& inputStream, uint32_t& uint32Value) {
	return inputStream.readValue(uint32Value);
}

inline InputStream& operator>>(InputStream& inputStream, uint64_t& uint64Value) {
	return inputStream.readValue(uint64Value);
}

inline InputStream& operator>>(InputStream& inputStream, float& floatValue) {
	return inputStream.readValue(floatValue);
}

inline InputStream& operator>>(InputStream& inputStream, double& doubleValue) {
	return inputStream.readValue(doubleValue);
}

inline InputStream& operator>>(InputStream& inputStream, std::string& stringValue) {
	return inputStream.readValue(stringValue);
}

inline InputStream& operator>>(InputStream& inputStream, Version& versionValue) {
	return inputStream.readVersionValue(versionValue);
}

inline InputStream& operator>>(InputStream& inputStream, SerializableStruct& serializableStruct) {
    inputStream.beginReadSerializableStruct(serializableStruct);
    serializableStruct.readFromInputStream(inputStream);
    inputStream.endReadSerializableStruct(serializableStruct);

    return inputStream;
}

template <typename _SerializablePolymorphicStructType>
typename std::enable_if<std::is_base_of<SerializablePolymorphicStruct, _SerializablePolymorphicStructType>::value, InputStream>::type&
operator>>(InputStream& inputStream, std::shared_ptr<_SerializablePolymorphicStructType>& serializablePolymorphicStruct) {
	uint32_t serialId;

	inputStream.beginReadSerializablePolymorphicStruct(serialId);
	if (!inputStream.hasError()) {
		_SerializablePolymorphicStructType* instancePtr = _SerializablePolymorphicStructType::createInstance(serialId);
		serializablePolymorphicStruct.reset(instancePtr);
		if (instancePtr != NULL) {
			instancePtr->readFromInputStream(inputStream);
		}

		inputStream.endReadSerializablePolymorphicStruct(serialId);
	}

	return inputStream;
}

inline InputStream& operator>>(InputStream& inputStream, SerializableVariant& serializableVariant) {
    inputStream.readSerializableVariant(serializableVariant);
    return inputStream;
}


template <typename _VectorElementType>
class InputStreamGenericTypeVectorHelper {
 public:
    static void beginReadVector(InputStream& inputStream, const std::vector<_VectorElementType>& vectorValue) {
        doBeginReadVector(inputStream, vectorValue);
    }

 private:
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<bool>& vectorValue) {
        inputStream.beginReadBoolVector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<int8_t>& vectorValue) {
        inputStream.beginReadInt8Vector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<int16_t>& vectorValue) {
        inputStream.beginReadInt16Vector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<int32_t>& vectorValue) {
        inputStream.beginReadInt32Vector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<int64_t>& vectorValue) {
        inputStream.beginReadInt64Vector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<uint8_t>& vectorValue) {
        inputStream.beginReadUInt8Vector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<uint16_t>& vectorValue) {
        inputStream.beginReadUInt16Vector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<uint32_t>& vectorValue) {
        inputStream.beginReadUInt32Vector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<uint64_t>& vectorValue) {
        inputStream.beginReadUInt64Vector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<float>& vectorValue) {
        inputStream.beginReadFloatVector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<double>& vectorValue) {
        inputStream.beginReadDoubleVector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<std::string>& vectorValue) {
        inputStream.beginReadStringVector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<ByteBuffer>& vectorValue) {
        inputStream.beginReadByteBufferVector();
    }
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<Version>& vectorValue) {
        inputStream.beginReadVersionVector();
    }

    template<typename _InnerVectorElementType>
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<std::vector<_InnerVectorElementType>>& vectorValue) {
        inputStream.beginReadVectorOfVectors();
    }

    template<typename _InnerKeyType, typename _InnerValueType>
    static inline void doBeginReadVector(InputStream& inputStream, const std::vector<std::unordered_map<_InnerKeyType, _InnerValueType>>& vectorValue) {
        inputStream.beginReadVectorOfMaps();
    }
};


template <typename _VectorElementType, bool _IsSerializableStruct = false>
struct InputStreamSerializableStructVectorHelper: InputStreamGenericTypeVectorHelper<_VectorElementType> {
};

template <typename _VectorElementType>
struct InputStreamSerializableStructVectorHelper<_VectorElementType, true> {
    static void beginReadVector(InputStream& inputStream, const std::vector<_VectorElementType>& vectorValue) {
        inputStream.beginReadVectorOfSerializableStructs();
    }
};


template <typename _VectorElementType, bool _IsSerializableVariant = false>
struct InputStreamSerializableVariantVectorHelper: InputStreamSerializableStructVectorHelper<_VectorElementType,
                                                                                             std::is_base_of<SerializableStruct, _VectorElementType>::value> {
};

template <typename _VectorElementType>
struct InputStreamSerializableVariantVectorHelper<_VectorElementType, true> {
    static void beginReadVector(InputStream& inputStream, const std::vector<_VectorElementType>& vectorValue) {
        inputStream.beginReadVectorOfSerializableVariants();
    }
};


template <typename _VectorElementType>
struct InputStreamVectorHelper: InputStreamSerializableVariantVectorHelper<_VectorElementType,
                                                                           std::is_base_of<SerializableVariant, _VectorElementType>::value> {
};

/**
 * Handles all reading of vectors from a given #InputStream. The given vector may contain any types for which a
 * (specialized) operator>>() is provided. For basic types, such an operator already is provided as a templated operator.
 * The vector does not need to be initialized in any way.
 *
 * @tparam _ElementType The type of the values that are contained in the vector that is to be read from the given stream.
 * @param val The vector in which the retrieved values are to be stored
 * @param inputStream The stream which the vector is to be read from
 * @return The given inputStream to allow for successive reading
 */
template<typename _VectorElementType>
InputStream& operator>>(InputStream& inputStream, std::vector<_VectorElementType>& vectorValue) {
    InputStreamVectorHelper<_VectorElementType>::beginReadVector(inputStream, vectorValue);

    while (inputStream.hasMoreVectorElements()) {
        _VectorElementType element;

        inputStream >> element;

        if (inputStream.hasError())
            break;

        vectorValue.push_back(std::move(element));
    }

    inputStream.endReadVector();
    return inputStream;
}

template<typename _KeyType, typename _ValueType, typename _HasherType>
InputStream& operator>>(InputStream& inputStream, std::unordered_map<_KeyType, _ValueType, _HasherType>& mapValue) {
    typedef typename std::unordered_map<_KeyType, _ValueType, _HasherType>::value_type MapValueType;

    inputStream.beginReadMap();

    while (inputStream.hasMoreMapElements()) {
        _KeyType elementKey;
        _ValueType elementValue;

        inputStream.beginReadMapElement();
        inputStream >> elementKey >> elementValue;
        inputStream.endReadMapElement();

        if (inputStream.hasError())
            break;

        mapValue.insert(MapValueType(std::move(elementKey), std::move(elementValue)));
    }

    inputStream.endReadMap();

    return inputStream;
}

} // namespace CommonAPI

#endif // COMMONAPI_INPUT_STREAM_H_
