cmake_minimum_required(VERSION 2.8)
project(portscanner)

set(SRC_PATH ${portscanner_SOURCE_DIR})
set(JSON_PATH ${SRC_PATH}/jsoncpp-src-0.5.0)
set(JSON_SRC_PATH ${JSON_PATH}/src/lib_json)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG -Werror")
set(CMAKE_DEBUG_POSTFIX "_d")

set(SRC ${SRC_PATH}/main.cpp)
set(JSON_SRC ${JSON_SRC_PATH}/json_reader.cpp
             ${JSON_SRC_PATH}/json_writer.cpp
             ${JSON_SRC_PATH}/json_internalarray.inl
             ${JSON_SRC_PATH}/json_value.cpp
             ${JSON_SRC_PATH}/json_internalmap.inl
             ${JSON_SRC_PATH}/json_valueiterator.inl)

if(UNIX)
    list(APPEND SRC 
        ${SRC_PATH}/mutex_posix.cpp
        ${SRC_PATH}/thread_posix.cpp
        ${SRC_PATH}/socket_posix.cpp
    )
endif(UNIX)

include_directories(
#    ${TOOLS_PATH}
    ${SRC_PATH}
    ${JSON_PATH}/include
)
add_executable(pscan ${SRC} ${JSON_SRC})

target_link_libraries(pscan pthread)

