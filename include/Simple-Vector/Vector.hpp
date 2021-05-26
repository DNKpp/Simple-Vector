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
#include <concepts>
#include <execution>

namespace sl::vec
{
	template <concepts::value_type T, auto VDimensions>
		requires concepts::cardinality<VDimensions, std::size_t>
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
		constexpr Vector(TArgs&&...args) noexcept :
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

		[[nodiscard]]
		constexpr static Vector make(const T& value) noexcept
		{
			Vector tmp;
			tmp.m_Values.fill(value);
			return tmp;
		}

		[[nodiscard]]
		constexpr static Vector zero() noexcept { return make({}); }

		template <std::integral TIndex>
		[[nodiscard]]
		constexpr const value_type& operator [](TIndex index) const noexcept
		{
			assert(std::in_range<std::size_t>(index) && "index must be in range of type std::size_t");
			return m_Values[static_cast<std::size_t>(index)];
		}

		template <std::integral TIndex>
		[[nodiscard]]
		constexpr const value_type& operator [](TIndex index) noexcept
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
				std::cbegin(other.m_Values),
				std::begin(m_Values),
				[](const auto& lhs, const auto& rhs) { return lhs + static_cast<T>(rhs); }
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
				std::cbegin(other.m_Values),
				std::begin(m_Values),
				[](const auto& lhs, const auto& rhs) { return lhs - static_cast<T>(rhs); }
			);
			return *this;
		}

		template <std::convertible_to<T> T2>
		constexpr Vector& operator +=(const T2& value) noexcept
		{
			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::end(m_Values),
				[&value](const auto& lhs) { return lhs + static_cast<T>(value); }
			);
			return *this;
		}

		template <std::convertible_to<T> T2>
		constexpr Vector& operator -=(const T2& value) noexcept
		{
			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::end(m_Values),
				[&value](const auto& lhs) { return lhs - static_cast<T>(value); }
			);
			return *this;
		}

		template <std::convertible_to<T> T2>
		constexpr Vector& operator *=(const T2& value) noexcept
		{
			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::end(m_Values),
				[&value](const auto& lhs) { return lhs * static_cast<T>(value); }
			);
			return *this;
		}

		template <std::convertible_to<T> T2>
		constexpr Vector& operator /=(const T2& value) noexcept
		{
			assert(value != 0 && "division by 0 is undefined.");

			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::end(m_Values),
				[&value](const auto& lhs) { return lhs / static_cast<T>(value); }
			);
			return *this;
		}

		template <std::convertible_to<T> T2>
			requires concepts::modable<T>
		constexpr Vector& operator %=(const T2& value) noexcept
		{
			assert(value != 0 && "division by 0 is undefined.");

			std::transform
			(
				std::execution::unseq,
				std::cbegin(m_Values),
				std::cend(m_Values),
				std::end(m_Values),
				[&value](const auto& lhs) { return lhs % static_cast<T>(value); }
			);
			return *this;
		}

		template <concepts::add_assignable<Vector> T2>
		friend Vector operator +(Vector rhs, const T2& lhs) noexcept
		{
			lhs += rhs;
			return lhs;
		}

		template <concepts::add_assignable<Vector> T2>
			requires std::same_as<T2, Vector>
		friend Vector operator +(const T2& lhs, Vector rhs) noexcept
		{
			rhs += lhs;
			return rhs;
		}

		template <concepts::sub_assignable<Vector> T2>
		friend Vector operator /(Vector lhs, const T2& rhs) noexcept
		{
			lhs -= rhs;
			return lhs;
		}

		template <concepts::mul_assignable<Vector> T2>
		friend Vector operator *(Vector rhs, const T2& lhs) noexcept
		{
			lhs *= rhs;
			return lhs;
		}

		template <concepts::mul_assignable<Vector> T2>
		friend Vector operator *(const T2& lhs, Vector rhs) noexcept
		{
			rhs *= lhs;
			return rhs;
		}

		template <concepts::div_assignable<Vector> T2>
		friend Vector operator /(Vector lhs, const T2& rhs) noexcept
		{
			lhs /= rhs;
			return lhs;
		}

		template <concepts::mod_assignable<Vector> T2>
		friend Vector operator %(Vector lhs, const T2& rhs) noexcept
		{
			lhs %= rhs;
			return lhs;
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

	//template <VectorObject TVector>
	//	requires ConstForwardIteratable<TVector>
	//[[nodiscard]] constexpr typename TVector::ValueType lengthSq(const TVector& Vector) noexcept
	//{
	//	using T = typename TVector::ValueType;
	//	return std::reduce(
	//						std::cbegin(Vector),
	//						std::cend(Vector),
	//						typename TVector::ValueType{},
	//						[](T value, T element) { return value + element * element; }
	//					);
	//}

	//template <VectorObject TVector1, VectorObject TVector2>
	//	requires ConstForwardIteratable<TVector1> && ConstForwardIteratable<TVector2> &&
	//			(TVector1::dimensions == TVector2::dimensions) &&
	//			Multiplicable<typename TVector1::ValueType, typename TVector2::ValueType>
	//[[nodiscard]] constexpr typename TVector1::ValueType scalarProduct(const TVector1& lhs, const TVector2& rhs) noexcept
	//{
	//	return std::transform_reduce(
	//								std::cbegin(lhs),
	//								std::cend(lhs),
	//								std::cbegin(rhs),
	//								typename TVector1::ValueType{},
	//								std::plus<>(),
	//								std::multiplies<>()
	//								);
	//}

	//template <VectorObject TVector>
	//[[nodiscard]] constexpr typename TVector::ValueType length(const TVector& Vector) noexcept
	//{
	//	return static_cast<typename TVector::ValueType>(std::sqrt(lengthSq(Vector)));
	//}

	//template <class T2, VectorObject TVector>
	//[[nodiscard]] constexpr T2 length(const TVector& Vector) noexcept
	//{
	//	return static_cast<T2>(std::sqrt(lengthSq(Vector)));
	//}

	//template <VectorObject TVector>
	//	requires std::floating_point<typename TVector::ValueType>
	//[[nodiscard]] constexpr TVector normalize(TVector Vector) noexcept
	//{
	//	assert(Vector != TVector::zero());

	//	auto length = georithm::length(Vector);
	//	return Vector /= length;
	//}

	//// unfortunately there already exists a namespace called transform
	//template <VectorObject TVector, invocable_r<typename TVector::ValueType, typename TVector::ValueType> TUnaryOp>
	//[[nodiscard]] TVector transmute(
	//	TVector Vector,
	//	TUnaryOp op
	//) noexcept(std::is_nothrow_invocable_v<TUnaryOp, typename TVector::ValueType>)
	//{
	//	std::transform(
	//					std::cbegin(Vector),
	//					std::cend(Vector),
	//					std::begin(Vector),
	//					op
	//				);
	//	return Vector;
	//}

	//template <VectorObject TVector1, VectorObject TVector2, invocable_r<
	//			typename TVector1::ValueType, typename TVector1::ValueType, typename TVector2::ValueType> TBinaryOp>
	//	requires (TVector1::dimensions == TVector2::dimensions)
	//[[nodiscard]] TVector1 transmuteElementWise(
	//	TVector1 lhs,
	//	const TVector2 rhs,
	//	TBinaryOp op
	//)
	//noexcept(std::is_nothrow_invocable_v<TBinaryOp, typename TVector1::ValueType, typename TVector2::ValueType>)
	//{
	//	std::transform(
	//					std::cbegin(lhs),
	//					std::cend(lhs),
	//					std::cbegin(rhs),
	//					std::begin(lhs),
	//					op
	//				);
	//	return lhs;
	//}

	//template <VectorObject TVector>
	//	requires std::floating_point<typename TVector::ValueType> || std::signed_integral<typename TVector::ValueType>
	//[[nodiscard]] TVector abs(TVector Vector)
	//{
	//	using T = typename TVector::ValueType;
	//	return transmute(Vector, [](T element) { return std::abs(element); });
	//}

	//template <VectorObject TVector>
	//[[nodiscard]] constexpr TVector ceil(TVector Vector) noexcept
	//{
	//	using T = typename TVector::ValueType;
	//	return transmute(Vector, [](T element) { return static_cast<T>(std::ceil(element)); });
	//}

	//template <VectorObject TVector>
	//[[nodiscard]] constexpr TVector floor(TVector Vector) noexcept
	//{
	//	using T = typename TVector::ValueType;
	//	return transmute(Vector, [](T element) { return static_cast<T>(std::floor(element)); });
	//}

	//template <VectorObject TVector>
	//[[nodiscard]] constexpr TVector trunc(TVector Vector) noexcept
	//{
	//	using T = typename TVector::ValueType;
	//	return transmute(Vector, [](T element) { return static_cast<T>(std::trunc(element)); });
	//}

	//template <VectorObject TVector>
	//[[nodiscard]] constexpr TVector round(TVector Vector) noexcept
	//{
	//	using T = typename TVector::ValueType;
	//	return transmute(Vector, [](T element) { return static_cast<T>(std::round(element)); });
	//}

	//template <NDimensionalVectorObject<2> TVector>
	//	requires std::floating_point<typename TVector::ValueType>
	//[[nodiscard]] constexpr TVector rotate(const TVector& Vector, typename TVector::ValueType radian) noexcept
	//{
	//	auto sin = std::sin(radian);
	//	auto cos = std::cos(radian);
	//	return { cos * Vector[0] - sin * Vector[1], sin * Vector[0] + cos * Vector[1] };
	//}

	//template <NDimensionalVectorObject<2> TVector>
	//	requires std::signed_integral<typename TVector::ValueType>
	//[[nodiscard]] constexpr TVector rotate(TVector Vector, double radian) noexcept
	//{
	//	const auto rotated = rotate(static_cast<Vector<double, 2>>(Vector), radian);
	//	return static_cast<TVector>(round(rotated));
	//}
}

#endif
