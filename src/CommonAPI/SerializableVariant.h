/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_SERIALIZABLE_VARIANT_H_
#define COMMONAPI_SERIALIZABLE_VARIANT_H_

#include "Stream.h"

#include <memory>
#include <cstdint>

#ifndef __COMMON_API_EXPERIMENTAL
#error "Use -D__COMMON_API_EXPERIMENTAL to include experimental variant support"
#endif

namespace CommonAPI {

class SerializableVariant {
 public:
	virtual ~SerializableVariant() { }

 	inline uint32_t getType() const;

 	virtual void readFromInputStream(InputStream& inputStream) = 0;
	virtual void writeToOutputStream(OutputStream& outputStream) = 0;

 protected:
	uint32_t type_;
	std::shared_ptr<void> value_;
};

uint32_t SerializableVariant::getType() const {
	return type_;
}

} // namespace CommonAPI

#endif // COMMONAPI_SERIALIZABLE_VARIANT_H_
