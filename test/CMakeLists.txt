cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(Simple_Vector_Test)

include(FetchContent)

CPMAddPackage("gh:catchorg/Catch2@2.13.8")

add_executable(
	${PROJECT_NAME}
	"main.cpp"
	"VectorTests.cpp"
	"GeneratorTests.cpp"
	"AlgorithmTests.cpp"
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	Simple_Vector
	Catch2::Catch2
)

add_test(SimpleVectorTestSuite ${PROJECT_NAME})
