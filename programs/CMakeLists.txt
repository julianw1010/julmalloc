add_executable(server server.c)
target_link_libraries(server socketutils)

add_executable(testclient testclient.c)
target_link_libraries(testclient socketutils)

add_executable(editorclient editorclient.c)
target_link_libraries(editorclient socketutils editor ${CURSES_LIBRARIES})
