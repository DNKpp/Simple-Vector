// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch.hpp>

#include "Simple-Vector/Generators.hpp"

#include <numeric>

using namespace sl::vec;

TEST_CASE("gen::fill should repeatedly return the exact same value on each invocation.", "[generator]")
{
	const auto value = GENERATE(0, -42, 42);

	gen::fill gen{ value };

	for (int i = 0; i < 10; ++i)
	{
		const auto result = std::invoke(gen);

		REQUIRE(result == value);
	}
}

TEST_CASE("gen::iota should return contiguously incrementing values on each invocation.", "[generator]")
{
	const auto begin = GENERATE(0, -42, 42);

	gen::iota gen{ begin };

	for (int i = 0; i < 10; ++i)
	{
		const auto result = std::invoke(gen);

		REQUIRE(result == begin + i);
	}
}

TEST_CASE("gen::range should yield results taken from its source range", "[generator]")
{
	std::vector<int> src(10);
	std::iota(std::begin(src), std::end(src), 0);
	gen::range gen{ src };

	for (int i = 0; i < 10; ++i)
	{
		const auto result = std::invoke(gen);

		REQUIRE(result == i);
	}
}