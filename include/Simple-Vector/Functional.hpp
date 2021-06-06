//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SIMPLE_VECTOR_FUNCTIONAL_HPP
#define SIMPLE_VECTOR_FUNCTIONAL_HPP

#pragma once

#include "Concepts.hpp"

#include <concepts>
#include <functional>
#include <utility>

namespace sl::vec::detail
{
	template <class TTargetType, std::copy_constructible TFunc>
	struct cast_invoke_result_fn
	{
		template <class... TArgs>
		constexpr TTargetType operator ()(TArgs&&... args)
		{
			return static_cast<TTargetType>(std::invoke(func, std::forward<TArgs>(args)...));
		}

		TFunc func;
	};
}

namespace sl::vec::fn
{
	/** \addtogroup Functional
	 * @{
	 */

	/**
	 * \brief Factory function creating a wrapper function object, which invokes the provided function object and casts
	 * the result into the given type.
	 * \tparam TTargetType Type to cast
	 * \tparam TFunc Type of function object
	 * \param func The wrapped function object
	 * \return function object
	 */
	template <class TTargetType, std::copy_constructible TFunc>
	constexpr auto cast_invoke_result(TFunc func) noexcept
	{
		return detail::cast_invoke_result_fn<TTargetType, TFunc>{ std::move(func) };
	}

	/** @}*/
}

#endif
