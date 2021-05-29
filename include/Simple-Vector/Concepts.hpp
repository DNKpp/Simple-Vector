//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SIMPLE_VECTOR_CONCEPTS_HPP
#define SIMPLE_VECTOR_CONCEPTS_HPP

#pragma once

#include <concepts>
#include <type_traits>

namespace sl::vec::concepts
{
	template <class TFrom, class TTo = TFrom>
	concept add_assignable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs += rhs };
	};

	template <class TFrom, class TTo = TFrom>
	concept sub_assignable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs -= rhs };
	};

	template <class TFrom, class TTo = TFrom>
	concept mul_assignable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs *= rhs };
	};

	template <class TFrom, class TTo = TFrom>
	concept div_assignable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs /= rhs };
	};

	template <class TFrom, class TTo = TFrom>
	concept mod_assignable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs %= rhs };
	};

	template <class TFrom, class TTo = TFrom>
	concept addable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs + rhs } -> std::convertible_to<TTo>;
	};

	template <class TFrom, class TTo = TFrom>
	concept subable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs - rhs } -> std::convertible_to<TTo>;
	};

	template <class TFrom, class TTo = TFrom>
	concept mulable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs * rhs } -> std::convertible_to<TTo>;
	};

	template <class TFrom, class TTo = TFrom>
	concept divable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs / rhs } -> std::convertible_to<TTo>;
	};

	template <class TFrom, class TTo = TFrom>
	concept modable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs % rhs } -> std::convertible_to<TTo>;
	};

	template <class T>
	concept value_type = std::regular<std::remove_cvref_t<T>> &&
						addable<std::remove_cvref_t<T>> &&
						subable<std::remove_cvref_t<T>> &&
						mulable<std::remove_cvref_t<T>> &&
						divable<std::remove_cvref_t<T>>;

	template <auto V, class TSizeType>
	concept cardinality = std::integral<TSizeType> && std::is_convertible_v<decltype(V), TSizeType> && 0 < V;
}

#endif
