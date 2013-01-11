/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_TYPES_H_
#define COMMONAPI_TYPES_H_

#include <cstdint>


namespace CommonAPI {

enum class AvailabilityStatus {
	AVAILABLE,
	NOT_AVAILABLE
};


enum class CallStatus {
	SUCCESS,
	OUT_OF_MEMORY,
	NOT_AVAILABLE,
	CONNECTION_FAILED,
	REMOTE_ERROR
};


struct Version {
	Version() = default;

	Version(const uint32_t& majorValue, const uint32_t& minorValue):
			Major(majorValue),
			Minor(minorValue) { }

	uint32_t Major;
	uint32_t Minor;
};


} // namespace CommonAPI

#endif // COMMONAPI_TYPES_H_
