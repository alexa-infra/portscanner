cmake_minimum_required(VERSION 2.8)
project(portscanner)

include(CheckIncludeFiles)

set(SRC_PATH ${portscanner_SOURCE_DIR})
set(JSON_PATH ${SRC_PATH}/jsoncpp-src-0.5.0)
set(JSON_SRC_PATH ${JSON_PATH}/src/lib_json)

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror")
endif()
if(WIN32 AND MSVC)
    set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /WX /EHsc")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
endif() 

set(SRC ${SRC_PATH}/main.cpp
        ${SRC_PATH}/data.h
        ${SRC_PATH}/data.cpp
        ${SRC_PATH}/app.cpp
        ${SRC_PATH}/app.h
        ${SRC_PATH}/socket.h
        ${SRC_PATH}/thread.h
        ${SRC_PATH}/mutex.h
        ${SRC_PATH}/types.h
        ${SRC_PATH}/platform.h)

set(JSON_SRC ${JSON_SRC_PATH}/json_reader.cpp
             ${JSON_SRC_PATH}/json_writer.cpp
             ${JSON_SRC_PATH}/json_internalarray.inl
             ${JSON_SRC_PATH}/json_value.cpp
             ${JSON_SRC_PATH}/json_internalmap.inl
             ${JSON_SRC_PATH}/json_valueiterator.inl)

check_include_files("pthread.h" HAS_PTHREAD_H)
if(HAS_PTHREAD_H)
    list(APPEND SRC 
        ${SRC_PATH}/mutex_posix.cpp
        ${SRC_PATH}/thread_posix.cpp
    )
elseif(WIN32)
    list(APPEND SRC 
        ${SRC_PATH}/mutex_win32.cpp
        ${SRC_PATH}/thread_win32.cpp
    )
else()
    message(FATAL_ERROR "Threading is not implemented for this system")
endif()

CHECK_INCLUDE_FILES("stdint.h" HAS_STDINT_H)
if(NOT HAS_STDINT_H)
    if(WIN32)
        list(APPEND SRC
            ${SRC_PATH}/win32/stdint.h
        )
        include_directories(${SRC_PATH}/win32)
    else()
        message(FATAL_ERROR "There is not stdint.h for this system")
    endif()
endif()

if(WIN32)
    list(APPEND SRC
        ${SRC_PATH}/socket_win32.cpp
    )
elseif(UNIX)
    list(APPEND SRC
        ${SRC_PATH}/socket_posix.cpp
    )
else()
    message(FATAL_ERROR "There is not socket support for this system")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pscan_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/pscan_config.h)

include_directories(
    ${SRC_PATH}
    ${JSON_PATH}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)
add_executable(pscan ${SRC} ${JSON_SRC})

if(HAS_PTHREAD_H)
	target_link_libraries(pscan pthread)
endif()

if(WIN32)
	target_link_libraries(pscan ws2_32)
endif(WIN32)
