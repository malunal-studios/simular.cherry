#pragma once
#include "../types.hpp"


namespace cherry::ast {

/// @brief   Defines a simple path consisting of identifiers separated by the
///          access operator.
/// @details This is an abstract syntax tree node which provides the individual
///          segments of the path for semantic analysis.
struct simple_path : detail::node {
    /// @brief A list of the segments that were parsed from the source code
    ///        input.
    vec_t<strview_t> segments;
};


/// @brief   Compares one simple path to another.
/// @param   lhs The left hand path to compare.
/// @param   rhs The right hand path to compare.
/// @returns True if the paths are the same reference or if they have the exact
///          same path segments; false otherwise.
inline static bool
operator==(const simple_path& lhs, const simple_path& rhs) noexcept {
    return &lhs == &rhs || lhs.segments == rhs.segments;
}

/// @brief   Compares one simple path to another.
/// @param   lhs The left hand path to compare.
/// @param   rhs The right hand path to compare.
/// @returns True if the paths are *not* the same reference or if they *do not*
///          have the exact same path segments; false otherwise.
inline static bool
operator!=(const simple_path& lhs, const simple_path& rhs) noexcept {
    return !operator==(lhs, rhs);
}

} // namespace cherry::ast