cmake_minimum_required(VERSION 3.2)
if(TARGET vdtreflect)
	return()
endif()
project(vdtreflect)

set(CMAKE_CXX_STANDARD 17)

if(ASAN_ENABLED)
	string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\n")
	add_compile_options(-fsanitize=address -fsanitize=fuzzer)
endif()

add_library(${PROJECT_NAME} INTERFACE)

if(MSVC)
	target_compile_options(${PROJECT_NAME} INTERFACE "/MP")
endif()

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)