

cmake_minimum_required(VERSION 4.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/fetch-content" CACHE PATH "" FORCE)
project(mango LANGUAGES C CXX)
include(FetchContent)

# RAYLIB
add_definitions(-DGRAPHICS_API_OPENGL_33)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(raylib)
set_target_properties(raylib PROPERTIES COMPILE_OPTIONS "")

# Collect all .cpp and .ixx files recursively
file(GLOB_RECURSE SOURCES
        CONFIGURE_DEPENDS
        "src/*.cpp"
)

file(GLOB_RECURSE SOURCES_MODULES
        CONFIGURE_DEPENDS
        "src/*.ixx"
)

add_library(mango STATIC ${SOURCES})
set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)
set_target_properties(mango PROPERTIES CXX_SCAN_FOR_MODULES ON)
target_sources(mango PUBLIC FILE_SET CXX_MODULES FILES ${SOURCES_MODULES})
target_link_libraries(mango PRIVATE raylib)

