cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(Simple_Vector
	LANGUAGES CXX
	VERSION 1.0.1
)

option(SIMPLE_VECTOR_BUILD_EXAMPLES "Enables example building" OFF)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(
	${PROJECT_NAME}
	INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(
	${PROJECT_NAME}
	INTERFACE
	cxx_std_20
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

	include(CTest)

	set(TestsDir "${CMAKE_CURRENT_SOURCE_DIR}/test/")

	if (BUILD_TESTING)
		add_subdirectory(${TestsDir})
	endif()

endif()

if (SIMPLE_VECTOR_BUILD_EXAMPLES)

	set(ExamplesDir "${CMAKE_CURRENT_SOURCE_DIR}/examples/")

	add_subdirectory(${ExamplesDir})

endif()
