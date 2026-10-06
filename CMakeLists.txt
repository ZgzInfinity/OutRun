cmake_minimum_required(VERSION 3.16)
project(OutRun LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch and build SFML 2.6 with the same compiler, linked statically
include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE OUTRUN_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(OutRun ${OUTRUN_SOURCES})
target_link_libraries(OutRun PRIVATE sfml-graphics sfml-audio sfml-window sfml-system)

if(WIN32)
    target_link_options(OutRun PRIVATE -static)
    # OpenAL ships as a DLL with SFML; copy it next to the executable
    add_custom_command(TARGET OutRun POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${sfml_SOURCE_DIR}/extlibs/bin/x64/openal32.dll $<TARGET_FILE_DIR:OutRun>)
endif()

# The game loads assets from ./Resources, so run it from the project root
