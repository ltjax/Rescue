set(TARGET_NAME Frontend)

qt5_wrap_ui(UI_HEADERS
  Rescue.ui)

add_executable(${TARGET_NAME}
  Main.hpp
  Main.cpp
  Rescue.hpp
  Rescue.cpp
  ${UI_HEADERS})
  
target_link_libraries(${TARGET_NAME}
  PUBLIC Domain
  PUBLIC Qt5::Widgets)