cmake_minimum_required(VERSION 3.2)
if(TARGET vdtreflect-compiler)
	return()
endif()
project(vdtreflect-compiler)

set(CMAKE_CXX_STANDARD 17)

if(ASAN_ENABLED)
	string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\n")
	add_compile_options(-fsanitize=address -fsanitize=fuzzer)
endif()

file(GLOB PROJECT_HEADERS "source/*.h") 
file(GLOB PROJECT_SOURCES "source/*.cpp")

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})

add_executable(
    ${PROJECT_NAME} 
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES} 
)

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif()