cmake_minimum_required(VERSION 3.2)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

set(REFTECT_COMPILER_EXE "vdtreflect-compiler.exe")

function(generate_file SRC_FILE)
	get_filename_component(_file_path "${SRC_FILE}" PATH)
	message(STATUS "`${SRC_FILE}`: add generation of C++ code in ${_file_path}")
	execute_process(
		COMMAND ${REFTECT_COMPILER_EXE} ${SRC_FILE}
		RESULT_VARIABLE CMD_ERROR
		OUTPUT_FILE CMD_OUTPUT
	)
	message(STATUS "vdtreflect_compiler_error:" ${CMD_ERROR})
	message(STATUS "vdtreflect_compiler_output:" ${CMD_OUTPUT})
endfunction()

generate_file("test.h")

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
