//          Copyright Dominic Koepke 2021 - 2022.
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

	/**
	 * \concept add_assignable
	 * \brief Checks if the given type can be used in mutable plus operations.
	 * \tparam TFrom Right-hand-side type of operation
	 * \tparam TTo Left-hand-side type of operation
	 */
	template <class TFrom, class TTo = TFrom>
	concept add_assignable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs += rhs };
	};

	/**
	 * \concept sub_assignable
	 * \brief Checks if the given type can be used in mutable minus operations.
	 * \tparam TFrom Right-hand-side type of operation
	 * \tparam TTo Left-hand-side type of operation
	 */
	template <class TFrom, class TTo = TFrom>
	concept sub_assignable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs -= rhs };
	};

	/**
	 * \concept mul_assignable
	 * \brief Checks if the given type can be used in mutable multiplication operations.
	 * \tparam TFrom Right-hand-side type of operation
	 * \tparam TTo Left-hand-side type of operation
	 */
	template <class TFrom, class TTo = TFrom>
	concept mul_assignable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs *= rhs };
	};

	/**
	 * \concept div_assignable
	 * \brief Checks if the given type can be used in mutable division operations.
	 * \tparam TFrom Right-hand-side type of operation
	 * \tparam TTo Left-hand-side type of operation
	 */
	template <class TFrom, class TTo = TFrom>
	concept div_assignable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs /= rhs };
	};

	/**
	 * \concept mod_assignable
	 * \brief Checks if the given type can be used in mutable modulo operations.
	 * \tparam TFrom Right-hand-side type of operation
	 * \tparam TTo Left-hand-side type of operation
	 */
	template <class TFrom, class TTo = TFrom>
	concept mod_assignable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs %= rhs };
	};

	/**
	 * \concept addable
	 * \brief Checks if the given type can be used in immutable plus operations.
	 * \tparam TFrom Right-hand-side type of operation
	 * \tparam TTo Left-hand-side type of operation and expected result type
	 */
	template <class TFrom, class TTo = TFrom>
	concept addable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs + rhs } -> std::convertible_to<TTo>;
	};

	/**
	 * \concept subable
	 * \brief Checks if the given type can be used in immutable minus operations.
	 * \tparam TFrom Right-hand-side type of operation
	 * \tparam TTo Left-hand-side type of operation and expected result type
	 */
	template <class TFrom, class TTo = TFrom>
	concept subable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs - rhs } -> std::convertible_to<TTo>;
	};

	/**
	 * \concept mulable
	 * \brief Checks if the given type can be used in immutable multiplication operations.
	 * \tparam TFrom Right-hand-side type of operation
	 * \tparam TTo Left-hand-side type of operation and expected result type
	 */
	template <class TFrom, class TTo = TFrom>
	concept mulable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs * rhs } -> std::convertible_to<TTo>;
	};

	/**
	 * \concept divable
	 * \brief Checks if the given type can be used in immutable division operations.
	 * \tparam TFrom Right-hand-side type of operation
	 * \tparam TTo Left-hand-side type of operation and expected result type
	 */
	template <class TFrom, class TTo = TFrom>
	concept divable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs / rhs } -> std::convertible_to<TTo>;
	};

	/**
	 * \concept modable
	 * \brief Checks if the given type can be used in immutable module operations.
	 * \tparam TFrom Right-hand-side type of operation
	 * \tparam TTo Left-hand-side type of operation and expected result type
	 */
	template <class TFrom, class TTo = TFrom>
	concept modable = requires(TTo lhs, TFrom rhs)
	{
		{ lhs % rhs } -> std::convertible_to<TTo>;
	};

	/**
	 * \concept binary_invokable_with_all_overloads_implicit_convertible_to
	 * \brief Checks if binary operation has all four overloads (one for each parameter constellation) and if all of those
	 * overloads are implicitly convertible to the result type.
	 * \tparam TBinaryOp Tpy of binary operation to check
	 * \tparam TResult Expected type to which each overload should be implicitly convertible to
	 * \tparam TArg1 Type of first parameter
	 * \tparam TArg2 Type of second parameter
	 */
	template <class TBinaryOp, class TResult, class TArg1, class TArg2>
	concept binary_invokable_with_all_overloads_implicit_convertible_to =
	std::is_convertible_v<std::invoke_result_t<TBinaryOp&, TArg1, TArg1>, TResult> &&
	std::is_convertible_v<std::invoke_result_t<TBinaryOp&, TArg1, TArg2>, TResult> &&
	std::is_convertible_v<std::invoke_result_t<TBinaryOp&, TArg2, TArg1>, TResult> &&
	std::is_convertible_v<std::invoke_result_t<TBinaryOp&, TArg2, TArg2>, TResult>;

	/**
	 * \concept value_type
	 * \brief Checks if the given type is regular and can be used in common arithmetically operations.
	 * \tparam T Type to check
	 */
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
