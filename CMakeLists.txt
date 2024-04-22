# This file is part of libknotdeviceinfo.
#
# For license and copyright information please follow this link:
# https://github.com/noseam-env/libknotdeviceinfo/blob/master/LEGAL

cmake_minimum_required(VERSION 3.5)

set(project_langs C CXX)
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(project_langs C OBJC OBJCXX)
endif ()

project(knotdeviceinfo LANGUAGES ${project_langs})

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OS_WINDOWS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(OS_MACOS TRUE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux|FreeBSD|OpenBSD")
    string(TOUPPER "${CMAKE_SYSTEM_NAME}" _SYSTEM_NAME_U)
    set(OS_${_SYSTEM_NAME_U} TRUE)
endif()

if (OS_LINUX AND NOT ANDROID) # GNU/Linux
    set(KNOTDEVICEINFO_SOURCES src/linux.c)
elseif (OS_MACOS)
    set(KNOTDEVICEINFO_SOURCES src/macos.c src/macos_obj.mm)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(FOUNDATION_LIBRARY Foundation)
    list(APPEND KNOTDEVICEINFO_LIBS ${IOKIT_LIBRARY} ${FOUNDATION_LIBRARY})
elseif (OS_WINDOWS)
    set(KNOTDEVICEINFO_SOURCES src/windows.cpp)
    list(APPEND KNOTDEVICEINFO_LIBS wbemuuid)
else ()
    message(FATAL_ERROR "Unknown or unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif ()

add_library(knotdeviceinfo_static STATIC src/free.c src/os.h ${KNOTDEVICEINFO_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/include/knot/deviceinfo.h")

target_include_directories(knotdeviceinfo_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(knotdeviceinfo_static PRIVATE ${KNOTDEVICEINFO_LIBS})
