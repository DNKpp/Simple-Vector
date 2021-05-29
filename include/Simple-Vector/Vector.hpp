//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SIMPLE_VECTOR_VECTOR_HPP
#define SIMPLE_VECTOR_VECTOR_HPP

#pragma once

#include "Concepts.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <execution>
#include <ranges>

namespace sl::vec
{
	template <value_type T, auto VDimensions>
		requires cardinality<VDimensions, std::size_t>
	class Vector
	{
	public:
		constexpr static auto dimensions{ VDimensions };
		using value_type = std::remove_cvref_t<T>;
		using storage_type = std::array<value_type, dimensions>;

		constexpr Vector() noexcept = default;
		constexpr ~Vector() noexcept = default;
		constexpr Vector(const Vector&) noexcept = default;
		constexpr Vector(Vector&&) noexcept = default;

		template <class... TArgs>
			requires (sizeof...(TArgs) == dimensions && (std::convertible_to<TArgs, T> && ...))
		constexpr explicit Vector(TArgs&&...args) noexcept :
			m_Values{ static_cast<T>(args)... }
		{
		}

		template <std::convertible_to<T> T2>
			requires (!std::same_as<T2, T>)
		explicit constexpr Vector(const Vector<T2, dimensions>& other) noexcept
		{
			std::transform
			(
				std::execution::unseq,
				std::cbegin(other),
				std::cend(other),
				std::begin(m_Values),
				[](T2 value) { return static_cast<T>(value); }
			);
		}

		constexpr Vector& operator =(const Vector&) noexcept = default;
		constexpr Vector& operator =(Vector&&) noexcept = default;

		[[nodiscard]]
		constexpr bool operator ==(const Vector&) const noexcept = default;

		template <std::integral TIndex>
		[[nodiscard]]
		constexpr const value_type& operator [](TIndex index) const noexcept
		{
			assert(std::in_range<std::size_t>(index) && "index must be in range of type std::size_t");
			return m_Values[static_cast<std::size_t>(index)];
		}

		template <std::integral TIndex>
		[[nodiscard]]
		constexpr value_type& operator [](TIndex index) noexcept
		{
			assert(std::in_range<std::size_t>(index) && "index must be in range of type std::size_t");
			return m_Values[static_cast<std::size_t>(index)];
		}

		template <auto VDimensions2 = VDimensions>
			requires (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr const value_type& x() const noexcept { return m_Values[0]; }

		template <auto VDimensions2 = VDimensions>
			requires (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr value_type& x() noexcept { return m_Values[0]; }

		template <auto VDimensions2 = VDimensions>
			requires (1 < VDimensions) && (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr const value_type& y() const noexcept { return m_Values[1]; }

		template <auto VDimensions2 = VDimensions>
			requires (1 < VDimensions) && (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr value_type& y() noexcept { return m_Values[1]; }

		template <auto VDimensions2 = VDimensions>
			requires (2 < VDimensions) && (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr const value_type& z() const noexcept { return m_Values[2]; }

		template <auto VDimensions2 = VDimensions>
			requires (2 < VDimensions) && (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr value_type& z() noexcept { return m_Values[2]; }

		template <std::convertible_to<T> T2>
		constexpr Vector& operator +=(const Vector<T2, dimensions>& other) noexcept
		{
			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::cbegin(other),
				std::begin(m_Values),
				[](const auto& lhs, const auto& rhs) { return static_cast<T>(lhs + rhs); }
			);
			return *this;
		}

		template <std::convertible_to<T> T2>
		constexpr Vector& operator -=(const Vector<T2, dimensions>& other) noexcept
		{
			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::cbegin(other),
				std::begin(m_Values),
				[](const auto& lhs, const auto& rhs) { return static_cast<T>(lhs - rhs); }
			);
			return *this;
		}

		template <std::convertible_to<T> T2>
		constexpr Vector& operator +=(T2&& value) noexcept
		{
			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::begin(m_Values),
				[&value](const auto& lhs) { return static_cast<T>(lhs + std::forward<T2>(value)); }
			);
			return *this;
		}

		template <std::convertible_to<T> T2>
		constexpr Vector& operator -=(T2&& value) noexcept
		{
			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::begin(m_Values),
				[&value](const auto& lhs) { return static_cast<T>(lhs - std::forward<T2>(value)); }
			);
			return *this;
		}

		template <std::convertible_to<T> T2>
		constexpr Vector& operator *=(T2&& value) noexcept
		{
			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::begin(m_Values),
				[&value](const auto& lhs) { return static_cast<T>(lhs * std::forward<T2>(value)); }
			);
			return *this;
		}

		template <std::convertible_to<T> T2>
		constexpr Vector& operator /=(T2&& value) noexcept
		{
			assert(value != 0 && "division by 0 is undefined.");

			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::begin(m_Values),
				[&value](const auto& lhs) { return static_cast<T>(lhs / std::forward<T2>(value)); }
			);
			return *this;
		}

		template <std::convertible_to<T> T2>
			requires modable<T>
		constexpr Vector& operator %=(T2&& value) noexcept
		{
			assert(value != 0 && "division by 0 is undefined.");

			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::begin(m_Values),
				[&value](const auto& lhs) { return lhs % static_cast<T>(value); }
			);
			return *this;
		}

		[[nodiscard]]
		constexpr auto begin() noexcept { return std::begin(m_Values); }

		[[nodiscard]]
		constexpr auto begin() const noexcept { return std::begin(m_Values); }

		[[nodiscard]]
		constexpr auto cbegin() const noexcept { return std::cbegin(m_Values); }

		[[nodiscard]]
		constexpr auto end() noexcept { return std::end(m_Values); }

		[[nodiscard]]
		constexpr auto end() const noexcept { return std::end(m_Values); }

		[[nodiscard]]
		constexpr auto cend() const noexcept { return std::cend(m_Values); }

		[[nodiscard]]
		constexpr auto rbegin() noexcept { return std::rbegin(m_Values); }

		[[nodiscard]]
		constexpr auto rbegin() const noexcept { return std::rbegin(m_Values); }

		[[nodiscard]]
		constexpr auto crbegin() const noexcept { return std::crbegin(m_Values); }

		[[nodiscard]]
		constexpr auto rend() noexcept { return std::rend(m_Values); }

		[[nodiscard]]
		constexpr auto rend() const noexcept { return std::rend(m_Values); }

		[[nodiscard]]
		constexpr auto crend() const noexcept { return std::crend(m_Values); }

	private:
		storage_type m_Values{};
	};

	template <class... T>
	Vector(T&&...) -> Vector<std::common_type_t<T...>, sizeof...(T)>;

	template <class T>
	struct vector_traits
	{
	};

	template <value_type T, auto VDimensions>
	struct vector_traits<Vector<T, VDimensions>>
	{
		using value_type = typename Vector<T, VDimensions>::value_type;

		static constexpr auto dimensions = Vector<T, VDimensions>::dimensions;
	};

	template <class T>
	using vector_value_t = typename vector_traits<std::remove_cvref_t<T>>::value_type;

	template <class T>
	constexpr auto vector_dims_v = vector_traits<std::remove_cvref_t<T>>::dimensions;

	template <class T>
	struct is_vectorial : std::false_type
	{
	};

	template <value_type T, auto VDimensions>
	struct is_vectorial<Vector<T, VDimensions>> : std::true_type
	{
	};

	template <class T>
	constexpr bool is_vectorial_v = is_vectorial<T>::value;

	template <class T>
	concept vectorial = is_vectorial_v<std::remove_cvref_t<T>>;

	template <vectorial TVector, add_assignable<TVector> T2>
	constexpr TVector operator +(TVector lhs, T2&& rhs) noexcept
	{
		lhs += std::forward<T2>(rhs);
		return lhs;
	}

	template <vectorial TVector, sub_assignable<TVector> T2>
	constexpr TVector operator -(TVector lhs, T2&& rhs) noexcept
	{
		lhs -= std::forward<T2>(rhs);
		return lhs;
	}

	template <vectorial TVector, mul_assignable<TVector> T2>
	constexpr TVector operator *(TVector lhs, T2&& rhs) noexcept
	{
		lhs *= std::forward<T2>(rhs);
		return lhs;
	}

	template <vectorial TVector, mul_assignable<TVector> T2>
	constexpr TVector operator *(T2&& lhs, TVector rhs) noexcept
	{
		rhs *= std::forward<T2>(lhs);
		return rhs;
	}

	template <vectorial TVector, div_assignable<TVector> T2>
	constexpr TVector operator /(TVector lhs, T2&& rhs) noexcept
	{
		lhs /= std::forward<T2>(rhs);
		return lhs;
	}

	template <vectorial TVector, mod_assignable<TVector> T2>
	constexpr TVector operator %(TVector lhs, T2&& rhs) noexcept
	{
		lhs %= std::forward<T2>(rhs);
		return lhs;
	}

	template <vectorial TVector>
	[[nodiscard]]
	constexpr vector_value_t<TVector> length_sq(const TVector& vector) noexcept
	{
		using T = vector_value_t<TVector>;
		return std::reduce
		(
			std::execution::unseq,
			std::ranges::cbegin(vector),
			std::ranges::cend(vector),
			T{},
			[](const T& val, const T& el) { return val + el * el; }
		);
	}

	template <vectorial TVector>
	[[nodiscard]]
	constexpr auto length(const TVector& vector) noexcept { return std::sqrt(length_sq(vector)); }

	template <vectorial TVector1, vectorial TVector2>
		requires mulable<vector_value_t<TVector2>, vector_value_t<TVector1>>
	[[nodiscard]]
	constexpr vector_value_t<TVector1> dot_product(const TVector1& lhs, const TVector2& rhs)
	{
		using T = vector_value_t<TVector1>;
		return std::transform_reduce
		(
			std::execution::unseq,
			std::cbegin(lhs),
			std::cend(lhs),
			std::cbegin(rhs),
			T{},
			std::plus<>{},
			[](const T& el1, const auto& el2) { return static_cast<T>(el1 * el2); }
		);
	}

	template <vectorial TVector>
		requires std::floating_point<vector_value_t<TVector>>
	[[nodiscard]]
	constexpr TVector normalize(TVector vec) noexcept
	{
		assert(vec != TVector{});

		return vec /= length(vec);
	}
}

#endif
