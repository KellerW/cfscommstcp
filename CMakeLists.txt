cmake_minimum_required(VERSION 3.22)
project(CFSCOMMSTCP VERSION 0.1.0 LANGUAGES C CXX)
set(EXECUTABLE_NAME cfscommstcp)
#set(CMAKE_BUILD_TYPE Debug)

set(LIBRARY_TCPEPOLL tcpepoll)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARY_TCPEPOLL})
#target_link_libraries(${PROJECT_NAME} PUBLIC Crow::Crow ${LIBRARY_MODELS} ${LIBRARY_CONTROLLER})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

add_subdirectory(src)
