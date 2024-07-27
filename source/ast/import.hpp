#pragma once
#include "paths.hpp"

namespace cherry::ast {

/// @brief   Defines a single import statement abstract syntax node.
/// @details Imports are a useful part of a document that indicate what modules
///          should be available to the current document. Imported modules
///          simplify usages of items within that module, but are not strictly
///          required. Full paths may be used in expressions that wish to access
///          an item from a module. Import statements are defined with the `using`
///          keyword followed by a simple path and ending with a semi-colon.
struct import final : detail::node {
    /// @brief   The path to the module that should be imported into the current
    ///          document that is being analyzed.
    simple_path path;
};


/// @brief   Compares one import node to another import node.
/// @param   lhs The left hand import node to compare.
/// @param   rhs The right hand import node to compare.
/// @returns True if the imports are the same reference or if they contain the
///          same paths; false otherwise.
inline static bool
operator==(const import& lhs, const import& rhs) noexcept {
    return &lhs == &rhs || lhs.path == rhs.path;
}

/// @brief   Compares one import node to another import node.
/// @param   lhs The left hand import node to compare.
/// @param   rhs The right hand import node to compare.
/// @returns True if the imports are *not* the same reference or if they *do not*
///          contain the same paths; false otherwise.
inline static bool
operator!=(const import& lhs, const import& rhs) noexcept {
    return !operator==(lhs, rhs);
}

} // namespace cherry::ast