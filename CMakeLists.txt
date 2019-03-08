cmake_minimum_required(VERSION 3.4)
project(Rescue)

set(VERSION "0.1")

# Canonize platform selection
set(Rescue_PLATFORM Undefined)
if(WIN32)
    set(Rescue_PLATFORM PLATFORM_WIN32)
elseif(APPLE)
    set(Rescue_PLATFORM PLATFORM_MACOS)
elseif(UNIX AND NOT APPLE)
    set(Rescue_PLATFORM PLATFORM_LINUX)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# External modules
add_subdirectory(externals/pugixml)

# Own modules
add_subdirectory(modules/Frontend/Frontend)
add_subdirectory(modules/Domain/Domain)

set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR "NSIS")  
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Marius Elvert")
set(CPACK_PACKAGE_VENDOR "")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple Response Curve Editor to be used with a Utility AI structured like Dave Mark's Infinite Axis Utility System")
set(CPACK_PACKAGE_DESCRIPTION "A simple Response Curve Editor to be used with a Utility AI structured like Dave Mark's Infinite Axis Utility System")
include(CPack)
