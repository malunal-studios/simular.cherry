#pragma once
#include "type.hpp"
#include "../types.hpp"


namespace cherry::ast {
namespace detail {

/// @brief   A type definition for the function pointer of a visitor object that
///          can visit an type node.
/// @details A visitor for an alias must be able to visit the type of that alias,
///          so this function pointer type declaration is defined to be used with
///          a concept which verifies that visitor has the required function.
/// @tparam  Visitor The type of the visitor that is being provided.
template<typename Visitor>
using visitor_visit_type_fn = void(Visitor::*)(const type&) noexcept;

/// @brief  Verifies that the provided visitor supports visitation to aliases.
/// @tparam Visitor The type of the visitor that is being provided.
template<typename Visitor>
concept AliasVisitor = requires {
    { &Visitor::visit } -> std::same_as<visitor_visit_type_fn<Visitor>>;
};

} // namespace cherry::ast::detail


/// @brief   Alias abstract syntax tree node.
/// @details An alias is the renaming of an existing type, for the purposes of
///          convenience and clarity.
struct alias final {
    /// @brief   The new identifier for the type being aliased.
    /// @details This will be provided directly from a token that was extracted
    ///          during lexical analysis and syntactic analysis.
    strview_t name;

    /// @brief   The type that is being aliased.
    /// @details ...
    type aliased;

    /// @brief   Accepts a visitor to this alias node.
    /// @details Accepting a visitor to this alias node means that the visitor
    ///          will also visit the type that this node is aliasing.
    /// @tparam  Visitor The type of the visitor that is being provided.
    /// @param   visitor The visitor that we're expecting to receive.
    template<detail::AliasVisitor Visitor>
    void
    accept(const Visitor& visitor) noexcept {
        visitor.visit(aliased);
    }
};


/// @brief   Compares one alias to another alias.
/// @param   lhs The left hand alias to compare.
/// @param   rhs The righ hand alias to compare.
/// @returns True if both alias have the same name and type; otherwise false.
inline bool
operator==(const alias& lhs, const alias& rhs) noexcept {
    return lhs.name    == rhs.name &&
           lhs.aliased == rhs.aliased;
}

/// @brief   Compares one alias to another alias.
/// @param   lhs The left hand alias to compare.
/// @param   rhs The righ hand alias to compare.
/// @returns True if both alias *do not* have the same name and type; otherwise
///          false.
inline bool
operator!=(const alias& lhs, const alias& rhs) noexcept {
    return !operator==(lhs, rhs);
}

} // namespace cherry::ast