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
	/**
	 * \brief A mathematically vector implementation
	 * \tparam T The value type
	 * \tparam VDimensions Amount of dimensions. Must be implicitly convertible to std::size_t.
	 */
	template <value_type T, auto VDimensions>
		requires cardinality<VDimensions, std::size_t>
	class Vector
	{
	public:
		/**
		 * \brief Dimension amount as constant.
		 */
		constexpr static auto dimensions{ VDimensions };
		/**
		 * \brief Alias for T, but without modifiers.
		 */
		using value_type = std::remove_cvref_t<T>;
		/**
		 * \brief Used storage type.
		 */
		using storage_type = std::array<value_type, dimensions>;

		/**
		 * \brief Default constructor.
		 * \detail Default initializes all elements via uniform initialization, thus all arithmetic types will defaulted to 0.
		 */
		constexpr Vector() noexcept = default;
		/**
		 * \brief Default destructor.
		 */
		constexpr ~Vector() noexcept = default;
		/**
		 * \brief Default copy-constructor.
		 */
		constexpr Vector(const Vector&) noexcept = default;
		/**
		 * \brief Default move-constructor.
		 */
		constexpr Vector(Vector&&) noexcept = default;

		/**
		 * \brief Aggregate initialization like constructor.
		 * \tparam TArgs Variadic types used for initialization.
		 * \param args Arguments from which the storage will be initialized.
		 *
		 * \details This constructor directly initializes the internal storage with the given arguments. The amount of args must be equal to the amount of dimensions. 
		 */
		template <class... TArgs>
			requires (sizeof...(TArgs) == dimensions && (std::convertible_to<TArgs, T> && ...))
		constexpr explicit Vector(TArgs&&...args) noexcept :
			m_Values{ static_cast<T>(args)... }
		{
		}

		/**
		 * \brief Conversion constructor.
		 * \tparam T2 T of other Vector
		 * \param other Vector which will be used for initialization
		 *
		 * \details Casts all elements from other Vector to T and initializes the storage.
		 */
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

		/**
		 * \brief Default copy-assign-operator
		 * \return reference to this
		 */
		constexpr Vector& operator =(const Vector&) noexcept = default;
		/**
		 * \brief Default move-assign-operator
		 * \return reference to this
		 */
		constexpr Vector& operator =(Vector&&) noexcept = default;

		/**
		 * \brief Default equality comparison operator
		 * \return true if both Vectors are treated as equal.
		 *
		 * \details This does automatically enable the operator !=.
		 */
		[[nodiscard]]
		constexpr bool operator ==(const Vector&) const noexcept = default;

		/**
		 * \brief Accesses a specific element
		 * \tparam TIndex Integral type
		 * \param index Index of the element.
		 * \return const reference to the element
		 *
		 * \remarks Using index less than zero or greater-equal than dimensions, is undefined.
		 */
		template <std::integral TIndex>
		[[nodiscard]]
		constexpr const value_type& operator [](TIndex index) const noexcept
		{
			assert(std::in_range<std::size_t>(index) && "index must be in range of type std::size_t");
			return m_Values[static_cast<std::size_t>(index)];
		}

		/**
		 * \brief Accesses a specific element
		 * \tparam TIndex Integral type
		 * \param index Index of the element.
		 * \return const reference to the element
		 *
		 * \remarks Using index less than zero or greater-equal than dimensions, is undefined.
		 */
		template <std::integral TIndex>
		[[nodiscard]]
		constexpr value_type& operator [](TIndex index) noexcept
		{
			assert(std::in_range<std::size_t>(index) && "index must be in range of type std::size_t");
			return m_Values[static_cast<std::size_t>(index)];
		}

		/**
		 * \brief Accesses the first element
		 * \tparam VDimensions2 Do not change!
		 * \return const reference to the first element
		 */
		template <auto VDimensions2 = VDimensions>
			requires (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr const value_type& x() const noexcept { return m_Values[0]; }

		/**
		 * \brief Accesses the first element
		 * \tparam VDimensions2 Do not change!
		 * \return reference to the first element
		 */
		template <auto VDimensions2 = VDimensions>
			requires (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr value_type& x() noexcept { return m_Values[0]; }

		/**
		 * \brief Accesses the second element
		 * \tparam VDimensions2 Do not change!
		 * \return const reference to the second element
		 *
		 * \remarks This functions is only available, if Vector has 1 or more dimension.
		 */
		template <auto VDimensions2 = VDimensions>
			requires (1 < VDimensions) && (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr const value_type& y() const noexcept { return m_Values[1]; }

		/**
		 * \brief Accesses the second element
		 * \tparam VDimensions2 Do not change!
		 * \return reference to the second element
		 *
		 * \remarks This functions is only available, if Vector has 1 or more dimension.
		 */
		template <auto VDimensions2 = VDimensions>
			requires (1 < VDimensions) && (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr value_type& y() noexcept { return m_Values[1]; }

		/**
		 * \brief Accesses the third element
		 * \tparam VDimensions2 Do not change!
		 * \return const reference to the third element
		 *
		 * \remarks This functions is only available, if Vector has 2 or more dimension.
		 */
		template <auto VDimensions2 = VDimensions>
			requires (2 < VDimensions) && (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr const value_type& z() const noexcept { return m_Values[2]; }

		/**
		 * \brief Accesses the third element
		 * \tparam VDimensions2 Do not change!
		 * \return reference to the third element
		 *
		 * \remarks This functions is only available, if Vector has 2 or more dimension.
		 */
		template <auto VDimensions2 = VDimensions>
			requires (2 < VDimensions) && (VDimensions2 == VDimensions)
		[[nodiscard]]
		constexpr value_type& z() noexcept { return m_Values[2]; }

		/**
		 * \brief Adds other Vector element-wise
		 * \tparam T2 T of other Vector
		 * \param other other Vector
		 * \return reference to this
		 */
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

		/**
		 * \brief Subtracts other Vector element-wise
		 * \tparam T2 T of other Vector
		 * \param other other Vector
		 * \return reference to this
		 */
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

		/**
		 * \brief Adds value to each element
		 * \tparam T2 Must be convertible to value_type
		 * \param value Value to be added
		 * \return reference to this
		 */
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

		/**
		 * \brief Subtracts value from each element
		 * \tparam T2 Must be convertible to value_type
		 * \param value Value to be subtracted
		 * \return reference to this
		 */
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

		/**
		 * \brief Multiplies each element
		 * \tparam T2 Must be convertible to value_type
		 * \param value Value each element will be multiplied by
		 * \return reference to this
		 */
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

		/**
		 * \brief Divides each element
		 * \tparam T2 Must be convertible to value_type
		 * \param value Value each element will be divided by
		 * \return reference to this
		 *
		 * \remarks Division by 0 is undefined.
		 */
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

		/**
		 * \brief Applies modulo on each element
		 * \tparam T2 Must be convertible to value_type
		 * \param value Value which will be modulo applied
		 * \return reference to this
		 *
		 * \remarks Division by 0 is undefined.
		 */
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

		/**
		 * \brief Returns an iterator to the first element.
		 * \return An iterator to the first element.
		 */
		[[nodiscard]]
		constexpr auto begin() noexcept { return std::begin(m_Values); }

		/**
		 * \brief Returns an const iterator to the first element.
		 * \return An const iterator to the first element.
		 */
		[[nodiscard]]
		constexpr auto begin() const noexcept { return std::begin(m_Values); }

		/**
		 * \brief Returns an const iterator to the first element.
		 * \return An const iterator to the first element.
		 */
		[[nodiscard]]
		constexpr auto cbegin() const noexcept { return std::cbegin(m_Values); }

		/**
		 * \brief Returns an iterator to the end (i.e. the element after the last element).
		 * \return An iterator to the end of the element range. Note that the end of a range is defined as the element following the last valid element.
		 */
		[[nodiscard]]
		constexpr auto end() noexcept { return std::end(m_Values); }

		/**
		 * \brief Returns an const iterator to the end (i.e. the element after the last element).
		 * \return An iterator to the end of the element range. Note that the end of a range is defined as the element following the last valid element.
		 */
		[[nodiscard]]
		constexpr auto end() const noexcept { return std::end(m_Values); }

		/**
		 * \brief Returns an const iterator to the end (i.e. the element after the last element).
		 * \return An const iterator to the end of the element range. Note that the end of a range is defined as the element following the last valid element.
		 */
		[[nodiscard]]
		constexpr auto cend() const noexcept { return std::cend(m_Values); }

		/**
		 * \brief Returns an iterator to the reverse-beginning.
		 * \return An iterator to the reverse-beginning.
		 */
		[[nodiscard]]
		constexpr auto rbegin() noexcept { return std::rbegin(m_Values); }

		/**
		 * \brief Returns an const iterator to the reverse-beginning.
		 * \return An const iterator to the reverse-beginning.
		 */
		[[nodiscard]]
		constexpr auto rbegin() const noexcept { return std::rbegin(m_Values); }

		/**
		 * \brief Returns an const iterator to the reverse-beginning.
		 * \return An const iterator to the reverse-beginning.
		 */
		[[nodiscard]]
		constexpr auto crbegin() const noexcept { return std::crbegin(m_Values); }

		/**
		 * \brief Returns an iterator to the reverse-end of the given range (i.e. the element before the first element).
		 * \return An iterator to the reverse-end of the given range. Note that the reverse-end of a range is defined as the element previous to the first valid element.
		 */
		[[nodiscard]]
		constexpr auto rend() noexcept { return std::rend(m_Values); }

		/**
		 * \brief Returns an const iterator to the reverse-end of the given range (i.e. the element before the first element).
		 * \return An const iterator to the reverse-end of the given range. Note that the reverse-end of a range is defined as the element previous to the first valid element.
		 */
		[[nodiscard]]
		constexpr auto rend() const noexcept { return std::rend(m_Values); }

		/**
		 * \brief Returns an const iterator to the reverse-end of the given range (i.e. the element before the first element).
		 * \return An const iterator to the reverse-end of the given range. Note that the reverse-end of a range is defined as the element previous to the first valid element.
		 */
		[[nodiscard]]
		constexpr auto crend() const noexcept { return std::crend(m_Values); }

	private:
		storage_type m_Values{};
	};

	/**
	 * \brief Vector deduction guide to make aggregate-like construction easier.
	 * \tparam T Variadic types
	 */
	template <class... T>
	Vector(T&&...) -> Vector<std::common_type_t<T...>, sizeof...(T)>;

	/**
	 * \brief Uniform interface to Vector types.
	 * \tparam T type of interest
	 */
	template <class T>
	struct vector_traits
	{
	};

	/**
	 * \brief Specialization for Vector
	 * \tparam T value_type of Vector
	 * \tparam VDimensions dimensions of Vector
	 */
	template <value_type T, auto VDimensions>
	struct vector_traits<Vector<T, VDimensions>>
	{
		using value_type = typename Vector<T, VDimensions>::value_type;

		static constexpr auto dimensions = Vector<T, VDimensions>::dimensions;
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
	 * \brief Specialization for Vector which registers Vector with all its template variations as vectorial.
	 * \tparam T value_type of Vector
	 * \tparam VDimensions dimensions of Vector
	 */
	template <value_type T, auto VDimensions>
	struct is_vectorial<Vector<T, VDimensions>> : std::true_type
	{
	};

	/**
	 * \brief Shortcut checking for vectorial classes
	 * \tparam T A type to check
	 */
	template <class T>
	constexpr bool is_vectorial_v = is_vectorial<T>::value;

	/**
	 * \brief Concept checking for vectorial types
	 * \tparam T A type to check
	 */
	template <class T>
	concept vectorial = is_vectorial_v<std::remove_cvref_t<T>>;

	/**
	 * \brief Sum operator
	 * \tparam TVector A vectorial type
	 * \tparam T2 A type, which is add-assignable to TVector
	 * \param lhs left-hand-side of sum
	 * \param rhs right-hand-side of sum
	 * \return newly constructed Vector
	 */
	template <vectorial TVector, add_assignable<TVector> T2>
	constexpr TVector operator +(TVector lhs, T2&& rhs) noexcept
	{
		lhs += std::forward<T2>(rhs);
		return lhs;
	}

	/**
	 * \brief Minus operator
	 * \tparam TVector A vectorial type
	 * \tparam T2 A type, which is subtract-assignable to TVector
	 * \param lhs left-hand-side of subtraction
	 * \param rhs right-hand-side of subtraction
	 * \return newly constructed Vector
	 */
	template <vectorial TVector, sub_assignable<TVector> T2>
	constexpr TVector operator -(TVector lhs, T2&& rhs) noexcept
	{
		lhs -= std::forward<T2>(rhs);
		return lhs;
	}

	/**
	 * \brief Multiplication operator
	 * \tparam TVector A vectorial type
	 * \tparam T2 A type, which is multiplication-assignable to TVector
	 * \param lhs left-hand-side of multiplication
	 * \param rhs right-hand-side of multiplication
	 * \return newly constructed Vector
	 */
	template <vectorial TVector, mul_assignable<TVector> T2>
	constexpr TVector operator *(TVector lhs, T2&& rhs) noexcept
	{
		lhs *= std::forward<T2>(rhs);
		return lhs;
	}

	/**
	 * \brief Commutative multiplication operator
	 * \tparam TVector A vectorial type
	 * \tparam T2 A type, which is multiplication-assignable to TVector
	 * \param lhs left-hand-side of multiplication
	 * \param rhs right-hand-side of multiplication
	 * \return newly constructed Vector
	 */
	template <vectorial TVector, mul_assignable<TVector> T2>
	constexpr TVector operator *(T2&& lhs, TVector rhs) noexcept
	{
		rhs *= std::forward<T2>(lhs);
		return rhs;
	}

	/**
	 * \brief Division operator
	 * \tparam TVector A vectorial type
	 * \tparam T2 A type, which is division-assignable to TVector
	 * \param lhs left-hand-side of division
	 * \param rhs right-hand-side of division
	 * \return newly constructed Vector
	 */
	template <vectorial TVector, div_assignable<TVector> T2>
	constexpr TVector operator /(TVector lhs, T2&& rhs) noexcept
	{
		lhs /= std::forward<T2>(rhs);
		return lhs;
	}

	/**
	 * \brief Modulo operator
	 * \tparam TVector A vectorial type
	 * \tparam T2 A type, which is modulo-assignable to TVector
	 * \param lhs left-hand-side of modulo
	 * \param rhs right-hand-side of modulo
	 * \return newly constructed Vector
	 */
	template <vectorial TVector, mod_assignable<TVector> T2>
	constexpr TVector operator %(TVector lhs, T2&& rhs) noexcept
	{
		lhs %= std::forward<T2>(rhs);
		return lhs;
	}

	/**
	 * \brief Calculates the squared length of a Vector
	 * \tparam TVector A vectorial type
	 * \param vector Vector to be calculated from
	 * \return scalar value
	 */
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

	/**
	 * \brief Calculates the length of a Vector
	 * \tparam TVector A vectorial type
	 * \param vector Vector to be calculated from
	 * \return scalar value
	 *
	 * \remarks Due to std::sqrt, this function will always return a floating point type.
	 */
	template <vectorial TVector>
	[[nodiscard]]
	constexpr auto length(const TVector& vector) noexcept { return std::sqrt(length_sq(vector)); }

	/**
	 * \brief Calculates the dot product of to Vectors
	 * \tparam TVector1 A vectorial type
	 * \tparam TVector2 Another vectorial type
	 * \param lhs left-hand-side of calculation
	 * \param rhs left-hand-side of calculation
	 * \return scalar value
	 */
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

	/**
	 * \brief Calculates the normalization of a Vector
	 * \tparam TVector A vectorial type
	 * \param vec Vector to be calculated from
	 * \return newly constructed Vector
	 */
	template <vectorial TVector>
		requires std::floating_point<vector_value_t<TVector>>
	[[nodiscard]]
	constexpr TVector normalized(TVector vec) noexcept
	{
		assert(vec != TVector{});

		return vec /= length(vec);
	}
}

#endif
