cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(Simple_Vector_Example_Basic_Usage)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	Simple_Vector
)
