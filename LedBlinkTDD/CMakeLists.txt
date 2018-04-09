project("LedBlinkTestHost" C) 
cmake_minimum_required(VERSION 3.0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
message("Binary will be created in " ${EXECUTABLE_OUTPUT_PATH})

include(CTest)

file(GLOB SOURCES
	"Test/*.c" "Test/Host/*.c" "Test/Common/*.c"
	"Unity/*.c"
	"Src/SampleLogic/*.c"
)

set(MAIN_BIN_NAME LedBlinkTestHost)
add_executable(${MAIN_BIN_NAME} ${SOURCES})

target_include_directories(${MAIN_BIN_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${PROJECT_SOURCE_DIR}/Include)

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -pg")
set(CMAKE_C_FLAGS_RELEASE "-O2 -s")
