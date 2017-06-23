set(TARGET_NAME Frontend)

qt5_wrap_ui(UI_HEADERS
  Rescue.ui
  Action.ui
  Axis.ui)

add_executable(${TARGET_NAME}
  Main.hpp
  Main.cpp
  RescueMainWindow.hpp
  RescueMainWindow.cpp
  ActionWidget.hpp
  ActionWidget.cpp
  AxisWidget.hpp
  AxisWidget.cpp
  GraphWidget.hpp
  GraphWidget.cpp
  FlowLayout.hpp
  FlowLayout.cpp
  ${UI_HEADERS})
  
target_include_directories(${TARGET_NAME}
  PUBLIC ../../../externals/optional)
  
target_link_libraries(${TARGET_NAME}
  PUBLIC Domain
  PUBLIC Qt5::Widgets)
