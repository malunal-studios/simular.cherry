#pragma once
#include "paths.hpp"

namespace cherry::syn {

/// @brief   Capable of paring an import statement.
/// @details An import statement is the `using` keyword followed by a simple
///          path then followed by the statement termination token (`;`).
/// @tparam  Lexer The type of the lexer that will provide the tokens for the
///          parser to syntactically analyze.
template<detail::LexicalAnalyzer Lexer>
struct import_parser final {
    static result<ast::import>
    parse(state<Lexer>& ctx) noexcept {
        if (ctx.current.type != leaf::kw_using)
            return std::unexpected(errc::not_my_syntax);
        auto node = ast::import{};
        ctx.next_token();

        // Parse path.
        auto result = simple_path_parser<Lexer>::parse(ctx);
        if (!result.has_value())
            // TODO: return appropriate error code.
            return std::unexpected(errc::failure);
        node.path = std::move(result.value());
        
        // Parse terminator.
        if (ctx.current.type != leaf::dc_terminator)
            return std::unexpected(errc::expected_terminator);
        ctx.next_token();
        return node;
    }
};

} // namespace cherry::syn