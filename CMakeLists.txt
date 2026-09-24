cmake_minimum_required(VERSION 3.14)

project(knotdeviceinfo VERSION 0.1.0 LANGUAGES C)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    enable_language(OBJC)
endif ()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OS_WINDOWS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(OS_DARWIN TRUE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux|FreeBSD|OpenBSD")
    string(TOUPPER "${CMAKE_SYSTEM_NAME}" _SYSTEM_NAME_U)
    set(OS_${_SYSTEM_NAME_U} TRUE)
endif()

set(KNOTDEVICEINFO_SOURCES src/base.c src/target.h include/knot/deviceinfo.h)

if (OS_LINUX AND NOT ANDROID)
    list(APPEND KNOTDEVICEINFO_SOURCES src/linux.c)
elseif (OS_DARWIN)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(FOUNDATION_LIBRARY Foundation)
    list(APPEND KNOTDEVICEINFO_SOURCES src/macos.c src/macos.m)
    list(APPEND KNOTDEVICEINFO_LIBS ${IOKIT_LIBRARY} ${FOUNDATION_LIBRARY})
elseif (OS_WINDOWS)
    list(APPEND KNOTDEVICEINFO_SOURCES src/windows.c)
    list(APPEND KNOTDEVICEINFO_LIBS wbemuuid)
elseif (ANDROID)
    list(APPEND KNOTDEVICEINFO_SOURCES src/android.c)
    list(APPEND KNOTDEVICEINFO_LIBS android)
else ()
    list(APPEND KNOTDEVICEINFO_SOURCES src/noop.c)
    message(WARNING "Unknown or unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif ()

add_library(knotdeviceinfo ${KNOTDEVICEINFO_SOURCES})
add_library(knotdeviceinfo::knotdeviceinfo ALIAS knotdeviceinfo)

get_target_property(KNOTDEVICEINFO_LIB_TYPE knotdeviceinfo TYPE)
if (KNOTDEVICEINFO_LIB_TYPE STREQUAL "SHARED_LIBRARY")
    target_compile_definitions(knotdeviceinfo PRIVATE KNOTDEVICEINFO_DYNAMIC)
    target_compile_definitions(knotdeviceinfo PUBLIC KNOTDEVICEINFO_DYNAMIC_IMPORT)
endif ()

target_include_directories(knotdeviceinfo PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(knotdeviceinfo PRIVATE ${KNOTDEVICEINFO_LIBS})

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS knotdeviceinfo
    EXPORT knotdeviceinfoTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT knotdeviceinfoTargets
    FILE knotdeviceinfoTargets.cmake
    NAMESPACE knotdeviceinfo::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/knotdeviceinfo
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/knotdeviceinfoConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/knotdeviceinfoConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/knotdeviceinfo"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/knotdeviceinfoConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/knotdeviceinfoConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/knotdeviceinfoConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/knotdeviceinfo"
)

if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory(example)
endif ()

