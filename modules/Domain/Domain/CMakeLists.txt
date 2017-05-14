set(TARGET_NAME Domain)

add_library(${TARGET_NAME}
  Action.hpp
  Action.cpp
  Group.hpp
  Group.cpp
  Curve.hpp
  Curve.cpp
  LoadSave.hpp
  LoadSave.cpp
  Axis.hpp
  Axis.cpp)
  
target_link_libraries(${TARGET_NAME}
  PUBLIC pugixml)

target_include_directories(${TARGET_NAME}
  INTERFACE ../)