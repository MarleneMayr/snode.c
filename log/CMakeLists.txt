cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

find_package(EASYLOGGINGPP REQUIRED)

add_library(logger STATIC
    Logger.cpp
    Logger.h
)

set_target_properties(logger PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            SOVERSION 1
            POSITION_INDEPENDENT_CODE ON
)

target_compile_options(logger PRIVATE --warn-all)

FILE(GLOB includes "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

set_target_properties(logger PROPERTIES PUBLIC_HEADER "${includes}")

target_include_directories (logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${EASYLOGGINGPP_INCLUDE_DIR})

if(CHECK_INCLUDES)
    set_property(TARGET logger PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif(CHECK_INCLUDES)
