# Library target
add_library(mylib
    mylib/mylib.cpp
    mylib/mylib.h
)

target_include_directories(mylib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Application target
add_executable(myapp main.cpp)

target_link_libraries(myapp PRIVATE
    mylib
)
