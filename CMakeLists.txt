# This file is part of libknotdeviceinfo.
#
# For license and copyright information please follow this link:
# https://github.com/noseam-env/libknotdeviceinfo/blob/master/LEGAL

cmake_minimum_required(VERSION 3.0)

set(project_langs C CXX)
if (APPLE)
    set(project_langs C CXX OBJC OBJCXX)
endif ()

project(knotdeviceinfo LANGUAGES ${project_langs})

set(CMAKE_CXX_STANDARD 17)

if (UNIX AND NOT APPLE)
    set(SOURCES src/linux.cpp)
elseif (APPLE)
    set(SOURCES src/macos.cpp src/macos_obj.mm)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(FOUNDATION_LIBRARY Foundation)
    list(APPEND LINK_LIBS ${IOKIT_LIBRARY} ${FOUNDATION_LIBRARY})
elseif (WIN32)
    set(SOURCES src/windows.cpp)
    list(APPEND LINK_LIBS wbemuuid)
endif ()

add_library(knotdeviceinfo_static STATIC ${SOURCES})

target_include_directories(knotdeviceinfo_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(knotdeviceinfo_static PRIVATE ${LINK_LIBS})
