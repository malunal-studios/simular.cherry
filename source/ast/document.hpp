#pragma once
#include "import.hpp"
#include "module.hpp"


namespace cherry::ast {
namespace detail {

/// @brief   A type definition for the function pointer of a visitor object that
///          can visit an import node.
/// @details A visitor for a document must be able to visit an import of that
///          document, so this function pointer type declaration is defined to
///          be used with a concept which verifies that visitor has the required
///          function.
/// @tparam  Visitor The type of the visitor that is being provided.
template<typename Visitor>
using visitor_visit_import_fn = void(Visitor::*)(const import&) noexcept;

/// @brief   A type definition for the function pointer of a visitor object that
///          can visit a module node.
/// @details A visitor for a document must be able to visit a module of that
///          document, so this function pointer type declaration is defined to
///          be used with a concept which verifies that visitor has the required
///          function.
/// @tparam  Visitor The type of the visitor that is being provided.
template<typename Visitor>
using visitor_visit_module_fn = void(Visitor::*)(const module&) noexcept;

/// @brief  Verifies that the provided visitor supports visitation to documents.
/// @tparam Visitor The type of the visitor that is being provided.
template<typename Visitor>
concept DocumentVisitor = requires {
    { &Visitor::visit } -> std::same_as<visitor_visit_import_fn<Visitor>>;
    { &Visitor::visit } -> std::same_as<visitor_visit_module_fn<Visitor>>;
};

} // namespace cherry::ast::detail


/// @brief   Document abstract syntax tree node.
/// @details A document contains a set of modules that should be imported into
///          the document, and a list of modules that are defined in the document
///          itself.
struct document final {
    /// @brief   The imports for this document.
    /// @details Each import defines what modules should be included into this
    ///          document. Those module do not physical get included, rather,
    ///          they are semantically included. This allows for the items of
    ///          that module to be used directly.
    vec_t<import> imports;

    /// @brief   The modules of this document.
    /// @details Each module contains items that only belong to that module. All
    ///          of the items in that module are namespaced to the that module
    ///          based on the name provided in the package declaration.
    vec_t<module> modules;

    /// @brief   Accepts a visitor to this document node.
    /// @details Accepting a visitor to this document node means that the
    ///          visitor will also visit each of the modules of this document.
    /// @tparam  Visitor The type of the visitor that is being provided.
    /// @param   visitor The visitor that we're expecting to receive.
    template<detail::DocumentVisitor Visitor>
    void
    accept(const Visitor& visitor) noexcept {
        using namespace std::ranges;
        using namespace std::placeholders;
        auto fn = std::mem_fn(&Visitor::visit);
        for_each(imports, std::bind(fn, &visitor, _1));
        for_each(modules, std::bind(fn, &visitor, _1));
    }
};


/// @brief   Compares one document to another document.
/// @param   lhs The left hand document to compare.
/// @param   rhs The right hand document to compare.
/// @returns True if the two documents have the same imports and modules; false
///          otherwise.
inline bool
operator==(const document& lhs, const document& rhs) noexcept {
    return lhs.imports == rhs.imports &&
           lhs.modules == rhs.modules;
}

/// @brief   Compares one document to another document.
/// @param   lhs The left hand document to compare.
/// @param   rhs The right hand document to compare.
/// @returns True if the two documents *do not* have the same imports and
///          modules; otherwise false.
inline bool
operator!=(const document& lhs, const document& rhs) noexcept {
    return !operator==(lhs, rhs);
}

} // namespace cherry::ast