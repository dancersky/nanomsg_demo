cmake_minimum_required(VERSION 2.8)
project(DoorSys)

#set(CMAKE_BUILD_TYPE "Debug")
aux_source_directory(. DIR_SRCS)

add_definitions(-DGOGENIUS_DEBUG)

add_subdirectory(nanomsg)

add_executable(DoorSys ${DIR_SRCS})

target_link_libraries(DoorSys nanomsg)
target_link_libraries(DoorSys pthread)


INSTALL(TARGETS DoorSys DESTINATION bin)
