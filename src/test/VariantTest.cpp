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


struct test1: CommonAPI::SerializableStruct {
    int a;
    std::string b;

    test1() = default;
    test1(const int& a, const std::string& b) :
    a(a), b(b) {
    }

    void readFromInputStream(CommonAPI::InputStream& inputStream) {

    }

    void writeToOutputStream(CommonAPI::OutputStream& outputStream) const {

    }

    static inline void writeToTypeOutputStream(CommonAPI::TypeOutputStream& typeOutputStream) {
    }

};

struct test2: CommonAPI::SerializableStruct {
    int a;
    std::string b;

    test2() = default;
    test2(const int& a, const std::string& b) :
    a(a), b(b) {
    }

    void readFromInputStream(CommonAPI::InputStream& inputStream) {

    }

    void writeToOutputStream(CommonAPI::OutputStream& outputStream) const {

    }

    static inline void writeToTypeOutputStream(CommonAPI::TypeOutputStream& typeOutputStream) {
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

    Variant<int, double, std::string> movedVariant = std::move(myVariant);
    std::cout << "myMovedInt is int = " << " (" << std::boolalpha << movedVariant.isType<int>() << ")\n";
    EXPECT_TRUE(movedVariant.isType<int>());
    EXPECT_EQ(fromInt, movedVariant.get<int>());

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

    test1 sourceStruct = {1, "a"};

    Variant<test1, test2> complexSource = sourceStruct;

    Variant<test1, test2> complexTarget = complexSource;
    EXPECT_EQ(1, complexTarget.get<test1>().a);

}

typedef Variant<test1, test2, std::string, uint8_t> ComplexTestVariant;

struct Container: CommonAPI::SerializableStruct {
    ComplexTestVariant a;
    std::string b;

    Container() = default;
    Container(const ComplexTestVariant& a, const std::string& b) :
    a(a), b(b) {
    }

    void readFromInputStream(CommonAPI::InputStream& inputStream) {

    }

    void writeToOutputStream(CommonAPI::OutputStream& outputStream) const {

    }

    static inline void writeToTypeOutputStream(CommonAPI::TypeOutputStream& typeOutputStream) {
    }

};

TEST_F(VariantTest, VariantMoveTest) {

    ComplexTestVariant emptyTest;
    Container cont(std::move(emptyTest), "Hello");

    test1 assignStruct(1, "Assign");
    ComplexTestVariant assignTest = assignStruct;
    Container contAss(std::move(assignTest), "Hello");
    EXPECT_EQ("Assign", contAss.a.get<test1>().b);

    test1 constructStruct(1, "Construct");
    ComplexTestVariant constructTest(constructStruct);
    Container contCon(std::move(constructTest), "Hello");
    EXPECT_EQ("Construct", contCon.a.get<test1>().b);

    ComplexTestVariant rec;

    rec = cont.a;

    rec = contAss.a;
    EXPECT_EQ("Assign", rec.get<test1>().b);

    rec = contCon.a;
    EXPECT_EQ("Construct", rec.get<test1>().b);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
