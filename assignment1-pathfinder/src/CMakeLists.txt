cmake_minimum_required(VERSION 3.7)

if (DEFINED ENV{VCPKG_ROOT})
  set(VCPKG_CMAKE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
else ()
  set(VCPKG_CMAKE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif ()

set(APP_NAME pathfinder)
project(${APP_NAME})

set(CMAKE_TOOLCHAIN_FILE ${VCPKG_CMAKE}) # n.b.

set(RLCPP "${CMAKE_CURRENT_SOURCE_DIR}/../deps/raylib-cpp") # a symbolic link
set(RLEXTRAS "${CMAKE_CURRENT_SOURCE_DIR}/../deps/raylib-extras")
set(REDBLOBDIR "${CMAKE_CURRENT_SOURCE_DIR}/../deps/redblob-games")
set(PATHFINDER_SOURCE_DIR "${PROJECT_SOURCE_DIR}")

message("PATHFINDER_SOURCE_DIR : " ${PATHFINDER_SOURCE_DIR})

file (GLOB_RECURSE SOURCE_FILES 
	${PATHFINDER_SOURCE_DIR}/*.c
	${PATHFINDER_SOURCE_DIR}/*.cpp
	${PATHFINDER_SOURCE_DIR}/*.h
	${PATHFINDER_SOURCE_DIR}/*.hpp)

#set(n main)
#project(${n})

find_package(raylib CONFIG REQUIRED)

#set(INCLUDE_DIR "./")

#include_directories(INCLUDE_DIR)

message("APP_NAME : " ${APP_NAME})
message("SOURCE_FILES : " ${SOURCE_FILES})

add_executable(${APP_NAME} ${SOURCE_FILES})
target_include_directories(${APP_NAME} PRIVATE ${raylib_INCLUDE_DIRS})
target_include_directories(${APP_NAME} PRIVATE ${RLCPP}/include ${RLEXTRAS})
target_include_directories(${APP_NAME} PRIVATE ${REDBLOBDIR})
target_link_libraries(${APP_NAME} PRIVATE raylib)
target_compile_features(${APP_NAME} PRIVATE cxx_std_17) # C++17 structured bindings

if (UNIX)
  # asan and ubsan now seem required, even with a non-Debug build.
  target_link_libraries(${APP_NAME} PRIVATE asan ubsan m dl pthread)
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${APP_NAME})

