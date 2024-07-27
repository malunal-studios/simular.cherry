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


struct document;
struct import;
struct module;
struct variable;
struct function;
struct object;



/// @brief 
struct document : detail::node {
    vec_t<import> includes;
    vec_t<module> modules;
};

/// @brief 
struct import : detail::node {
    string_t module;
};

/// @brief 
struct module : detail::node {
    vec_t<variable> variables;
    vec_t<function> functions;
    vec_t<object>   objects;
};

/// @brief 
struct variable : detail::node {
    
};

/// @brief 
struct function : detail::node {
};

/// @brief 
struct object : detail::node {
};


/// @brief   Type definition for unique pointer of abstract syntax tree node.
/// @details This simplifies the usage of abstract syntax tree nodes that are
///          produced from the syntax analysis system.
using node = uptr_t<detail::node>;

} // namespace cherry::ast