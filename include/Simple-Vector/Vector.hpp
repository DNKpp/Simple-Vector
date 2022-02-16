//          Copyright Dominic Koepke 2021 - 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SIMPLE_VECTOR_VECTOR_HPP
#define SIMPLE_VECTOR_VECTOR_HPP

#pragma once

#include "Algorithm.hpp"
#include "Concepts.hpp"
#include "Functional.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <execution>
#include <ranges>

namespace sl::vec
{
	/** \addtogroup Vector
	 * @{
	 */

	/**
	 * \brief A mathematically vector implementation
	 * \tparam T The value type
	 * \tparam VDimensions Amount of dimensions. Must be implicitly convertible to std::size_t.
	 */
	template <value_type T, std::size_t VDimensions>
		requires (0 < VDimensions)
	class Vector
	{
	public:
		/**
		 * \brief Dimension amount as constant.
		 */
		constexpr static std::size_t dimensions{ VDimensions };
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
		 * \details Default initializes all elements via uniform initialization, thus all arithmetic types will defaulted to 0.
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
		explicit (VDimensions <= 1)
		constexpr Vector(TArgs&&...args) noexcept :
			m_Values{ static_cast<T>(args)... }
		{
		}

		/**
		 * \brief Conversion constructor.
		 * \tparam T2 T of other Vector
		 * \tparam VOtherDimensions dimensions of other Vector
		 * \param other Vector which will be used for initialization
		 *
		 * \details Casts all elements from other Vector to value_type and initializes the storage.
		 * If the target Vector has less dimensions than source all values beyond will be ignored.
		 * If the source Vector has less dimensions than the  target, missing elements will be default initialized.
		 */
		template <std::convertible_to<value_type> T2, auto VOtherDimensions>
			requires (!std::same_as<T2, value_type> || dimensions != VOtherDimensions)
		constexpr explicit Vector(const Vector<T2, VOtherDimensions>& other)
		{
			transform_unseq
			(
				std::ranges::take_view{ other, std::min(dimensions, VOtherDimensions) },
				std::ranges::begin(m_Values),
				fn::cast_invoke_result<value_type>(std::identity{})
			);
		}

		/**
		 * \brief Initializes each element with invocation results of the given generator.
		 * \tparam TGenerator Type of generator
		 * \param generator used generator
		 *
		 * \details This constructor can be used to hand over invokable an object with the following signature:
		 * \code{.cpp}
		 * value_type()
		 * \endcode
		 * The library already offers some \ref Generators "generators".
		 */
		template <std::invocable TGenerator>
			requires std::convertible_to<std::invoke_result_t<TGenerator&>, value_type>
		constexpr explicit Vector(TGenerator generator)
		{
			std::ranges::generate(m_Values, std::ref(generator));
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
		 * \param index Index of the element.
		 * \return const reference to the element
		 *
		 * \remarks Using index less than zero or greater-equal than dimensions, is undefined.
		 */
		[[nodiscard]]
		constexpr const value_type& operator [](std::integral auto index) const noexcept
		{
			assert(std::in_range<std::size_t>(index) && "index must be in range of type std::size_t");
			return m_Values[static_cast<std::size_t>(index)];
		}

		/**
		 * \brief Accesses a specific element
		 * \param index Index of the element.
		 * \return const reference to the element
		 *
		 * \remarks Using index less than zero or greater-equal than dimensions, is undefined.
		 */
		template <std::integral TIndex>
		[[nodiscard]]
		constexpr value_type& operator [](std::integral auto index) noexcept
		{
			assert(std::in_range<std::size_t>(index) && "index must be in range of type std::size_t");
			return m_Values[static_cast<std::size_t>(index)];
		}

		/**
		 * \brief Accesses the first element
		 * \return const reference to the first element
		 */
		[[nodiscard]]
		constexpr const value_type& x() const noexcept
		{
			return m_Values[0];
		}

		/**
		 * \brief Accesses the first element
		 * \return reference to the first element
		 */
		[[nodiscard]]
		constexpr value_type& x() noexcept
		{
			return m_Values[0];
		}

		/**
		 * \brief Accesses the second element
		 * \return const reference to the second element
		 *
		 * \remarks This functions is only available, if Vector has 2 or more dimension.
		 */
		[[nodiscard]]
		constexpr const value_type& y() const noexcept
			requires (1 < VDimensions)
		{
			return m_Values[1];
		}

		/**
		 * \brief Accesses the second element
		 * \return reference to the second element
		 *
		 * \remarks This functions is only available, if Vector has 2 or more dimension.
		 */
		[[nodiscard]]
		constexpr value_type& y() noexcept
			requires (1 < VDimensions)
		{
			return m_Values[1];
		}

		/**
		 * \brief Accesses the third element
		 * \return const reference to the third element
		 *
		 * \remarks This functions is only available, if Vector has 3 or more dimension.
		 */
		[[nodiscard]]
		constexpr const value_type& z() const noexcept
			requires (2 < VDimensions)
		{
			return m_Values[2];
		}

		/**
		 * \brief Accesses the third element
		 * \return reference to the third element
		 *
		 * \remarks This functions is only available, if Vector has 3 or more dimension.
		 */
		[[nodiscard]]
		constexpr value_type& z() noexcept
			requires (2 < VDimensions)
		{
			return m_Values[2];
		}

		/**
		 * \brief Adds other Vector element-wise
		 * \tparam T2 T of other Vector
		 * \param other other Vector
		 * \return reference to this
		 */
		template <std::convertible_to<value_type> T2>
		constexpr Vector& operator +=(const Vector<T2, dimensions>& other)
		{
			transform_unseq
			(
				m_Values,
				other,
				std::ranges::begin(m_Values),
				fn::cast_invoke_result<value_type>(std::plus{})
			);
			return *this;
		}

		/**
		 * \brief Subtracts other Vector element-wise
		 * \tparam T2 T of other Vector
		 * \param other other Vector
		 * \return reference to this
		 */
		template <std::convertible_to<value_type> T2>
		constexpr Vector& operator -=(const Vector<T2, dimensions>& other)
		{
			transform_unseq
			(
				m_Values,
				other,
				std::ranges::begin(m_Values),
				fn::cast_invoke_result<value_type>(std::minus{})
			);
			return *this;
		}

		/**
		 * \brief Adds value to each element
		 * \tparam T2 Must be convertible to value_type
		 * \param value Value to be added
		 * \return reference to this
		 */
		template <std::convertible_to<value_type> T2>
		constexpr Vector& operator +=(const T2& value)
		{
			transform_unseq
			(
				m_Values,
				std::ranges::begin(m_Values),
				fn::cast_invoke_result<value_type>([&value](const auto& lhs) { return lhs + value; })
			);
			return *this;
		}

		/**
		 * \brief Subtracts value from each element
		 * \tparam T2 Must be convertible to value_type
		 * \param value Value to be subtracted
		 * \return reference to this
		 */
		template <std::convertible_to<value_type> T2>
		constexpr Vector& operator -=(const T2& value)
		{
			transform_unseq
			(
				m_Values,
				std::ranges::begin(m_Values),
				fn::cast_invoke_result<value_type>([&value](const auto& lhs) { return lhs - value; })
			);
			return *this;
		}

		/**
		 * \brief Multiplies each element
		 * \tparam T2 Must be convertible to value_type
		 * \param value Value each element will be multiplied by
		 * \return reference to this
		 */
		template <std::convertible_to<value_type> T2>
		constexpr Vector& operator *=(const T2& value)
		{
			transform_unseq
			(
				m_Values,
				std::ranges::begin(m_Values),
				fn::cast_invoke_result<value_type>([&value](const auto& lhs) { return lhs * value; })
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
		template <std::convertible_to<value_type> T2>
		constexpr Vector& operator /=(const T2& value)
		{
			assert(value != 0 && "division by 0 is undefined.");

			transform_unseq
			(
				m_Values,
				std::ranges::begin(m_Values),
				fn::cast_invoke_result<value_type>([&value](const auto& lhs) { return lhs / value; })
			);
			return *this;
		}

		/**
		 * \brief Applies modulo on each element
		 * \tparam T2 Must be convertible to value_type
		 * \param rawValue Value which will be modulo applied
		 * \return reference to this
		 *
		 * \remarks Division by 0 is undefined.
		 */
		template <std::convertible_to<value_type> T2>
			requires modable<T>
		constexpr Vector& operator %=(const T2& rawValue)
		{
			const auto value = static_cast<value_type>(rawValue);
			assert(value != 0 && "division by 0 is undefined.");

			transform_unseq
			(
				m_Values,
				std::ranges::begin(m_Values),
				[value](const auto& lhs) { return lhs % value; }
			);
			return *this;
		}

		/**
		 * \brief Returns an iterator to the first element.
		 * \return An iterator to the first element.
		 */
		[[nodiscard]]
		constexpr auto begin() noexcept { return std::ranges::begin(m_Values); }

		/**
		 * \brief Returns an const iterator to the first element.
		 * \return An const iterator to the first element.
		 */
		[[nodiscard]]
		constexpr auto begin() const noexcept { return std::ranges::begin(m_Values); }

		/**
		 * \brief Returns an const iterator to the first element.
		 * \return An const iterator to the first element.
		 */
		[[nodiscard]]
		constexpr auto cbegin() const noexcept { return std::ranges::cbegin(m_Values); }

		/**
		 * \brief Returns an iterator to the end (i.e. the element after the last element).
		 * \return An iterator to the end of the element range. Note that the end of a range is defined as the element following the last valid element.
		 */
		[[nodiscard]]
		constexpr auto end() noexcept { return std::ranges::end(m_Values); }

		/**
		 * \brief Returns an const iterator to the end (i.e. the element after the last element).
		 * \return An iterator to the end of the element range. Note that the end of a range is defined as the element following the last valid element.
		 */
		[[nodiscard]]
		constexpr auto end() const noexcept { return std::ranges::end(m_Values); }

		/**
		 * \brief Returns an const iterator to the end (i.e. the element after the last element).
		 * \return An const iterator to the end of the element range. Note that the end of a range is defined as the element following the last valid element.
		 */
		[[nodiscard]]
		constexpr auto cend() const noexcept { return std::ranges::cend(m_Values); }

		/**
		 * \brief Returns an iterator to the reverse-beginning.
		 * \return An iterator to the reverse-beginning.
		 */
		[[nodiscard]]
		constexpr auto rbegin() noexcept { return std::ranges::rbegin(m_Values); }

		/**
		 * \brief Returns an const iterator to the reverse-beginning.
		 * \return An const iterator to the reverse-beginning.
		 */
		[[nodiscard]]
		constexpr auto rbegin() const noexcept { return std::ranges::rbegin(m_Values); }

		/**
		 * \brief Returns an const iterator to the reverse-beginning.
		 * \return An const iterator to the reverse-beginning.
		 */
		[[nodiscard]]
		constexpr auto crbegin() const noexcept { return std::ranges::crbegin(m_Values); }

		/**
		 * \brief Returns an iterator to the reverse-end of the given range (i.e. the element before the first element).
		 * \return An iterator to the reverse-end of the given range. Note that the reverse-end of a range is defined as the element previous to the first valid element.
		 */
		[[nodiscard]]
		constexpr auto rend() noexcept { return std::ranges::rend(m_Values); }

		/**
		 * \brief Returns an const iterator to the reverse-end of the given range (i.e. the element before the first element).
		 * \return An const iterator to the reverse-end of the given range. Note that the reverse-end of a range is defined as the element previous to the first valid element.
		 */
		[[nodiscard]]
		constexpr auto rend() const noexcept { return std::ranges::rend(m_Values); }

		/**
		 * \brief Returns an const iterator to the reverse-end of the given range (i.e. the element before the first element).
		 * \return An const iterator to the reverse-end of the given range. Note that the reverse-end of a range is defined as the element previous to the first valid element.
		 */
		[[nodiscard]]
		constexpr auto crend() const noexcept { return std::ranges::crend(m_Values); }

	private:
		storage_type m_Values{};
	};

	/**
	 * \brief Vector deduction guide to make aggregate-like construction easier.
	 * \tparam T Variadic types
	 */
	template <class... T>
	Vector(T&&...) -> Vector<std::common_type_t<T...>, sizeof...(T)>;

	/** @}*/

	/** \addtogroup TypeTraits
	 * @{
	 */

	/**
	 * \brief Specialization for Vector
	 * \tparam T value_type of Vector
	 * \tparam VDimensions dimensions of Vector
	 */
	template <value_type T, std::size_t VDimensions>
	struct vector_traits<Vector<T, VDimensions>>
	{
		using value_type = typename Vector<T, VDimensions>::value_type;

		static constexpr std::size_t dimensions = Vector<T, VDimensions>::dimensions;
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

	/** @}*/

	/** \addtogroup Vector
	 * @{
	 */

	/**
	 * \brief Sum operator
	 * \tparam TVector A vectorial type
	 * \tparam T2 A type, which is add-assignable to TVector
	 * \param lhs left-hand-side of sum
	 * \param rhs right-hand-side of sum
	 * \return newly constructed Vector
	 */
	template <vectorial TVector, add_assignable<TVector> T2>
	[[nodiscard]]
	constexpr TVector operator +(TVector lhs, T2&& rhs)
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
	[[nodiscard]]
	constexpr TVector operator -(TVector lhs, T2&& rhs)
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
	[[nodiscard]]
	constexpr TVector operator *(TVector lhs, T2&& rhs)
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
	[[nodiscard]]
	constexpr TVector operator *(T2&& lhs, TVector rhs)
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
	[[nodiscard]]
	constexpr TVector operator /(TVector lhs, T2&& rhs)
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
	[[nodiscard]]
	constexpr TVector operator %(TVector lhs, T2&& rhs)
	{
		lhs %= std::forward<T2>(rhs);
		return lhs;
	}

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
		return transform_reduce_unseq
		(
			lhs,
			rhs,
			T{},
			std::plus{},
			fn::cast_invoke_result<T>(std::multiplies{})
		);
	}

	/**
	 * \brief Calculates the squared length of a Vector
	 * \tparam TVector A vectorial type
	 * \param vector Vector to be calculated from
	 * \return scalar value
	 */
	template <vectorial TVector>
	[[nodiscard]]
	constexpr vector_value_t<TVector> length_squared(const TVector& vector)
	{
		return dot_product(vector, vector);
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
	constexpr auto length(const TVector& vector) { return std::sqrt(length_squared(vector)); }

	/**
	 * \brief Calculates the normalization of a Vector
	 * \tparam TVector A vectorial type
	 * \param vec Vector to be calculated from
	 * \return newly constructed Vector
	 */
	template <vectorial TVector>
		requires std::floating_point<vector_value_t<TVector>>
	[[nodiscard]]
	constexpr TVector normalized(TVector vec)
	{
		assert(vec != TVector{});

		return vec /= length(vec);
	}

	/**
	 * \brief Projects vector onto the target Vector.
	 * \tparam TVector Type of Vector
	 * \param vector The Vector to be projected
	 * \param target The Vector to be projected onto
	 * \return newly constructed Vector
	 */
	template <vectorial TVector>
	[[nodiscard]]
	constexpr TVector projected(const TVector& vector, TVector target)
	{
		assert(vector != TVector{} && "vector must not be the null vector.");
		assert(target != TVector{} && "target must not be the null vector.");

		const auto dot = dot_product(vector, target);
		const auto targetLengthSq = length_squared(target);
		target *= (dot / targetLengthSq);
		return target;
	}

	/**
	 * \brief Computes the linear interpolation between both vectors for the parameter t (or extrapolation, when t is outside the range [0,1]).
	 * \tparam TVector Type of Vector
	 * \tparam T Type of interpolation distance
	 * \param vector1 The first Vector
	 * \param vector2 The second Vector
	 * \param t The interpolation distance (usually between [0, 1])
	 * \return newly constructed Vector
	 *
	 * \remark Vector with integral value_type may work as expected if difference between both vectors is very small.
	 */
	template <vectorial TVector, class T>
		requires std::is_arithmetic_v<T>
	[[nodiscard]]
	constexpr TVector lerp(TVector vector1, const TVector& vector2, T t)
	{
		transform_unseq
		(
			vector1,
			vector2,
			std::ranges::begin(vector1),
			fn::cast_invoke_result<vector_value_t<TVector>>([t](auto a, auto b) { return std::lerp(a, b, t); })
		);
		return vector1;
	}

	/**
	 * \brief Computes the inverse of the vector (1./v[0], 1./v[1], ...).
	 * \tparam TVector Type of Vector
	 * \param vector The Vector to be inversed
	 * \return newly constructed Vector
	 */
	template <vectorial TVector>
		requires std::floating_point<vector_value_t<TVector>>
	[[nodiscard]]
	constexpr TVector inversed(TVector vector)
	{
		transform_unseq
		(
			vector,
			std::ranges::begin(vector),
			[](auto value) { return 1. / value; }
		);
		return vector;
	}

	/** @}*/
}

#endif
