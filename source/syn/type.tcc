#pragma once


namespace cherry::syn {

// Definition of this function after the fact.
template<detail::LexicalAnalyzer Lexer>
inline result<uptr_t<ast::type>>
type_parser<Lexer>::parse(state<Lexer>& ctx) noexcept {
    // Try to parse path expr first.
    auto path = path_expr_parser<Lexer>::parse(ctx);
    if (!path.has_value()) {
        auto error = path.error();
        if (error == errc::not_my_syntax)
            return std::unexpected(error);
        
        // TODO: return appropriate error.
        return std::unexpected(errc::failure);
    }
    
    // Are we parsing a function type?
    if (ctx.current.type == leaf::dc_lparen) {
        auto fn = parse_function(ctx);
        if (!fn.has_value())
            // TODO: return appropriate error code.
            return std::unexpected(errc::failure);
        fn.value()->segments = std::move(path.value().segments);
        return std::move(fn.value());
    }
    
    // Are we parsing an array type?
    if (ctx.current.type == leaf::dc_lbracket) {
        auto arr = parse_array(ctx);
        if (!arr.has_value())
            // TODO: return appropriate error code.
            return std::unexpected(errc::failure);
        arr.value()->segments = std::move(path.value().segments);
        return std::move(arr.value());
    }
    
    // Are we parsing a variant at all?
    if (ctx.current.type != leaf::op_mul &&
        ctx.current.type != leaf::op_bitand) {
        auto node = std::make_unique<ast::type>();
        node->segments = std::move(path.value().segments);
        return node;
    }

    // Looks like we're parsing a pointer/reference type.
    auto node = std::make_unique<ast::type::ref>();
    while (ctx.current.type == leaf::op_mul ||
           ctx.current.type == leaf::op_bitand) {
        if (ctx.current.type == leaf::op_mul)
            node->depth.emplace_back(1);
        else if (ctx.current.type == leaf::op_bitand)
            node->depth.emplace_back(0);
        else break;
        
        ctx.next_token();
    }

    node->segments = std::move(path.value().segments);
    return node;
}

} // namespace cherry::syn