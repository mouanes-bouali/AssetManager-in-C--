# Test executable using local catch.hpp (Catch2 v2 single-header)
add_executable(tests
    test_mylib.cpp
)

target_link_libraries(tests PRIVATE
    mylib
)
