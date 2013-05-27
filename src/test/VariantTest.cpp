/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include <gtest/gtest.h>
#include <src/CommonAPI/SerializableVariant.h>

using namespace CommonAPI;

class VariantTest: public ::testing::Test {

  void SetUp() {
  }

  void TearDown() {
  }
};

TEST_F(VariantTest, VariantTestPack) {

    int fromInt = 5;
    double fromDouble = 12.344d;
    std::string fromString = "123abcsadfaljkawlöfasklöerklöfjasklfjysklfjaskfjsklösdfdko4jdfasdjioögjopefgip3rtgjiprg!";
    Variant<int, double, std::string> myVariant(fromInt);

    Variant<int, double, std::string>* myVariants = new Variant<int, double, std::string>(fromString);

    Variant<int, double, std::string> myVariantf(fromDouble);

    std::string myString = myVariants->get<std::string>();
    std::cout << "myString = " << myString << "\n";


    const int& myInt = myVariant.get<int>();
    std::cout << "myInt = " << myInt << "\n";

    int vType = myVariant.getValueType();
    std::cout << "myVariant value type = " << vType << ", should be 0 \n";

    Variant<int, double, std::string> myVariant2 = myVariant;
    const int& myInt2 = myVariant2.get<int>();
    std::cout << "myInt2 = " << myInt2 << "\n";

    Variant<int, double, std::string> myVariant3 = fromInt;
    const int& myInt3 = myVariant3.get<int>();
    std::cout << "myInt3 = " << myInt3 << "\n"; "\n";

    myString = myVariants->get<std::string>();
    std::cout << "myString = " << myString << "\n";

    Variant<int, double, std::string> myVariantCopy(myVariant);
    const int& myIntCopy = myVariantCopy.get<int>();
    std::cout << "myIntCopy = " << myIntCopy << "\n";

    std::cout << "myIntCopy equals myInt " << "(" << std::boolalpha << (myVariant == myVariantCopy) << ")\n";

    Variant<int, double, std::string> myVariantCopy2;
    myVariantCopy2 = myVariant;

    const int& myIntCopy2 = myVariantCopy2.get<int>();
    std::cout << "myIntCopy2 = " << myIntCopy << "\n";

    std::cout << "myIntCopy2 equals myInt " << "(" << std::boolalpha << (myVariant == myVariantCopy2) << ")\n";


    EXPECT_ANY_THROW(const int& myFake = myVariant.get<double>());

    std::cout << "myInt is int = " << " (" << std::boolalpha << myVariant.isType<int>() << ")\n";
    EXPECT_TRUE(myVariant.isType<int>());

    std::cout << "myInt is std::string = " << "\n";
    EXPECT_FALSE(myVariant.isType<std::string>());

    const double& myDouble = myVariantf.get<double>();
    std::cout << "myDouble = " << myDouble << "\n";

    Variant<int, double, std::string> myVariantsCopy(*myVariants);
    std::string myStringCopy = myVariantsCopy.get<std::string>();
    std::cout << "myStringCopy = " << myStringCopy << "\n";

    *myVariants = std::string("Hello World");
    myString = myVariants->get<std::string>();
    std::cout << "myString = " << myString << "\n";

    myStringCopy = myVariantsCopy.get<std::string>();
    std::cout << "myStringCopy = " << myStringCopy << "\n";

    delete myVariants;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
