//          Copyright Dominic Koepke 2021 - 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "Simple-Vector/Generators.hpp"

#include <functional>
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

TEST_CASE("gen::range should actually store non-borrowed-ranges provided as rvalue-ref or value.", "[generator]")
{
	using Range_t = std::vector<int>;

	gen::range gen{ Range_t{} };

	REQUIRE(std::same_as<decltype(gen)::range_type, Range_t>);
}

TEST_CASE("gen::range should just store the range as provided if it is a borrowed_range.", "[generator]")
{
	using Range_t = std::vector<int>;
	Range_t src{};
	gen::range gen{ src };

	REQUIRE(std::same_as<decltype(gen)::range_type, std::add_lvalue_reference_t<Range_t>>);
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
