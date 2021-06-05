//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch.hpp>

#include "Simple-Vector/Algorithm.hpp"

#include <array>
#include <functional>

using namespace sl::vec;

namespace
{
	template <class TFunc = std::identity>
	class invoke_counter
	{
	public:
		constexpr explicit invoke_counter(int& counter) :
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
	"binary transform_unseq should invoke function for each element in range.",
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

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"constexpr binary transform_reduce_unseq should invoke first binary operation for each element in range.",
	"[algorithm]",
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
		auto result = transform_reduce_unseq
			(src, src, 0, invoke_counter{ [&](auto lhs, auto rhs) { return lhs; }, counter }, std::multiplies{});
		return counter;
	}();

	REQUIRE(invocations == VSize);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"binary transform_reduce_unseq should invoke first binary operation for each element in range.",
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
		auto result = transform_reduce_unseq
			(src, src, 0, invoke_counter{ [&](auto lhs, auto rhs) { return lhs; }, counter }, std::multiplies{});
		return counter;
	}();

	REQUIRE(invocations == VSize);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"constexpr binary transform_reduce_unseq should invoke second binary operation for each element in range.",
	"[algorithm]",
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
		auto result = transform_reduce_unseq
			(src, src, 0, std::multiplies{}, invoke_counter{ [&](auto lhs, auto rhs) { return lhs; }, counter });
		return counter;
	}();

	REQUIRE(invocations == VSize);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"binary transform_reduce_unseq should invoke second binary operation for each element in range.",
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
		auto result = transform_reduce_unseq
			(src, src, 0, std::multiplies{}, invoke_counter{ [&](auto lhs, auto rhs) { return lhs; }, counter });
		return counter;
	}();

	REQUIRE(invocations == VSize);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"constexpr unary transform_reduce_unseq should invoke binary operation for each element in range.",
	"[algorithm]",
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
		auto result = transform_reduce_unseq
			(src, 0, invoke_counter{ [&](auto lhs, auto rhs) { return lhs; }, counter }, std::identity{});
		return counter;
	}();

	REQUIRE(invocations == VSize);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"unary transform_reduce_unseq should invoke binary operation for each element in range.",
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
		auto result = transform_reduce_unseq
			(src, 0, invoke_counter{ [&](auto lhs, auto rhs) { return lhs; }, counter }, std::identity{});
		return counter;
	}();

	REQUIRE(invocations == VSize);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"constexpr unary transform_reduce_unseq should invoke unary operation for each element in range.",
	"[algorithm]",
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
		auto result = transform_reduce_unseq(src, 0, std::multiplies{}, invoke_counter{ counter });
		return counter;
	}();

	REQUIRE(invocations == VSize);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"unary transform_reduce_unseq should invoke unary operation for each element in range.",
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
		auto result = transform_reduce_unseq(src, 0, std::multiplies{}, invoke_counter{ counter });
		return counter;
	}();

	REQUIRE(invocations == VSize);
}
