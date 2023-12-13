#include <gtest/gtest.h>
#include <iostream>


TEST(HelloConan, Hello)
{
    std::cout << "Hello Conan!" << std::endl;

    #ifdef NDEBUG
    std::cout << "Release configuration" << std::endl;
    #else
    std::cout << "Debug configuration" << std::endl;
    #endif
}

int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
