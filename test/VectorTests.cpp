//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch.hpp>

#include "Simple-Vector/Generators.hpp"
#include "Simple-Vector/Vector.hpp"

#include <ranges>

using namespace sl::vec;

namespace
{
	template <class TValueType, std::size_t VDims>
	constexpr Vector<TValueType, VDims> make_iota_vector(TValueType begin = {}) noexcept
	{
		if constexpr (std::integral<TValueType>)
		{
			return Vector<TValueType, VDims>{ gen::iota{ begin } };
		}
		else
		{
			return static_cast<Vector<TValueType, VDims>>(Vector<int, VDims>{ gen::iota{ static_cast<int>(begin) } });
		}
	}

	template <class TValueType, std::size_t VDims>
	constexpr Vector<TValueType, VDims> make_filled_vector(TValueType value = {}) noexcept
	{
		return Vector<TValueType, VDims>{ gen::fill{ value } };
	}
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE
(
	"vector_value_t should yield the same result as value_type of Vector.",
	"[vector][traits]",
	int,
	float,
	const int&,
	const int,
	int&&
)
#pragma warning(default: 26444)
{
	using vector_t = Vector<TestType, 1>;

	REQUIRE(std::same_as<typename vector_t::value_type, vector_value_t<vector_t>>);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"vector_dims_v should yield the same result as dimensions of Vector.",
	"[vector][traits]",
	((std::size_t VDims), VDims),
	(1),
	(2),
	(3)
)
#pragma warning(default: 26444)
{
	using vector_t = Vector<int, VDims>;

	REQUIRE(vector_t::dimensions == VDims);
	REQUIRE(vector_t::dimensions == vector_dims_v<vector_t>);
}

TEST_CASE("Vector types should be default constructible with zeros", "[Vector][construction]")
{
	constexpr Vector<int, 3> vec;

	REQUIRE(std::cmp_equal(vec.dimensions, 3));
	REQUIRE(std::cmp_equal(vec[0], 0));
	REQUIRE(std::cmp_equal(vec[1], 0));
	REQUIRE(std::cmp_equal(vec[2], 0));
}

TEST_CASE("Vector types should be constructible via generator", "[vector][construction]")
{
	constexpr int value = 42;
	constexpr Vector<int, 3> vec{ gen::fill{ value } };

	REQUIRE(std::ranges::all_of(vec, [](auto val) { return val == value; }));
}

#pragma warning(disable: 26444)
TEMPLATE_PRODUCT_TEST_CASE
(
	"Vector types should be explicitly convertible between different value_types.",
	"[vector][construction]",
	std::tuple,
	((Vector<int, 3>, Vector<std::size_t, 3>),
		(Vector<std::size_t, 5>, Vector<int, 5>))
)
#pragma warning(default: 26444)
{
	using Source_t = std::tuple_element_t<0, TestType>;
	using Target_t = std::tuple_element_t<1, TestType>;
	constexpr auto result = static_cast<Target_t>(Source_t{ gen::iota{ 1 } });

	REQUIRE(result == Target_t{ gen::iota{ vector_dims_v<Target_t> } });
}

#if __cpp_nontype_template_args >= 201911L
// well, clang sucks
#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"Vector types should be explicitly convertible between different dimension sizes.",
	"[vector][construction]",
	((std::size_t VSourceDims, std::size_t VTargetDims, auto VExpectedRange), VSourceDims, VTargetDims, VExpectedRange),
	(3, 2, std::to_array({ 1, 2 })),
	(3, 5, std::to_array({ 1, 2, 3, 0, 0 }))
)
#pragma warning(default: 26444)
{
	using SourceVector_t = Vector<int, VSourceDims>;
	using TargetVector_t = Vector<int, VTargetDims>;
	constexpr auto result = static_cast<TargetVector_t>(SourceVector_t{ gen::iota{ 1 } });

	REQUIRE(std::ranges::equal(result, VExpectedRange));
}
#endif

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
	constexpr Vector vec{ V... };

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
	REQUIRE(std::as_const(vec).x() == 1);
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
		REQUIRE(std::as_const(vec).y() == 2);
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
		REQUIRE(std::as_const(vec).z() == 3);
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

	REQUIRE(sub == Vector<int, VDims>{});
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"length_squared should calculate the squared length of given vectors",
	"[vector][algorithm]",
	((std::size_t VDims, int VExpected), VDims, VExpected),
	(1, 1),
	(2, 5),
	(3, 14)
)
#pragma warning(default: 26444)
{
	const auto vec = make_iota_vector<int, VDims>(1);

	auto squaredLength = sl::vec::length_squared(vec);

	REQUIRE(VExpected == squaredLength);
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

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"dot_product should calculate the dot product of the two given vectors",
	"[vector][algorithm]",
	((class TOther, std::size_t VDims, int VExpected), TOther, VDims, VExpected),
	(int, 1, 2),
	(int, 2, 8),
	(int, 3, 20),
	(float, 1, 2),
	(float, 2, 8),
	(float, 3, 20)
)
#pragma warning(default: 26444)
{
	const auto vec1 = make_iota_vector<int, VDims>(1);
	const auto vec2 = make_iota_vector<TOther, VDims>(2);

	int dotProd = sl::vec::dot_product(vec1, vec2);

	REQUIRE(dotProd == VExpected);
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE_SIG
(
	"normalized should scale vectors to length of 1.",
	"[vector][algorithm]",
	((std::size_t VDims), VDims),
	(1),
	(2),
	(3)
)
#pragma warning(default: 26444)
{
	const auto iotaBegin = GENERATE(as<float>{}, 1, 3, 10);
	const auto vec = make_iota_vector<float, VDims>(iotaBegin);

	const auto normalizedVec = normalized(vec);

	REQUIRE(sl::vec::length(normalizedVec) == Approx(1));
}
