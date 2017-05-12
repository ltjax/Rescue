set(TARGET_NAME Domain)

add_library(${TARGET_NAME}
  Action.hpp
  Action.cpp
  Group.hpp
  Group.cpp
  Curve.hpp
  Curve.cpp
  Axis.hpp
  Axis.cpp)
  
target_include_directories(${TARGET_NAME}
  INTERFACE ../)