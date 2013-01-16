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

    bool success;

    std::string myString = myVariants->get<std::string>(success);
    std::cout << "myString = " << myString << " (" << std::boolalpha << success << ")\n";
    EXPECT_TRUE(success);

    const int& myInt = myVariant.get<int>(success);
    std::cout << "myInt = " << myInt << " (" << std::boolalpha << success << ")\n";
    EXPECT_TRUE(success);

    Variant<int, double, std::string> myVariant2 = myVariant;
    const int& myInt2 = myVariant2.get<int>(success);
    std::cout << "myInt2 = " << myInt2 << " (" << std::boolalpha << success << ")\n";
    EXPECT_TRUE(success);

    Variant<int, double, std::string> myVariant3 = fromInt;
    const int& myInt3 = myVariant3.get<int>(success);
    std::cout << "myInt3 = " << myInt3 << " (" << std::boolalpha << success << ")\n";
    EXPECT_TRUE(success);

    myString = myVariants->get<std::string>(success);
    std::cout << "myString = " << myString << " (" << std::boolalpha << success << ")\n";
    EXPECT_TRUE(success);

    Variant<int, double, std::string> myVariantCopy(myVariant);
    const int& myIntCopy = myVariantCopy.get<int>(success);
    std::cout << "myIntCopy = " << myIntCopy << " (" << std::boolalpha << success << ")\n";
    EXPECT_TRUE(success);

    std::cout << "myIntCopy equals myInt= " << "(" << std::boolalpha << (myVariant == myVariantCopy) << ")\n";
    EXPECT_TRUE((myVariant == myVariantCopy));

    const int& myFake = myVariant.get<double>(success);
    std::cout << "myFake = " << myFake << " (" << std::boolalpha << success << ")\n";
    EXPECT_FALSE(success);

    std::cout << "myInt is int = " << " (" << std::boolalpha << myVariant.isType<int>() << ")\n";
    EXPECT_TRUE(myVariant.isType<int>());

    std::cout << "myInt is std::string = " << " (" << std::boolalpha << myVariant.isType<std::string>() << ")\n";
    EXPECT_FALSE(myVariant.isType<std::string>());

    const double& myDouble = myVariantf.get<double>(success);
    std::cout << "myDouble = " << myDouble << " (" << std::boolalpha << success << ")\n";
    EXPECT_TRUE(success);

    Variant<int, double, std::string> myVariantsCopy(*myVariants);
    std::string myStringCopy = myVariantsCopy.get<std::string>(success);
    std::cout << "myStringCopy = " << myStringCopy << " (" << std::boolalpha << success << ")\n";
    EXPECT_TRUE(success);
//    EXPECT_TRUE((myVariants == myVariantsCopy));

    bool s2;
    myVariants->set<std::string>(std::string("Hello World"), s2);
    myString = myVariants->get<std::string>(success);
    std::cout << "myString = " << myString << " (" << std::boolalpha << success << ")\n";
    EXPECT_TRUE(success);

    myStringCopy = myVariantsCopy.get<std::string>(success);
    std::cout << "myStringCopy = " << myStringCopy << " (" << std::boolalpha << success << ")\n";
    EXPECT_TRUE(success);

    delete myVariants;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
