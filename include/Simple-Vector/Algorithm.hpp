//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SIMPLE_VECTOR_ALGORITHM_HPP
#define SIMPLE_VECTOR_ALGORITHM_HPP

#pragma once

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
}

#endif
