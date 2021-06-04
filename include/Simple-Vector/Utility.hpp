//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SIMPLE_VECTOR_UTILITY_HPP
#define SIMPLE_VECTOR_UTILITY_HPP

#pragma once

#include <concepts>
#include <functional>
#include <utility>

namespace sl::vec::detail
{
	template <class TTargetType, std::copy_constructible TFunc>
	struct convert_invoke_result_fn
	{
		template <class... TArgs>
		constexpr TTargetType operator ()(TArgs&&... args)
		{
			return static_cast<TTargetType>(std::invoke(func, std::forward<TArgs>(args)...));
		}

		TFunc func;
	};
}

namespace sl::vec::util
{
	template <class TTargetType, std::copy_constructible TFunc>
	constexpr auto convert_invoke_result(TFunc func) noexcept
	{
		return detail::convert_invoke_result_fn<TTargetType, TFunc>{ std::move(func) };
	}
}

#endif
