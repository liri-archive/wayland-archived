include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../headers"
    ${Qt5Core_PRIVATE_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
)

set(SOURCES
    main.cpp
)

add_library(lirieglfs MODULE ${SOURCES})
target_link_libraries(lirieglfs
                      Qt5::Core
                      Qt5::Gui
                      Liri::Platform
)

install(TARGETS lirieglfs DESTINATION ${PLUGIN_INSTALL_DIR}/platforms)
