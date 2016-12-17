include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../../headers"
    ${Qt5Core_PRIVATE_INCLUDE_DIRS}
    ${Qt5WaylandCompositor_PRIVATE_INCLUDE_DIRS}
)

add_executable(tst_client_display tst_display.cpp)
target_link_libraries(tst_client_display
                      Qt5::Test
                      Qt5::WaylandCompositor
                      Liri::WaylandClient)
add_test(liri-test-client-display tst_client_display)
ecm_mark_as_test(tst_client_display)

add_executable(tst_client_output tst_output.cpp)
target_link_libraries(tst_client_output
                      Qt5::Test
                      Qt5::WaylandCompositor
                      Liri::WaylandClient)
add_test(liri-test-client-output tst_client_output)
ecm_mark_as_test(tst_client_output)

add_executable(tst_client_registry tst_registry.cpp)
target_link_libraries(tst_client_registry
                      Qt5::Test
                      Qt5::WaylandCompositor
                      Liri::WaylandClient
                      Liri::WaylandServer)
add_test(liri-test-client-registry tst_client_registry)
ecm_mark_as_test(tst_client_registry)

add_executable(tst_client_shmpool tst_shmpool.cpp)
target_link_libraries(tst_client_shmpool
                      Qt5::Test
                      Qt5::WaylandCompositor
                      Liri::WaylandClient)
add_test(liri-test-client-shmpool tst_client_shmpool)
ecm_mark_as_test(tst_client_shmpool)
