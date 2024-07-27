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

} // namespace cherry::ast