/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include <src/CommonAPI/SerializableVariant.h>

using namespace CommonAPI;

int main(int argc, char** argv) {
    int fromInt = 5;
    double fromDouble = 12.344d;
    std::string fromString = "123abc!";
    Variant<int, double, std::string> myVariant(fromInt);

    Variant<int, double, std::string> myVariantf(fromDouble);

    Variant<int, double, std::string>* myVariants = new Variant<int, double, std::string>(fromString);
    bool success;

    const int& myInt = myVariant.get<int>(success);
    std::cout << "myInt = " << myInt << " (" << std::boolalpha << success << ")\n";

    Variant<int, double, std::string> myVariant2 = myVariant;
    const int& myInt2 = myVariant2.get<int>(success);
    std::cout << "myInt2 = " << myInt2 << " (" << std::boolalpha << success << ")\n";

    Variant<int, double, std::string> myVariant3 = fromInt;
    const int& myInt3 = myVariant3.get<int>(success);
    std::cout << "myInt3 = " << myInt3 << " (" << std::boolalpha << success << ")\n";

    Variant<int, double, std::string> myVariantCopy(myVariant);
    const int& myIntCopy = myVariantCopy.get<int>(success);
    std::cout << "myIntCopy = " << myIntCopy << " (" << std::boolalpha << success << ")\n";

    std::cout << "myIntCopy equals myInt= " << "(" << std::boolalpha << (myVariant == myVariantCopy) << ")\n";

    const int& myFake = myVariant.get<double>(success);
    std::cout << "myFake = " << myFake << " (" << std::boolalpha << success << ")\n";

    std::cout << "myInt is int = " << " (" << std::boolalpha << myVariant.isType<int>() << ")\n";
    std::cout << "myInt is std::string = " << " (" << std::boolalpha << myVariant.isType<std::string>() << ")\n";

    const int& myDouble = myVariantf.get<double>(success);
    std::cout << "myDouble = " << myDouble << " (" << std::boolalpha << success << ")\n";

    const std::string& myString = myVariants->get<std::string>(success);
    std::cout << "myString = " << myString << " (" << std::boolalpha << success << ")\n";

    delete myVariants;

    return 0;
}


