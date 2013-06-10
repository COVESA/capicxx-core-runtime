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
    EXPECT_EQ(fromString, myString);


    const int& myInt = myVariant.get<int>();
    EXPECT_EQ(myInt, fromInt);

    int vType = myVariant.getValueType();
    EXPECT_EQ(3, vType);

    Variant<int, double, std::string> myVariant2 = myVariant;
    const int& myInt2 = myVariant2.get<int>();
    EXPECT_EQ(myInt2, fromInt);

    Variant<int, double, std::string> myVariant3 = fromInt;
    const int& myInt3 = myVariant3.get<int>();
    EXPECT_EQ(myInt3, fromInt);

    myString = myVariants->get<std::string>();
    EXPECT_EQ(myString, fromString);

    Variant<int, double, std::string> myVariantCopy(myVariant);
    const int& myIntCopy = myVariantCopy.get<int>();
    EXPECT_EQ(myIntCopy, fromInt);

    Variant<int, double, std::string> myVariantCopy2;
    myVariantCopy2 = myVariant;

    const int& myIntCopy2 = myVariantCopy2.get<int>();
    EXPECT_EQ(myIntCopy2, fromInt);


    EXPECT_ANY_THROW(const int& myFake = myVariant.get<double>());

    EXPECT_TRUE(myVariant.isType<int>());

    EXPECT_FALSE(myVariant.isType<std::string>());

    Variant<int, double, std::string> movedVariant = std::move(myVariant);
    EXPECT_TRUE(movedVariant.isType<int>());
    EXPECT_EQ(fromInt, movedVariant.get<int>());

    const double& myDouble = myVariantf.get<double>();
    EXPECT_EQ(myDouble, fromDouble);

    Variant<int, double, std::string> myVariantsCopy(*myVariants);
    std::string myStringCopy = myVariantsCopy.get<std::string>();
    EXPECT_EQ(myStringCopy, fromString);

    *myVariants = std::string("Hello World");
    myString = myVariants->get<std::string>();
    EXPECT_EQ(myString, "Hello World");

    myStringCopy = myVariantsCopy.get<std::string>();
    EXPECT_EQ(myStringCopy, fromString);

    delete myVariants;

    test1 sourceStruct = {1, "a"};

    Variant<test1, test2> complexSource = sourceStruct;

    Variant<test1, test2> complexTarget = complexSource;
    EXPECT_EQ(1, complexTarget.get<test1>().a);
}

typedef Variant<test1, test2, std::string, uint8_t> ComplexTestVariant;
typedef Variant<ComplexTestVariant, test2, std::string, uint8_t> OuterTestVariant;

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

struct ContainerOuter: CommonAPI::SerializableStruct {
    OuterTestVariant a;
    std::string b;

    ContainerOuter() = default;
    ContainerOuter(const OuterTestVariant& a, const std::string& b) :
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

TEST_F(VariantTest, VariantInVariantMoveTest) {

    OuterTestVariant emptyTest;
    ContainerOuter cont(std::move(emptyTest), "Hello");

    test1 assignStruct(1, "Assign");
    ComplexTestVariant assignTest = assignStruct;
    OuterTestVariant assignOuter = assignTest;
    ContainerOuter contAss(std::move(assignOuter), "Hello");
    EXPECT_EQ("Assign", contAss.a.get<ComplexTestVariant>().get<test1>().b);

    test1 constructStruct(1, "Construct");
    ComplexTestVariant constructTest(constructStruct);
    OuterTestVariant constructOuter(constructTest);
    ContainerOuter contCon(std::move(constructOuter), "Hello");
    EXPECT_EQ("Construct", contCon.a.get<ComplexTestVariant>().get<test1>().b);

    ComplexTestVariant rec;

    rec = contAss.a.get<ComplexTestVariant>();
    EXPECT_EQ("Assign", rec.get<test1>().b);


    rec = contCon.a.get<ComplexTestVariant>();
    EXPECT_EQ("Construct", rec.get<test1>().b);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
