#pragma once
#include "paths.hpp"

namespace cherry::ast {
namespace detail {

/// @brief   A type definition for the function pointer of a visitor object that
///          can visit a simple path node.
/// @details A visitor for a import declaration must be able to visit the simple
///          path of that import, so this function pointer type declaration is
///          defined to be used with a concept which verifies that visitor has
///          the required function.
/// @tparam  Visitor The type of the visitor that is being provided.
template<typename Visitor>
using visitor_visit_simple_path_fn = void(Visitor::*)(const simple_path&) noexcept;

/// @brief  Verifies that the provided visitor supports visitation to imports.
/// @tparam Visitor The type of the visitor that is being provided.
template<typename Visitor>
concept ImportVisitor = requires {
    { &Visitor::visit } -> std::same_as<visitor_visit_simple_path_fn<Visitor>>;
};

} // namespace cherry::ast::detail


/// @brief   Defines a single import statement abstract syntax node.
/// @details Imports are a useful part of a document that indicate what modules
///          should be available to the current document. Imported modules
///          simplify usages of items within that module, but are not strictly
///          required. Full paths may be used in expressions that wish to access
///          an item from a module. Import statements are defined with the `using`
///          keyword followed by a simple path and ending with a semi-colon.
struct import final {
    /// @brief   The children of this node.
    /// @details Import delarations only allow simple paths to be its children.
    ///          This is because we must know what what module the import
    ///          is supposed to bring in.
    simple_path path;

    /// @brief   Accepts a visitor to this import node.
    /// @details Accepting a visitor to this import node means that the visitor
    ///          will also visit the path of this import.
    /// @tparam  Visitor The type of the visitor that is being provided.
    /// @param   visitor The visitor that we're expecting to receive.
    template<detail::ImportVisitor Visitor>
    void
    accept(const Visitor& visitor) noexcept {
        visitor.visit(path);
    }
};


/// @brief   Compares one import node to another import node.
/// @param   lhs The left hand import node to compare.
/// @param   rhs The right hand import node to compare.
/// @returns True if they are the same nodes; false otherwise.
inline bool
operator==(const import& lhs, const import& rhs) noexcept {
    return lhs.path == rhs.path;
}

/// @brief   Compares one import node to another import node.
/// @param   lhs The left hand import node to compare.
/// @param   rhs The right hand import node to compare.
/// @returns True if the imports are *not* the same reference or if they *do not*
///          contain the same paths; false otherwise.
inline bool
operator!=(const import& lhs, const import& rhs) noexcept {
    return !operator==(lhs, rhs);
}

/// @brief   Writes the node to the output stream.
/// @param   oss  The output stream to write the node to.
/// @param   node The node to write to the output stream.
/// @returns The output stream with the node written to it.
inline std::ostream&
operator<<(std::ostream& oss, const import& node) noexcept {
    auto index    = 0;
    auto original = oss.width();
    oss.width(original + 6);
    oss << "import\n";
    oss.width(original + 3);
    oss << "|_ " << node.path << std::endl;
    oss.width(original);
    return oss;
}

} // namespace cherry::ast