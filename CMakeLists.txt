cmake_minimum_required(VERSION 3.18.0)
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")
    message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(playground)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(unofficial-mathc CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

include_directories(include)
file(GLOB SOURCES "src/*.c")

add_executable(main ${SOURCES})
set_property(TARGET main PROPERTY C_STANDARD 99)
target_include_directories(main PRIVATE ${STB_INCLUDE_DIRS})
target_link_libraries(main PRIVATE glfw)
target_link_libraries(main PRIVATE glad::glad)
target_link_libraries(main PRIVATE unofficial::mathc::mathc)
