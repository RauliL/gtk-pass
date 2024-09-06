FIND_PACKAGE(PkgConfig)

INCLUDE(CheckIncludeFile)

PKG_CHECK_MODULES(GTKMM gtkmm-3.0 REQUIRED)

ADD_EXECUTABLE(
  gtk-pass
  ./entry-completion.cpp
  ./entry-list.cpp
  ./main.cpp
  ./password-store.cpp
  ./window.cpp
)

TARGET_COMPILE_FEATURES(
  gtk-pass
  PRIVATE
    cxx_std_17
)

IF(MSVC)
  TARGET_COMPILE_OPTIONS(
    gtk-pass
    PRIVATE
      /W4 /WX
  )
ELSE()
  TARGET_COMPILE_OPTIONS(
    gtk-pass
    PRIVATE
      -Wall -Werror
  )
ENDIF()

TARGET_INCLUDE_DIRECTORIES(
  gtk-pass
  PRIVATE
    ${GTKMM_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES(
  gtk-pass
  ${GTKMM_LIBRARIES}
)

INSTALL(
  TARGETS
    gtk-pass
  RUNTIME DESTINATION
    bin
)

INSTALL(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../gtk-pass.desktop
  DESTINATION
    share/applications
)
