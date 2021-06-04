//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch.hpp>

#include "Simple-Vector/Algorithm.hpp"

#include <array>

using namespace sl::vec;

namespace
{
	template <class TFunc = std::identity>
	class invoke_counter
	{
	public:
		constexpr invoke_counter(int& counter) :
			m_Counter{ std::addressof(counter) }
		{
		}

		constexpr invoke_counter(TFunc func, int& counter) :
			m_Func{ std::move(func) },
			m_Counter{ std::addressof(counter) }
		{
		}

		template <class... TArgs>
		constexpr auto operator ()(TArgs&&... args)
		{
			++*m_Counter;
			return std::invoke(m_Func, std::forward<TArgs>(args)...);
		}

	private:
		TFunc m_Func{};
		int* m_Counter = nullptr;
	};
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"constexpr unary transform_unseq should invoke function for each element in range.",
	"[algorithm][constexpr]",
	((std::size_t VSize), VSize),
	(0),
	(1),
	(10)
)
{
	constexpr int invocations = []
	{
		std::array<int, VSize> src{};
		int counter = 0;
		transform_unseq(src, std::ranges::begin(src), invoke_counter{ counter });
		return counter;
	}();

	REQUIRE(invocations == VSize);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"unary transform_unseq should invoke function for each element in range.",
	"[algorithm]",
	((std::size_t VSize), VSize),
	(0),
	(1),
	(10)
)
{
	const int invocations = []
	{
		std::array<int, VSize> src{};
		int counter = 0;
		transform_unseq(src, std::ranges::begin(src), invoke_counter{ counter });
		return counter;
	}();

	REQUIRE(invocations == VSize);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"constexpr binary transform_unseq should invoke function for each element in range.",
	"[algorithm][constexpr]",
	((std::size_t VSize), VSize),
	(0),
	(1),
	(10)
)
{
	constexpr int invocations = []
	{
		std::array<int, VSize> src{};
		int counter = 0;
		transform_unseq(src, src, std::ranges::begin(src), invoke_counter{ [&](auto lhs, auto rhs) { return lhs; }, counter });
		return counter;
	}();

	REQUIRE(invocations == VSize);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"binary transform_unseq should invoke function for each element of Vector.",
	"[algorithm]",
	((std::size_t VSize), VSize),
	(0),
	(1),
	(10)
)
{
	const int invocations = []
	{
		std::array<int, VSize> src{};
		int counter = 0;
		transform_unseq(src, src, std::ranges::begin(src), invoke_counter{ [&](auto lhs, auto rhs) { return lhs; }, counter });
		return counter;
	}();

	REQUIRE(invocations == VSize);
}
