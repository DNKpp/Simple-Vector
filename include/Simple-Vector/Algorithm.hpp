//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SIMPLE_VECTOR_ALGORITHM_HPP
#define SIMPLE_VECTOR_ALGORITHM_HPP

#pragma once

#include "Concepts.hpp"

#include <algorithm>
#include <concepts>
#include <execution>
#include <ranges>

namespace sl::vec
{
	/**
	 * \brief Applies the given unary operation to each element and writes into result.
	 * \tparam TRange Type of range
	 * \tparam TOut result destination
	 * \tparam TUnaryOp Type of unary operation
	 * \param range source range
	 * \param result destination where the result will be written to
	 * \param unaryOp unary operation applied to each element of the source range
	 *
	 * \details This functions serves as a convenient wrapper, which is used for enabling constexpr when possible and otherwise use the potentially
	 * benefit of vectorization.
	 * If this function is called in a non-constant-evaluated context it uses the std::execution::unseq policy. Due to the lack of a constexpr declaration
	 * of these overload, this function will otherwise fallback to the std::ranges::transform algorithm.
	 * For further details read here: https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t
	 */
	template
	<
		std::ranges::input_range TRange,
		std::weakly_incrementable TOut,
		std::copy_constructible TUnaryOp
	>
	/** \cond Requires */
		requires std::indirectly_writable
		<
			TOut,
			std::indirect_result_t<TUnaryOp&, std::ranges::iterator_t<TRange>>
		>
	/** \endcond */
	constexpr void transform_unseq(TRange&& range, TOut result, TUnaryOp unaryOp)
	{
		if (std::is_constant_evaluated())
		{
			std::ranges::transform(std::forward<TRange>(range), result, std::ref(unaryOp));
		}
		else // ReSharper disable once CppUnreachableCode
		{
			std::transform
			(
				std::execution::unseq,
				std::ranges::cbegin(range),
				std::ranges::cend(range),
				result,
				std::ref(unaryOp)
			);
		}
	}

	/**
	 * \brief Applies the given unary operation to each element and writes into result.
	 * \tparam TRange1 Type of range1
	 * \tparam TRange2 Type of range2
	 * \tparam TOut result destination
	 * \tparam TBinaryOp Type of binary operation
	 * \param range1 source range1
	 * \param range2 source range2
	 * \param result destination where the result will be written to
	 * \param binaryOp binary operation applied to each pair of elements of both source ranges
	 *
	 * \details This functions serves as a convenient wrapper, which is used for enabling constexpr when possible and otherwise use the potentially
	 * benefit of vectorization.
	 * If this function is called in a non-constant-evaluated context it uses the std::execution::unseq policy. Due to the lack of a constexpr declaration
	 * of these overload, this function will otherwise fallback to the std::ranges::transform algorithm.
	 * For further details read here: https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t
	 */
	template
	<
		std::ranges::input_range TRange1,
		std::ranges::input_range TRange2,
		std::weakly_incrementable TOut,
		std::copy_constructible TBinaryOp
	>
	/** \cond Requires */
		requires std::indirectly_writable
		<
			TOut,
			std::indirect_result_t<TBinaryOp&, std::ranges::iterator_t<TRange1>, std::ranges::iterator_t<TRange2>>
		>
	/** \endcond */
	constexpr void transform_unseq
	(
		TRange1&& range1,
		TRange2&& range2,
		TOut result,
		TBinaryOp binaryOp
	)
	{
		if (std::is_constant_evaluated())
		{
			std::ranges::transform(std::forward<TRange1>(range1), std::forward<TRange2>(range2), result, std::ref(binaryOp));
		}
		else // ReSharper disable once CppUnreachableCode
		{
			std::transform
			(
				std::execution::unseq,
				std::ranges::cbegin(range1),
				std::ranges::cend(range1),
				std::ranges::cbegin(range2),
				result,
				std::ref(binaryOp)
			);
		}
	}

	/**
	 * \brief Applies the unaryOp to each elements from the range and reduces the results (possibly permuted and aggregated unspecified manner)
	 * along with the initial value init over binaryOp.
	 * \tparam TRange Type of range
	 * \tparam T result type
	 * \tparam TBinaryOp Type of binary operation
	 * \tparam TUnaryOp Type of unary operation
	 * \param range source range1
	 * \param init initial value
	 * \param binaryOp binary operation applied to the results of unaryOp along with the initial value
	 * \param unaryOp unary operation applied to each elements from the range
	 *
	 * \details This functions serves as a convenient wrapper, which is used for enabling constexpr when possible and otherwise use the potentially
	 * benefit of vectorization.
	 * If this function is called in a non-constant-evaluated context it uses the std::execution::unseq policy. Due to the lack of a constexpr declaration
	 * of these overload, this function will otherwise fallback to the std::transform_reduce algorithm.
	 * For further details read here: https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t
	 */
	template
	<
		std::ranges::input_range TRange,
		std::move_constructible T,
		std::copy_constructible TBinaryOp,
		std::copy_constructible TUnaryOp
	>
	/** \cond Requires */
		requires std::indirectly_unary_invocable
				<
					TUnaryOp&,
					std::ranges::iterator_t<TRange>
				> &&
				binary_invokable_with_all_overloads_implicit_convertible_to
				<
					TBinaryOp&,
					T,
					T,
					std::indirect_result_t<TUnaryOp&, std::ranges::iterator_t<TRange>>
				>
	/** \endcond */
	constexpr T transform_reduce_unseq
	(
		TRange&& range,
		T init,
		TBinaryOp binaryOp,
		TUnaryOp unaryOp
	)
	{
		if (std::is_constant_evaluated())
		{
			return std::transform_reduce
			(
				std::ranges::cbegin(range),
				std::ranges::cend(range),
				init,
				std::ref(binaryOp),
				std::ref(unaryOp)
			);
		}
		// ReSharper disable once CppUnreachableCode
		return std::transform_reduce
		(
			std::execution::unseq,
			std::ranges::cbegin(range),
			std::ranges::cend(range),
			init,
			std::ref(binaryOp),
			std::ref(unaryOp)
		);
	}

	/**
	 * \brief Applies the binaryOp2 to each pair of elements from both ranges and reduces the results (possibly permuted and aggregated unspecified manner)
	 * along with the initial value init over binaryOp1.
	 * \tparam TRange1 Type of range1
	 * \tparam TRange2 Type of range2
	 * \tparam T result type
	 * \tparam TBinaryOp1 Type of inner binary operation
	 * \tparam TBinaryOp2 Type of outer binary operation
	 * \param range1 source range1
	 * \param range2 source range2
	 * \param init initial value
	 * \param binaryOp1 binary operation applied to the results of binaryOp2 along with the initial value
	 * \param binaryOp2 binary operation applied to each pair of elements from both ranges
	 *
	 * \details This functions serves as a convenient wrapper, which is used for enabling constexpr when possible and otherwise use the potentially
	 * benefit of vectorization.
	 * If this function is called in a non-constant-evaluated context it uses the std::execution::unseq policy. Due to the lack of a constexpr declaration
	 * of these overload, this function will otherwise fallback to the std::transform_reduce algorithm.
	 * For further details read here: https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t
	 */
	template
	<
		std::ranges::input_range TRange1,
		std::ranges::input_range TRange2,
		std::move_constructible T,
		std::copy_constructible TBinaryOp1,
		std::copy_constructible TBinaryOp2
	>
	/** \cond Requires */
		requires std::invocable
				<
					TBinaryOp2&,
					std::iter_reference_t<std::ranges::iterator_t<TRange1>>,
					std::iter_reference_t<std::ranges::iterator_t<TRange2>>
				> &&
				binary_invokable_with_all_overloads_implicit_convertible_to
				<
					TBinaryOp1&,
					T,
					T,
					std::indirect_result_t<TBinaryOp2&, std::ranges::iterator_t<TRange1>, std::ranges::iterator_t<TRange2>>
				>
	/** \endcond */
	constexpr T transform_reduce_unseq
	(
		TRange1&& range1,
		TRange2&& range2,
		T init,
		TBinaryOp1 binaryOp1,
		TBinaryOp2 binaryOp2
	)
	{
		if (std::is_constant_evaluated())
		{
			return std::transform_reduce
			(
				std::ranges::cbegin(range1),
				std::ranges::cend(range1),
				std::ranges::cbegin(range2),
				init,
				std::ref(binaryOp1),
				std::ref(binaryOp2)
			);
		}
		// ReSharper disable once CppUnreachableCode
		return std::transform_reduce
		(
			std::execution::unseq,
			std::ranges::cbegin(range1),
			std::ranges::cend(range1),
			std::ranges::cbegin(range2),
			init,
			std::ref(binaryOp1),
			std::ref(binaryOp2)
		);
	}
}

#endif
