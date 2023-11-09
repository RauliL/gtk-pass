INSTALL(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/gtk-pass.svg
  DESTINATION
    share/icons/hicolor/scalable/apps
)

FOREACH(ICON_SIZE 16 32 48 64 128 256)
  INSTALL(
    FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/gtk-pass${ICON_SIZE}.png
    DESTINATION
      share/icons/hicolor/${ICON_SIZE}x${ICON_SIZE}/apps
    RENAME
      gtk-pass
  )
ENDFOREACH()
