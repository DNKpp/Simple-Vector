# Simple-Vector C++20 library

|windows msvc | windows clang-cl | ubuntu clang | ubuntu gcc|
-------------|------------------|--------------|--------------
| ![MSVC](https://github.com/DNKpp/Simple-Vector/workflows/Build%20&%20Test%20-%20MSVC/badge.svg) | ![Clang-Cl](https://github.com/DNKpp/Simple-Vector/workflows/Build%20&%20Test%20-%20Clang-Cl/badge.svg) | ![Clang-10](https://github.com/DNKpp/Simple-Vector/workflows/Build%20&%20Test%20-%20Clang-10/badge.svg) | ![GCC-10](https://github.com/DNKpp/Simple-Vector/workflows/Build%20&%20Test%20-%20GCC-10/badge.svg) |
|			|					| ![Clang-11](https://github.com/DNKpp/Simple-Vector/actions/workflows/build_and_test-Clang-11.yml/badge.svg) | ![GCC-11](https://github.com/DNKpp/Simple-Vector/actions/workflows/build_and_test-GCC-11.yml/badge.svg) |
|			|					| ![Clang-12](https://github.com/DNKpp/Simple-Vector/actions/workflows/build_and_test-Clang-12.yml/badge.svg) |	|

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/ba448bbe4bc04b6289e24d302b68ef44)](https://www.codacy.com/gh/DNKpp/Simple-Vector/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=DNKpp/Simple-Vector&amp;utm_campaign=Badge_Grade)

## Author
Dominic Koepke  
Mail: [DNKpp2011@gmail.com](mailto:dnkpp2011@gmail.com)

## License

[BSL-1.0](https://github.com/DNKpp/Simple-Log/blob/master/LICENSE_1_0.txt) (free, open source)

```text
          Copyright Dominic Koepke 2021 - 2021.
 Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
          https://www.boost.org/LICENSE_1_0.txt)
```

## Description
This library provides an implementation of a simple to use but versatile mathematically vector class. Due to the usage of lots of c++20 features, this library currently doesn't compile without errors on some compilers. Look at the table above to get an idea about, which compiler is able to compile the library tests.

One of the main intentions behind this vector implementation is being able to do as much during compile-time as possible, thus all functions are declared as constexpr.

## Installation with CMake
This library can easily be integrated into your project via CMake target_link_libraries command.

```cmake
target_link_libraries(
	<your_target_name>
	PRIVATE
	Simple_Vector
)
```
This will add the the include path "<simple_vector_install_dir>/include", thus you are able to include the headers via
```cpp
#include <Simple-Vector/Simple-Vector.hpp>
```

### FetchContent
It is possible fetching this library via CMakes FetchContent module.

```cmake
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(<your_project_name>)

include(FetchContent)

FetchContent_Declare(
	Simple_Vector
	GIT_REPOSITORY	https://github.com/DNKpp/Simple-Vector
	GIT_TAG		origin/v1.x
)
FetchContent_MakeAvailable(Simple_Vector)

target_link_libraries(
	<your_target_name>
	PUBLIC Simple_Vector
)
```