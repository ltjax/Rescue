set(TARGET_NAME Rescue)

set(CMAKE_AUTORCC ON)

qt5_wrap_ui(UI_HEADERS
  Rescue.ui
  Action.ui
  Axis.ui)

add_executable(${TARGET_NAME}
  Action.hpp
  Action.cpp
  Curve.hpp
  Curve.cpp
  LoadSave.hpp
  LoadSave.cpp
  LoadSaveService.hpp
  LoadSaveService.cpp
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
  State.hpp
  State.cpp
  Vocabulary.cpp
  Vocabulary.hpp
  SignalBlocker.cpp
  SignalBlocker.hpp
  Events.hpp
  ../../icons.qrc
  ${UI_HEADERS})
  
target_include_directories(${TARGET_NAME}
  PUBLIC ../../externals/optional)
  
target_link_libraries(${TARGET_NAME}
  PUBLIC pugixml
  PUBLIC Qt5::Widgets
  PUBLIC ushiro
  PUBLIC Boost::boost)

install(TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION Frontend)
