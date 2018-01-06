cmake_minimum_required(VERSION 3.4)
project(Rescue)

set(VERSION "0.1")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

# External modules
add_subdirectory(externals/pugixml)
set(lager_BUILD_DEBUGGER OFF CACHE BOOL "No lager debugger" FORCE)
set(lager_BUILD_EXAMPLES OFF CACHE BOOL "No lager examples" FORCE)
set(lager_BUILD_TESTS OFF CACHE BOOL "No lager tests" FORCE)
add_subdirectory(externals/lager)

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
