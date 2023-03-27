cmake_minimum_required(VERSION 3.8)

set(WARNINGS_AS_ERRORS_FOR_WANTS_DARK_THEME OFF CACHE BOOL "ON iff you want to treat warnings as errors")

add_library(wants_dark_theme)
add_library(wants_dark_theme::wants_dark_theme ALIAS wants_dark_theme)
target_compile_features(wants_dark_theme PUBLIC cxx_std_17)

# ---Add source files---
if(WARNINGS_AS_ERRORS_FOR_WANTS_DARK_THEME)
    target_include_directories(wants_dark_theme PUBLIC include)
else()
    target_include_directories(wants_dark_theme SYSTEM PUBLIC include)
endif()
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
target_sources(wants_dark_theme PRIVATE ${SRC_FILES})

# ---Set warning level---
if(MSVC)
    target_compile_options(wants_dark_theme PRIVATE /W4)
else()
    target_compile_options(wants_dark_theme PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion)
endif()

# ---Maybe enable warnings as errors---
if(WARNINGS_AS_ERRORS_FOR_WANTS_DARK_THEME)
    if(MSVC)
        target_compile_options(wants_dark_theme PRIVATE /WX)
    else()
        target_compile_options(wants_dark_theme PRIVATE -Werror)
    endif()
endif()
