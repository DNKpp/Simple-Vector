//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch.hpp>

#include "Simple-Vector/Vector.hpp"

#include <ranges>

using namespace sl::vec;

namespace
{
	template <class TValueType, std::size_t VDims>
	constexpr Vector<TValueType, VDims> make_iota_vector(TValueType begin) noexcept
	{
		Vector<TValueType, VDims> vec;
		std::iota(std::begin(vec), std::end(vec), begin);
		return vec;
	}
}

TEST_CASE("Vector types should be default constructible", "[Vector][construction]")
{
	Vector<int, 3> vec;

	REQUIRE(std::cmp_equal(vec.dimensions, 3));
	REQUIRE(std::cmp_equal(vec[0], 0));
	REQUIRE(std::cmp_equal(vec[1], 0));
	REQUIRE(std::cmp_equal(vec[2], 0));
}

#pragma warning(disable: 26444)
#if __cpp_nontype_template_args < 201911L
TEMPLATE_TEST_CASE_SIG
(
	"Vector types should be constructible by direct initialization",
	"[Vector][construction]",
	((auto... V), V...),
	(1, 2, 3)
)
#else
TEMPLATE_TEST_CASE_SIG
(
	"Vector types should be constructible by direct initialization",
	"[Vector][construction]",
	((auto... V), V...),
	(1, 2, 3),
	(4, 3, 2.f, 1.)
)
#endif
#pragma warning(default: 26444)
{
	Vector vec{ V... };
	REQUIRE(std::cmp_equal(vec.dimensions, sizeof...(V)));
}

TEST_CASE("Vector types should be iteratable", "[Vector][iteration]")
{
	constexpr std::size_t dims = 5;
	auto vec = make_iota_vector<int, 5>(1);

	REQUIRE(std::cmp_equal(dims, std::ranges::distance(vec)));

	REQUIRE(std::cmp_equal(vec[0], 1));
	REQUIRE(std::cmp_equal(vec[1], 2));
	REQUIRE(std::cmp_equal(vec[2], 3));
	REQUIRE(std::cmp_equal(vec[3], 4));
	REQUIRE(std::cmp_equal(vec[4], 5));

	REQUIRE(std::equal(std::begin(vec), std::end(vec), std::begin(std::as_const(vec)), std::end(std::as_const(vec))));
	REQUIRE(std::equal(std::begin(vec), std::end(vec), std::cbegin(vec), std::cend(vec)));
}

TEST_CASE("Vector types should be reverse iteratable", "[Vector][iteration]")
{
	auto vec = make_iota_vector<int, 5>(1);

	REQUIRE(std::cmp_equal(*std::rbegin(vec), 5));
	REQUIRE(std::cmp_equal(*(std::rbegin(vec) + 1), 4));
	REQUIRE(std::cmp_equal(*(std::rbegin(vec) + 2), 3));
	REQUIRE(std::cmp_equal(*(std::rbegin(vec) + 3), 2));
	REQUIRE(std::cmp_equal(*(std::rbegin(vec) + 4), 1));

	REQUIRE(std::equal(std::rbegin(vec), std::rend(vec), std::rbegin(std::as_const(vec)), std::rend(std::as_const(vec))));
	REQUIRE(std::equal(std::rbegin(vec), std::rend(vec), std::crbegin(vec), std::crend(vec)));
}

TEST_CASE("Vector can be created via make function", "[Vector]")
{
	constexpr int value = 1337;
	auto vec = Vector<int, 5>::make(value);

	REQUIRE(std::ranges::all_of(vec, [value](auto v){ return value == v; }));
}

TEST_CASE("Vector can be created via zero function", "[Vector]")
{
	auto vec = Vector<int, 5>::zero();

	REQUIRE(std::ranges::all_of(vec, [](auto v){ return 0 == v; }));
}

namespace
{
	template <class T>
	concept has_y_function = requires(T t)
	{
		{ t.y() };
	};

	template <class T>
	concept has_z_function = requires(T t)
	{
		{ t.z() };
	};
}

TEST_CASE("Vector has x member function", "[Vector]")
{
	auto vec = make_iota_vector<int, 3>(1);

	REQUIRE(vec.x() == 1);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"Vector has y member function",
	"[Vector]",
	((std::size_t VDims), VDims),
	(1),
	(2),
	(3)
)
#pragma warning(default: 26444)
{
	auto vec = make_iota_vector<int, VDims>(1);
	using Vector_t = decltype(vec);

	if constexpr (has_y_function<Vector_t>)
	{
		REQUIRE(std::cmp_less_equal(2, Vector_t::dimensions));
		REQUIRE(vec.y() == 2);
	}
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"Vector has z member function",
	"[Vector]",
	((std::size_t VDims), VDims),
	(2),
	(3),
	(4)
)
#pragma warning(default: 26444)
{
	auto vec = make_iota_vector<int, VDims>(1);
	using Vector_t = decltype(vec);

	if constexpr (has_z_function<Vector_t>)
	{
		REQUIRE(std::cmp_less_equal(3, Vector_t::dimensions));
		REQUIRE(vec.z() == 3);
	}
}

TEST_CASE("Vector should be equality comparable.", "[vector]")
{
	constexpr std::size_t dims = 2;
	const auto [firstBegin, secBegin, expected] = GENERATE
	(
		table<int,
		int,
		bool>({
			{ 1, 1, true},
			{ 1, 0, false},
			{ -2, -2, true},
			{ -4, 4, false}
			})
	);

	const auto vec1 = make_iota_vector<int, dims>(firstBegin);
	const auto vec2 = make_iota_vector<int, dims>(secBegin);
	const auto eqResult = vec1 == vec2;
	const auto neqResult = vec1 != vec2;
	REQUIRE(eqResult == expected);
	REQUIRE(neqResult != expected);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"Vector should be addable with types equally or convertible to its value_type.",
	"[vector][arithmetic]",
	((class T, std::size_t VDims), T, VDims),
	(int, 1),
	(int, 2),
	(int, 3),
	(unsigned int, 4),
	(unsigned int, 5),
	(unsigned int, 6),
	(float, 7),
	(float, 8),
	(float, 9)
)
#pragma warning(default: 26444)
{
	const Vector vec = make_iota_vector<int, VDims>(1);
	const auto value = GENERATE(as<T>{}, 0, 1, 5, -1, -2);
	const Vector sum = vec + value;

	REQUIRE
	(
		std::ranges::equal
		(
			std::ranges::transform_view{ vec, [value](auto v){ return v + static_cast<int>(value); } },
			sum
		)
	);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"Vector should be subtractable with types equally or convertible to its value_type.",
	"[vector][arithmetic]",
	((class T, std::size_t VDims), T, VDims),
	(int, 1),
	(int, 2),
	(int, 3),
	(unsigned int, 4),
	(unsigned int, 5),
	(unsigned int, 6),
	(float, 7),
	(float, 8),
	(float, 9)
)
#pragma warning(default: 26444)
{
	const Vector vec = make_iota_vector<int, VDims>(1);
	const auto value = GENERATE(as<T>{}, 0, 1, 5, -1, -2);
	const Vector diff = vec - value;

	REQUIRE(diff + value == vec);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"Vector should be commutatively multiplyable with types equally or convertible to its value_type.",
	"[vector][arithmetic]",
	((class T, std::size_t VDims), T, VDims),
	(int, 1),
	(int, 2),
	(int, 3),
	(unsigned int, 4),
	(unsigned int, 5),
	(unsigned int, 6),
	(float, 7),
	(float, 8),
	(float, 9)
)
#pragma warning(default: 26444)
{
	const Vector vec = make_iota_vector<int, VDims>(1);
	auto value = GENERATE(as<T>{}, 0, 1, 5, -1, -2);
	const Vector product = vec * value;
	const Vector commutativeProduct = value * vec;

	REQUIRE
	(
		std::ranges::equal
		(
			std::ranges::transform_view{ vec, [value](auto v){ return static_cast<int>(v * value); } },
			product
		)
	);
	REQUIRE(product == commutativeProduct);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"Vector should be dividable with types equally or convertible to its value_type.",
	"[vector][arithmetic]",
	((class T, std::size_t VDims), T, VDims),
	(int, 1),
	(int, 2),
	(int, 3),
	(unsigned int, 4),
	(unsigned int, 5),
	(unsigned int, 6),
	(float, 7),
	(float, 8),
	(float, 9)
)
#pragma warning(default: 26444)
{
	const Vector vec = make_iota_vector<int, VDims>(1);
	const auto value = GENERATE(as<T>{}, 1, 5, -1, -2);
	const Vector fraction = vec / value;

	REQUIRE
	(
		std::ranges::equal
		(
			fraction,
			std::ranges::transform_view{ vec, [value](auto v){ return static_cast<int>(v / value); } }
		)
	);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"Vector should be modable with types equally or convertible to its value_type.",
	"[vector][arithmetic]",
	((class T, std::size_t VDims), T, VDims),
	(int, 1),
	(int, 2),
	(int, 3),
	(unsigned int, 4),
	(unsigned int, 5),
	(unsigned int, 6),
	(float, 7),
	(float, 8),
	(float, 9)
)
#pragma warning(default: 26444)
{
	const Vector vec = make_iota_vector<int, VDims>(1);
	const auto value = GENERATE(as<T>{}, 1, 5, -1, -2);

	const Vector remainder = vec % value;

	REQUIRE
	(
		std::ranges::equal
		(
			remainder,
			std::views::transform(vec, [value](auto v){ return v % static_cast<int>(value); })
		)
	);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"Vector should be addable by Vectors, whichs value_types are equally or convertible to the targets value_type.",
	"[vector][arithmetic]",
	((class T, std::size_t VDims), T, VDims),
	(int, 1),
	(int, 2),
	(int, 3),
	(unsigned int, 4),
	(unsigned int, 5),
	(unsigned int, 6),
	(float, 7),
	(float, 8),
	(float, 9)
)
#pragma warning(default: 26444)
{
	const auto vec1 = make_iota_vector<int, VDims>(1);
	const auto vec2 = make_iota_vector<T, VDims>(1);

	const Vector sum = vec1 + vec2;

	REQUIRE(sum == 2 * vec1);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"Vector should be subtractable by Vectors, whichs value_types are equally or convertible to the targets value_type.",
	"[vector][arithmetic]",
	((class T, std::size_t VDims), T, VDims),
	(int, 1),
	(int, 2),
	(int, 3),
	(unsigned int, 4),
	(unsigned int, 5),
	(unsigned int, 6),
	(float, 7),
	(float, 8),
	(float, 9)
)
#pragma warning(default: 26444)
{
	const auto vec1 = make_iota_vector<int, VDims>(1);
	const auto vec2 = make_iota_vector<T, VDims>(1);

	const Vector sub = vec1 - vec2;

	REQUIRE(sub == Vector<int, VDims>::zero());
}


#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"length_sq should calculate the squared length of given vectors",
	"[vector][algorithm]",
	((std::size_t VDims, int VExpected), VDims, VExpected),
	(1, 1),
	(2, 5),
	(3, 14)
)
#pragma warning(default: 26444)
{
	const auto vec = make_iota_vector<int, VDims>(1);

	const auto length_sq = sl::vec::length_sq(vec);
	
	REQUIRE(VExpected == length_sq);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"length should calculate the length of given vectors",
	"[vector][algorithm]",
	((std::size_t VDims, int VExpectedSq), VDims, VExpectedSq),
	(1, 1),
	(2, 5),
	(3, 14)
)
#pragma warning(default: 26444)
{
	const auto vec = make_iota_vector<int, VDims>(1);

	const auto length = sl::vec::length(vec);
	
	REQUIRE(length == Approx(std::sqrt(VExpectedSq)));
}

