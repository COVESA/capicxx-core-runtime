/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef VERIFICATIONTESTSTUB_H_
#define VERIFICATIONTESTSTUB_H_

#include "commonapi/tests/TestInterfaceStubDefault.h"

namespace commonapi {
namespace verification {

class VerificationTestStub : public commonapi::tests::TestInterfaceStubDefault {
public:
    VerificationTestStub();

    int getCalledTestDerivedTypeMethod() const {
        return calledTestDerivedTypeMethod;
    }
private:
    virtual void testDerivedTypeMethod(
                    commonapi::tests::DerivedTypeCollection::TestEnumExtended2 testEnumExtended2InValue,
                    commonapi::tests::DerivedTypeCollection::TestMap testMapInValue,
                    commonapi::tests::DerivedTypeCollection::TestEnumExtended2& testEnumExtended2OutValue,
                    commonapi::tests::DerivedTypeCollection::TestMap& testMapOutValue);

    virtual void testPredefinedTypeMethod(const CommonAPI::ClientId& clientId,
                                          uint32_t uint32InValue,
                                          std::string stringInValue,
                                          uint32_t& uint32OutValue,
                                          std::string& stringOutValue);

    int calledTestDerivedTypeMethod;
};

} /* namespace verification */
} /* namespace commonapi */
#endif /* VERIFICATIONTESTSTUB_H_ */