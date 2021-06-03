//          Copyright Dominic Koepke 2021 - 2021.
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
		using value_type = T;

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
		using value_type = T;

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

	/**
	 * \brief Generator which retrieves its values out of the given source iterator.
	 * \tparam TIterator type of source iterator
	 *
	 * \attention Requesting values from beyond the last element of the source range is undefined.
	 */
	template <std::forward_iterator TIterator>
		requires value_type<std::iter_value_t<TIterator>>
	class range
	{
	public:
		using value_type = std::iter_value_t<TIterator>;
		using iterator_type = TIterator;

		/**
		 * \brief Constructs the generator with a given range.
		 * \param range The source range to be used
		 */
		template <std::ranges::forward_range TRange>
		constexpr explicit range(TRange& range) :
			m_Itr{ std::begin(range) }
		{
		}

		/**
		 * \brief Constructs the generator with a given iterator.
		 * \param itr The source range to be used
		 */
		constexpr explicit range(TIterator itr) :
			m_Itr{ std::move(itr) }
		{
		}

		/**
		 * \brief post-increments the stored iterator and returns a copy of the previously pointed element.
		 * \return copy of value
		 */
		constexpr value_type operator ()()
		{
			return *m_Itr++;
		}

	private:
		iterator_type m_Itr;
	};

	template <std::ranges::forward_range TRange>
	range(TRange&) -> range<std::ranges::iterator_t<TRange>>;

	/** @}*/
}

#endif
