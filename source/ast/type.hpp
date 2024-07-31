#pragma once
#include "paths.hpp"


namespace cherry::ast {

/// @brief   Type abstract syntax tree node.
/// @details A type is any path that leads to a function, or a pointer to-,
///          reference of-, or array of- an object type or primitive type.
struct type final {
    // Forwarded.
    struct fn;
    struct arr;

    // /// @brief   Defines a type definition for the variants of this type
    // ///          abstract syntax node.
    // /// @details The variants are stored in this manner instead of a polymorphic
    // ///          manner, in an attempt to save just a bit of memory and align
    // ///          more with how the grammar is defined.
    // using type_variants = std::variant<fn, arr>;

    // /// @brief Contains the variant of this type that was parsed from the
    // ///        source code input.
    // type_variants variant;

    /// @brief A list of the segments that were parsed from the source code
    ///        input that make up the path to this type.
    vec_t<segment> segments;
};

/// @brief   A type variant for functions.
/// @details Stores the input types for the function and the function's
///          return type.
struct type::fn final {
    /// @brief A list of types that the function expects.
    vec_t<type> inputs;

    /// @brief The return type the function produces.
    opt_t<type> output;
};

/// @brief   A type variant for arrays.
/// @details ...
struct type::arr final {
    // TODO: Store list of expressions for the dimensions of the array.
};


// Forwarded.
bool operator==(const type& lhs, const type& rhs) noexcept;
bool operator!=(const type& lhs, const type& rhs) noexcept;


/// @brief   Compares one function type variant to another.
/// @param   lhs The left hand function type variant to compare.
/// @param   rhs The right hand function type variant to compare.
/// @returns True if both function type variants have the same inputs and
///          output; otherwise false.
inline bool
operator==(
    const type::fn& lhs,
    const type::fn& rhs
) noexcept {
    return lhs.inputs == lhs.inputs &&
           *lhs.output == *rhs.output;
}

/// @brief   Compares one function type variant to another.
/// @param   lhs The left hand function type variant to compare.
/// @param   rhs The right hand function type variant to compare.
/// @returns True if both function type variants *do not* have the same inputs
///          and output; otherwise false.
inline bool
operator!=(
    const type::fn& lhs,
    const type::fn& rhs
) noexcept {
    return !operator==(lhs, rhs);
}


/// @brief   Compares one array type variant to another.
/// @param   lhs The left hand array type variant to compare.
/// @param   rhs The right hand array type variant to compare.
/// @returns True if both array type variants have the same inputs; otherwise
///          false.
inline bool
operator==(
    const type::arr& lhs,
    const type::arr& rhs
) noexcept {
    return true; // Not yet defined.
}

/// @brief   Compares one array type variant to another.
/// @param   lhs The left hand array type variant to compare.
/// @param   rhs The right hand array type variant to compare.
/// @returns True if both array type variants *do not* have the same inputs;
///          otherwise false.
inline bool
operator!=(
    const type::arr& lhs,
    const type::arr& rhs
) noexcept {
    return false; // Not yet defined.
}


/// @brief   Compares one type to another type.
/// @param   lhs The left hand type to compare.
/// @param   rhs The right hand type to compare.
/// @returns True if both types have the same variant and segments; otherwise
///          false.
inline bool
operator==(const type& lhs, const type& rhs) noexcept {
    return // lhs.variant  == rhs.variant &&
           lhs.segments == rhs.segments;
}

/// @brief   Compares one type to another type.
/// @param   lhs The left hand type to compare.
/// @param   rhs The right hand type to compare.
/// @returns True if both types *do not* have the same variant and segments;
///          otherwise false.
inline bool
operator!=(const type& lhs, const type& rhs) noexcept {
    return !operator==(lhs, rhs);
}

} // namespace cherry::ast