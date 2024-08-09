#pragma once


namespace cherry::syn {

/// @brief   Capable of parsing a type expression.
/// @details A type expression is one where a type is being defined and is
///          either annotating another expression, or the value of another
///          expression.
/// @tparam  Lexer The type of the lexer that will provide the tokens for the
///          parser to syntactically analyze.
template<detail::LexicalAnalyzer Lexer>
struct type_parser final {
    static result<uptr_t<ast::type>>
    parse(state<Lexer>& ctx) noexcept;

private:
    static result<uptr_t<ast::type>>
    parse_function(state<Lexer>& ctx) noexcept {
        // Flush '(' from input.
        ctx.next_token();
        
        auto result = std::make_unique<ast::type::fn>();
        while (ctx.current.type != leaf::dc_rparen) {
            auto param = parse(ctx);
            if (!param.has_value())
                // TODO: return appropriate error code.
                return std::unexpected(errc::failure);
            result->inputs.emplace_back(std::move(param.value()));
            if (ctx.current.type == leaf::dc_comma)
                ctx.next_token();
        }

        // Flush ')' from input.
        ctx.next_token();
        if (ctx.current.type == leaf::dc_colon) {
            ctx.next_token();
            auto output = parse(ctx);
            if (!output.has_value())
                // TODO: return appropriate error code.
                return std::unexpected(errc::failure);
            result->output = std::move(output.value());
        }
        return result;
    }

    static result<uptr_t<ast::type>>
    parse_array(state<Lexer>& ctx) noexcept {
        return nullptr;
    }
};


} // namespace cherry::syn