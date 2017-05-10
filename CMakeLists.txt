cmake_minimum_required(VERSION 3.4)
project(Rescue)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

add_subdirectory(modules/Frontend/Frontend)
add_subdirectory(modules/Domain/Domain)
