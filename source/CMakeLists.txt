set(TARGET_NAME Rescue)

set(CMAKE_AUTORCC ON)

qt5_wrap_ui(UI_HEADERS
  Rescue.ui
  Action.ui
  Axis.ui
  InputPanel.ui)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set(RESOURCE_FILE
    Rescue.rc
  )
endif()

add_executable(${TARGET_NAME} WIN32
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
  MainWindow.hpp
  MainWindow.cpp
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
  InputPanel.cpp
  InputPanel.hpp
  InputPanelListWidget.cpp
  InputPanelListWidget.hpp
  Events.hpp
  CuteDiffableList.hpp
  OutputListWidget.cpp
  OutputListWidget.hpp
  ../icons.qrc
  ${RESOURCE_FILE}
  ${UI_HEADERS})
  
target_include_directories(${TARGET_NAME}
  PUBLIC ../externals/optional)
  
target_link_libraries(${TARGET_NAME}
  PUBLIC pugixml
  PUBLIC Qt5::Widgets
  PUBLIC CONAN_PKG::ushiro
  PUBLIC CONAN_PKG::boost)

install(TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION .)
