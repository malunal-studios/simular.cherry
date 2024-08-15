#pragma once


namespace cherry::ast {

/// @brief   Expression abstract syntax tree node.
/// @details ...
struct expr {
    /// @brief   The different expression types supported.
    /// @details ...
    enum class type {
        literal,
        path,
        arith,
        group,
        array,
        index,
        fncall,
        method,
        field,
        next,
        brk,
        ret,
        block,
        loop,
        ifthen
    };

protected:
    type subtype_;
    bool with_block_;

    explicit
    expr(type subtype, bool with_block) noexcept
        : subtype_{ subtype }
        , with_block_{ with_block }
    { }
};


/// @brief 
struct lit_expr final : expr {
};


/// @brief 
struct arith_expr final : expr {
};



struct group_expr;
struct array_expr;
struct index_expr;
struct fncall_expr;
struct method_expr;
struct field_expr;
struct next_expr;
struct break_expr;
struct return_expr;
struct block_expr;
struct loop_expr;
struct ifthen_expr;


} // namespace cherry::ast