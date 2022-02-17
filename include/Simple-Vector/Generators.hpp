//          Copyright Dominic Koepke 2021 - 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SIMPLE_VECTOR_GENERATORS_HPP
#define SIMPLE_VECTOR_GENERATORS_HPP

#pragma once

#include "Concepts.hpp"

#include <iterator>
#include <ranges>
#include <type_traits>

namespace sl::vec::gen
{
	/** \addtogroup Generators
	 * @{
	 * \brief Generators are used to conveniently initialize Vector objects. 
	 */

	/**
	 * \brief Generator which repeatedly returns the same value on each invocation.
	 * \tparam T value type
	 */
	template <value_type T>
	struct fill
	{
		using value_type = std::remove_cvref_t<T>;

		const value_type value{};

		/**
		 * \brief Copies the stored value and returns it
		 * \return copy of value
		 */
		constexpr value_type operator ()() const noexcept(std::is_nothrow_copy_constructible_v<value_type>)
		{
			return value;
		}
	};

	template <value_type T>
	fill(T) -> fill<T>;

	/**
	 * \brief Generator which returns incrementing values on each invocation.
	 * \tparam T value type
	 */
	template <value_type T>
	/** \cond Requires */
		requires std::weakly_incrementable<T>
	/** \endcond */
	struct iota
	{
		using value_type = std::remove_cvref_t<T>;

		value_type value{};

		/**
		 * \brief post-increments the stored value and returns a copy
		 * \return copy of value
		 */
		constexpr value_type operator ()()
		{
			return value++;
		}
	};

	template <value_type T>
	iota(T) -> iota<T>;

	/**
	 * \brief Generator which retrieves its values out of the given source iterator.
	 * \tparam TRange type of source range
	 *
	 * \details If the given range is a borrowed range then the generator will use that type as range_type, otherwise the whole source range will
	 * be stored internally.
	 * \attention Requesting values from beyond the last element of the source range is undefined.
	 */
	template <std::ranges::forward_range TRange>
	class range
	{
	public:
		using range_type = std::conditional_t
		<
			std::ranges::borrowed_range<TRange>,
			TRange,
			std::remove_cvref_t<TRange>
		>;
		using iterator_type = std::ranges::iterator_t<range_type>;
		using value_type = std::ranges::range_value_t<TRange>;

		/**
		 * \brief Constructs the generator with a given range.
		 * \param range The source range to be used
		 */
		constexpr explicit range(TRange&& range)
			: m_Range{ std::forward<TRange>(range) },
			m_Iterator{ std::ranges::begin(m_Range) }
		{
		}

		/**
		 * \brief post-increments the stored iterator and returns a copy of the previously pointed element.
		 * \return copy of value
		 */
		constexpr value_type operator ()()
		{
			return *m_Iterator++;
		}

	private:
		range_type m_Range;
		iterator_type m_Iterator;
	};

	template <std::ranges::forward_range TRange>
	range(TRange&&) -> range<TRange>;

	/** @}*/
}

#endif
