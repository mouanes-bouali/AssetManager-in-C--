#include "catch.hpp"
#include "../src/core/AssetManager.hpp"

TEST_CASE("Test it reads files in a folder an Image")
{
    auto& manager= AssetManager::getInstance();
    manager.ReadAssetFile("C:/Users/ACER/Desktop/C++ Files/AssetManager in c++/AssetManager/test/Assets");
    auto registry=manager.GetRegistry();
    auto test =registry.back();
    REQUIRE(test.id==0);

}