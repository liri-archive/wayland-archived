include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../../headers"
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
)

add_executable(tst_libinput tst_libinput.cpp)
target_link_libraries(tst_libinput Liri::Platform)
