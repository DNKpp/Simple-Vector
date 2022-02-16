cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(Simple_Vector
	LANGUAGES CXX
	VERSION 1.1.0
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include("get_cpm")

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
	# latest msvc disabled ranges for c++20; force /std:c++latest instead
	$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<VERSION_GREATER_EQUAL:${MSVC_VERSION},1930>>:cxx_std_23>
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
