FIND_PACKAGE(PkgConfig)

INCLUDE(CheckIncludeFile)

PKG_CHECK_MODULES(GTKMM gtkmm-3.0 REQUIRED)

CHECK_INCLUDE_FILE(glob.h HAVE_GLOB_H)
CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)
CHECK_INCLUDE_FILE(wordexp.h HAVE_WORDEXP_H)

ADD_EXECUTABLE(
  gtk-pass
  ./entry-completion.cpp
  ./entry-list.cpp
  ./main.cpp
  ./password-store.cpp
  ./utils.cpp
  ./window.cpp
)

TARGET_COMPILE_FEATURES(
  gtk-pass
  PRIVATE
    cxx_std_11
)

TARGET_COMPILE_OPTIONS(
  gtk-pass
  PRIVATE
    -Wall -Werror
)

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
