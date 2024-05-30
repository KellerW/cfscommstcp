set(LIBRARY_SOURCES "tcp_epoll.c" "tcp_network.c" "utils.c" "event_mgnt.c" "state_mgnt.c")
set(LIBRARY_HEADERS "tcp_types.h" "tcp_epoll.h" "tcp_network.h" "utils.h" "tcp_constants.h" 
"event_mgnt.h" "state_mgnt.h")

add_library(${LIBRARY_TCPEPOLL} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS})
    #target_include_directories(${LIBRARY_TCPEPOLL} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_include_directories(${LIBRARY_TCPEPOLL} PUBLIC "./")