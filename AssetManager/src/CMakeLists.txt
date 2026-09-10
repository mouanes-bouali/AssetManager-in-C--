
# ✅ Third-party implementations
add_library(asset_lib STATIC
    core/impl.cpp
)

target_include_directories(asset_lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core
)

target_include_directories(asset_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core
)