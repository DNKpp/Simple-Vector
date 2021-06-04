//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SIMPLE_VECTOR_CONCEPTS_HPP
#define SIMPLE_VECTOR_CONCEPTS_HPP

#pragma once

#include <concepts>
#include <type_traits>

namespace sl::vec
{
	/** \addtogroup Concepts
	 * @{
	 */

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

	/** @}*/

	/** \addtogroup TypeTraits
	 * @{
	 */

	/**
	 * \brief Uniform interface to Vector types.
	 * \tparam T type of interest
	 */
	template <class T>
	struct vector_traits
	{
	};

	/**
	 * \brief Convenience alias type to the value_type of Vectors
	 * \tparam T type of interest
	 */
	template <class T>
	using vector_value_t = typename vector_traits<std::remove_cvref_t<T>>::value_type;

	/**
	 * \brief Convenience constant to the dimensions of Vectors
	 * \tparam T type of interest
	 */
	template <class T>
	constexpr auto vector_dims_v = vector_traits<std::remove_cvref_t<T>>::dimensions;

	/**
	 * \brief Checks whether T is a vector type.
	 * \tparam T a type to check
	 */
	template <class T>
	struct is_vectorial : std::false_type
	{
	};

	/**
	 * \brief Shortcut checking for vectorial classes
	 * \tparam T A type to check
	 */
	template <class T>
	constexpr bool is_vectorial_v = is_vectorial<T>::value;

	/** @}*/

	/** \addtogroup Concepts
	 * @{
	 */

	/**
	 * \concept vectorial
	 * \brief Concept checking for vectorial types
	 * \tparam T A type to check
	 */
	template <class T>
	concept vectorial = is_vectorial_v<std::remove_cvref_t<T>>;

	/** @}*/
}

#endif
