#pragma once
#include "types.hpp"

namespace cherry::ast {
namespace detail {

/// @brief
struct node {
    explicit node() = default;
    virtual ~node() = default;
};

} // namespace cherry::ast::detail

/// @brief   Type definition for unique pointer of abstract syntax tree node.
/// @details This simplifies the usage of abstract syntax tree nodes that are
///          produced from the syntax analysis system.
using node = uptr_t<detail::node>;

} // namespace cherry::ast

#include "ast/import.hpp"
#include "ast/paths.hpp"
