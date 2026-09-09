# ─── Change this ──────────────────────────────────────────────────
add_library(mylib INTERFACE)   # ← Name it mylib

target_include_directories(mylib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)