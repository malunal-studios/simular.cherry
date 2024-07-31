#pragma once
#include "alias.hpp"
#include "enumeration.hpp"
#include "extension.hpp"
#include "function.hpp"
#include "object.hpp"
#include "variable.hpp"


namespace cherry::ast {
namespace detail {

/// @brief   A type definition for the function pointer of a visitor object that
///          can visit an alias node.
/// @details A visitor for a module must be able to visit an alias item of that
///          module, so this function pointer type declaration is defined to
///          be used with a concept which verifies that visitor has the required
///          function.
/// @tparam  Visitor The type of the visitor that is being provided.
template<typename Visitor>
using visitor_visit_alias_fn = void(Visitor::*)(const alias&) noexcept;

/// @brief   A type definition for the function pointer of a visitor object that
///          can visit an enumeration node.
/// @details A visitor for a module must be able to visit an enum item of that
///          module, so this function pointer type declaration is defined to
///          be used with a concept which verifies that visitor has the required
///          function.
/// @tparam  Visitor The type of the visitor that is being provided.
template<typename Visitor>
using visitor_visit_enum_fn = void(Visitor::*)(const enumeration&) noexcept;

/// @brief   A type definition for the function pointer of a visitor object that
///          can visit an extension node.
/// @details A visitor for a module must be able to visit an extension item of
///          that module, so this function pointer type declaration is defined to
///          be used with a concept which verifies that visitor has the required
///          function.
/// @tparam  Visitor The type of the visitor that is being provided.
template<typename Visitor>
using visitor_visit_ext_fn = void(Visitor::*)(const extension&) noexcept;

/// @brief   A type definition for the function pointer of a visitor object that
///          can visit an function node.
/// @details A visitor for a module must be able to visit an function item of 
///          that module, so this function pointer type declaration is defined to
///          be used with a concept which verifies that visitor has the required
///          function.
/// @tparam  Visitor The type of the visitor that is being provided.
template<typename Visitor>
using visitor_visit_func_fn = void(Visitor::*)(const function&) noexcept;

/// @brief   A type definition for the function pointer of a visitor object that
///          can visit an object node.
/// @details A visitor for a module must be able to visit an object item of that
///          module, so this function pointer type declaration is defined to
///          be used with a concept which verifies that visitor has the required
///          function.
/// @tparam  Visitor The type of the visitor that is being provided.
template<typename Visitor>
using visitor_visit_object_fn = void(Visitor::*)(const object&) noexcept;

/// @brief   A type definition for the function pointer of a visitor object that
///          can visit an variable node.
/// @details A visitor for a module must be able to visit an var item of that
///          module, so this function pointer type declaration is defined to
///          be used with a concept which verifies that visitor has the required
///          function.
/// @tparam  Visitor The type of the visitor that is being provided.
template<typename Visitor>
using visitor_visit_var_fn = void(Visitor::*)(const variable&) noexcept;

/// @brief  Verifies that the provided visitor supports visitation to modules.
/// @tparam Visitor The type of the visitor that is being provided.
template<typename Visitor>
concept ModuleVisitor = requires {
    { &Visitor::visit } -> std::same_as<visitor_visit_alias_fn<Visitor>>;
    { &Visitor::visit } -> std::same_as<visitor_visit_enum_fn<Visitor>>;
    { &Visitor::visit } -> std::same_as<visitor_visit_ext_fn<Visitor>>;
    { &Visitor::visit } -> std::same_as<visitor_visit_func_fn<Visitor>>;
    { &Visitor::visit } -> std::same_as<visitor_visit_object_fn<Visitor>>;
    { &Visitor::visit } -> std::same_as<visitor_visit_var_fn<Visitor>>;
};

} // namespace cherry::ast::detail


/// @brief   Module abstract syntax tree node.
/// @details A module contains a set of type aliases, enumerations, extensions,
///          functions, objects, and variables. The module makes these available
///          to other modules that import it.
struct module final {
    /// @brief   The type alias items within this module.
    /// @details Each type alias is redefinition of a given type for convenience
    ///          that this module uses and may make available to other modules.
    vec_t<alias> aliases;

    /// @brief   The enumeration items within this module.
    /// @details Each enumeration is a list of numeric constants that represent
    ///          anything from states to options.
    vec_t<enumeration> enumerations;

    /// @brief   The extension items within this module.
    /// @details Each extension defines that it reopens an object and adds new
    ///          items to it, such as: aliases, enumerations, functions, and
    ///          objects.
    vec_t<extension> extensions;

    /// @brief   The function items within this module.
    /// @details Each function defines a free-standing piece of logic within the
    ///          module which may be shared with other modules.
    vec_t<function> functions;

    /// @brief   The object items within this module.
    /// @details Each object defines a structure which stores information. They
    ///          may have utility members within them. A module may choose to
    ///          make those available with other modules.
    vec_t<object> objects;

    /// @brief   The variable items within this module.
    /// @details Each variable of a module is considered a "global" variable.
    ///          They are a way to share data between modules or to track data
    ///          privately within the same module heirarchy. A module may choose
    ///          to share it's variable with other modules.
    vec_t<variable> variables;


    /// @brief   Accepts a visitor to this module node.
    /// @details Accepting a visitor to this module node means that the visitor
    ///          will also visit each item of the module, that is, all of the
    ///          aliases, enumerations, extensions, functions, objects, and
    ///          variables.
    /// @tparam  Visitor The type of the visitor that is being provided.
    /// @param   visitor The visitor that we're expecting to receive.
    template<detail::ModuleVisitor Visitor>
    void
    accept(const Visitor& visitor) noexcept {
        using namespace std::ranges;
        using namespace std::placeholders;
        auto fn = std::mem_fn(&Visitor::visit);
        for_each(aliases, std::bind(fn, &visitor, _1));
        for_each(enumerations, std::bind(fn, &visitor, _1));
        for_each(extensions, std::bind(fn, &visitor, _1));
        for_each(functions, std::bind(fn, &visitor, _1));
        for_each(objects, std::bind(fn, &visitor, _1));
        for_each(variables, std::bind(fn, &visitor, _1));
    }
};


/// @brief   Compares one module to another module.
/// @param   lhs The left hand module to compare.
/// @param   rhs The right hand module to compare.
/// @returns True if both modules have the same aliases, enumerations,
///          extensions, functions, objects, and variables; otherwise false.
inline bool
operator==(const module& lhs, const module& rhs) noexcept {
    return lhs.aliases      == rhs.aliases      &&
           lhs.enumerations == rhs.enumerations &&
           lhs.extensions   == rhs.extensions   &&
           lhs.functions    == rhs.functions    &&
           lhs.objects      == rhs.objects      &&
           lhs.variables    == rhs.variables;
}

/// @brief   Compares one module to another module.
/// @param   lhs The left hand module to compare.
/// @param   rhs The right hand module to compare.
/// @returns True if both modules *do not* have the same aliases, enumerations,
///          extensions, functions, objects, and variables; otherwise false.
inline bool
operator!=(const module& lhs, const module& rhs) noexcept {
    return !operator==(lhs, rhs);
}

} // namespace cherry::ast