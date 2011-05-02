cmake_minimum_required(VERSION 2.8)
project(portscanner)

set(SRC_PATH ${portscanner_SOURCE_DIR})
#set(TOOLS_PATH ${SRC_PATH}/thirdparty)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG -Werror")
set(CMAKE_DEBUG_POSTFIX "_d")

set(SRC main.cpp)
if(UNIX)
    list(APPEND SRC 
        mutex_posix.cpp
        thread_posix.cpp
        socket_posix.cpp
    )
endif(UNIX)

include_directories(
#    ${TOOLS_PATH}
    ${SRC_PATH}
)
add_executable(pscan ${SRC})

target_link_libraries(pscan pthread)

