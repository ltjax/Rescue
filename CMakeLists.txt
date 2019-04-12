cmake_minimum_required(VERSION 3.4)
project(Rescue)

set(VERSION "0.1")

set(CMAKE_CXX_STANDARD 17)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC OFF)

# Find the QtWidgets library (we're not using conan here)
find_package(Qt5Widgets)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# External modules
add_subdirectory(externals/pugixml)

# Own modules
add_subdirectory(modules/Rescue)
add_subdirectory(modules/ushiro)

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
