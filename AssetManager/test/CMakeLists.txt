

# ─── Find all test source files ──────────────────────────────────
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# ─── Create test executable ─────────────────────────────────────
add_executable(tests ${TEST_SOURCES})

# ─── Include directories ────────────────────────────────────────
target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}        # for catch.hpp
    ${CMAKE_SOURCE_DIR}/src            # for library headers
)

# ─── Link the library ──────────────────────────────────────────
target_link_libraries(tests PRIVATE
    mylib        # Your library target name
)

# ─── Define CATCH_CONFIG_MAIN ──────────────────────────────────
# If you have a main defined in one of your test files, skip this.
# Otherwise, add a file called test_main.cpp with:
#   #define CATCH_CONFIG_MAIN
#   #include "catch.hpp"