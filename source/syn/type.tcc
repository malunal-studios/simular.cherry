#pragma once


namespace cherry::syn {

// Definition of this function after the fact.
template<detail::LexicalAnalyzer Lexer>
inline result<ast::type>
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

    auto node = ast::type{};
    node.segments = std::move(path.value().segments);
    
    // Determine variant and parse that.
    return node;
}

} // namespace cherry::syn