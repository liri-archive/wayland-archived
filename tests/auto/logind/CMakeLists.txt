include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../../headers"
)

add_executable(tst_logind fakelogind.cpp tst_logind.cpp)
target_link_libraries(tst_logind Qt5::Test Liri::Platform)
add_test(liri-test-logind tst_logind)
ecm_mark_as_test(tst_logind)

add_executable(tst_udev tst_udev.cpp)
target_link_libraries(tst_udev Qt5::Test Liri::Platform)
add_test(liri-test-udev tst_udev)
ecm_mark_as_test(tst_udev)
