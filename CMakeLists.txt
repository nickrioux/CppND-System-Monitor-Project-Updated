cmake_minimum_required(VERSION 2.6)

# Check platforms
if(CMAKE_HOST_APPLE)
    message("APPLE PLATFORM")
    set(MACOS 1)
elseif(CMAKE_HOST_UNIX)
    message("LINUX PLATFORM")
    set(LINUX 1)
endif()

set(CMAKE_PREFIX_PATH /opt/local ${CMAKE_PREFIX_PATH})

project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

if(LINUX)
    message("LINUX SOURCES")
    file(GLOB PLATFORMSOURCES "src/linux/*.cpp")
    include_directories(include/linux)
elseif(MACOS)
    message("MACOS SOURCES")
    file(GLOB PLATFORMSOURCES "src/macosx/*.cpp")
    include_directories(include/macosx)
endif()

add_executable(monitor ${SOURCES} ${PLATFORMSOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
