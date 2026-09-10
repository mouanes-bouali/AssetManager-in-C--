file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(tests ${TEST_SOURCES})

# ✅ Fix WinMain
set_target_properties(tests PROPERTIES WIN32_EXECUTABLE OFF)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
)

target_link_libraries(tests PRIVATE asset_lib)

include(CTest)
add_test(NAME tests COMMAND tests)

