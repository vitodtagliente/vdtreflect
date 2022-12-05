cmake_minimum_required(VERSION 3.2)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

set(GENERATOR_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/../tools/vdtreflect-compiler.exe")

function(generate_reflection EXECUTABLE SRC_DIR)
	execute_process(
		COMMAND ${EXECUTABLE} ${SRC_DIR}
		RESULT_VARIABLE CMD_ERROR
		OUTPUT_FILE CMD_OUTPUT
	)
	if(${CMD_ERROR} EQUAL 0)
		message(STATUS "Generated C++ code for ${SRC_DIR}")
	else()
		message(STATUS "`${SRC_DIR}` failed to generate C++ code with error: ${CMD_ERROR}")
	endif()
endfunction()

# generate reflection data
generate_reflection(${GENERATOR_EXECUTABLE} ${PROJECT_SOURCE_DIR})

file(GLOB PROJECT_HEADERS "*.h") 
file(GLOB PROJECT_SOURCES "*.cpp")

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})

add_executable(
    ${PROJECT_NAME} 
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES} 
)

target_link_libraries(
    ${PROJECT_NAME} 
    vdtreflect
)

add_subdirectory(../ compiler)
add_subdirectory(../ vdtreflect)
