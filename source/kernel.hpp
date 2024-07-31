#pragma once
#include "ast/alias.hpp"
#include "ast/document.hpp"
#include "ast/enumeration.hpp"
#include "ast/extension.hpp"
#include "ast/function.hpp"
#include "ast/import.hpp"
#include "ast/module.hpp"
#include "ast/object.hpp"
#include "ast/paths.hpp"
#include "ast/variable.hpp"


namespace cherry::ast {

/// @brief   Defines an object which is a visitor for the abstract syntax tree.
/// @details Responsible for visiting each node of an abstract syntax tree to
///          perform operations on it such as symbol table construction,
///          semantic analysis, and code generation.
struct visitor  {
    explicit visitor() = default;
    virtual ~visitor() = default;

    virtual void
    visit(const alias& alias) noexcept = 0;

    virtual void
    visit(const document& document) noexcept = 0;

    virtual void
    visit(const enumeration& enumeration) noexcept = 0;

    virtual void
    visit(const extension& extension) noexcept = 0;

    virtual void
    visit(const function& function) noexcept = 0;

    virtual void
    visit(const import& import) noexcept = 0;

    virtual void
    visit(const module& module) noexcept = 0;

    virtual void
    visit(const object& object) noexcept = 0;

    virtual void
    visit(const simple_path& path) noexcept = 0;

    virtual void
    visit(const variable& variable) noexcept = 0;
};

} // namespace cherry::ast