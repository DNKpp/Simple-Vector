//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

/* With inclusion of this header, everything of the library will be available.*/
#include <Simple-Vector/Simple-Vector.hpp>

#include <iostream>
#include <ranges>

int main()
{
	// make the Vector class locally available with less verbosity 
	using sl::vec::Vector;
	
	Vector<int, 2> intVec1;			// this will default construct all elements to 0
	intVec1 += 2;					// adds 2 to each element
	
	Vector<int, 2> intVec2{ 1, 3 }; // constructs a new vector with x == 1 and y == 3

	Vector difference = static_cast<Vector<float, 2>>(intVec1 - intVec2);	// binary vector + and - arithmetic is possible as well as casting to vectors with other value types
	auto distance = length(difference);										// length is a free function, which is luckily available due to ADL even if it sits in the sl::vec namespace
	
	auto tmp = difference.x();					// each vector has a .x() member function, which returns a ref to the first element
	tmp = difference.y();						// vectors with two or more dimensions also have a y.() function; lastly vectors with three or more dimensions have a .z() function
	tmp = difference[0];						// but users may also access the elements via operator []

	for (auto el : difference)					// or simply iterate over each element
	{
		std::cout << el << " ";
	}
	std::cout << std::endl;

	for (auto el : intVec2 | std::views::take(1)) // vectors are also fully compatible to the new ranges or the older stl algorithms
	{
		std::cout << el;
	}
	
	Vector doubles{ 42. };		// this will be deduced to Vector<double, 1>
	auto doubles2 = static_cast<Vector<double, 3>>(doubles);		// casting between different dimensions is also possible
}
