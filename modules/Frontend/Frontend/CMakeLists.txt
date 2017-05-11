set(TARGET_NAME Frontend)

qt5_wrap_ui(UI_HEADERS
  Rescue.ui
  Action.ui
  Axis.ui)

add_executable(${TARGET_NAME}
  Main.hpp
  Main.cpp
  Rescue.hpp
  Rescue.cpp
  ActionWidget.hpp
  ActionWidget.cpp
  AxisWidget.hpp
  AxisWidget.cpp
  ${UI_HEADERS})
  
target_link_libraries(${TARGET_NAME}
  PUBLIC Domain
  PUBLIC Qt5::Widgets)