#pragma once


namespace simular::cherry::lex {

/// @brief   A lexical rule that can test the source code input for a operator
///          character and tokenize it when the litmus is successful.
/// @details This rule is greedy and will tokenize the longest known operator
///          for any given input.
/// @remarks This will also handle delimiters!
struct operator_rule final {
    /// @brief   Checks the `source` if it starts with a known operator or
    ///          delimiting character.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a known operator or delimiting
    ///          character; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        static uset_t<wchar_t>
        k_known_ops {
            '+', '-', '*', '/', '%', '=',
            '.', '?', '~', '&', '|', '^',
            '<', '>', '!', '(', ')', '{',
            '}', '[', ']', ',', ';', ':'
        };

        return k_known_ops.find(source[0])
            != k_known_ops.cend();
    }

    /// @brief   Tokenizes an operator or delimiting character by reading the
    ///          input until it no longer recognizes the input as a operator or
    ///          delimiting character.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the operator or delimiting character that
    ///          was tokenized; an error will not be produced from this lexical
    ///          analyzer.
    result
    tokenize(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return tokenize_single(ctx);
    }

private:
    static result
    tokenize_single(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        ctx.start_token();
        switch (ctx.read_src_char()) {
        // Possible doubles or equals.
        case '+': return tokenize_double(ctx, leaf::op_add);
        case '-': return tokenize_double(ctx, leaf::op_sub);
        case '&': return tokenize_double(ctx, leaf::op_bitand);
        case '|': return tokenize_double(ctx, leaf::op_bitor);
        case '<': return tokenize_double(ctx, leaf::op_logless);
        case '>': return tokenize_double(ctx, leaf::op_logmore);
        case '.': return tokenize_double(ctx, leaf::op_access);
        case '*': return tokenize_equals(ctx, leaf::op_mul);
        case '/': return tokenize_equals(ctx, leaf::op_div);
        case '%': return tokenize_equals(ctx, leaf::op_mod);
        case '=': return tokenize_equals(ctx, leaf::op_assign);
        case '~': return tokenize_equals(ctx, leaf::op_bitnot);
        case '^': return tokenize_equals(ctx, leaf::op_bitxor);
        case '!': return tokenize_equals(ctx, leaf::op_lognot);
        
        // Actual singles.
        case '?': return ctx.extract_token(leaf::op_ternary);
        case '(': return ctx.extract_token(leaf::dc_lparen);
        case ')': return ctx.extract_token(leaf::dc_rparen);
        case '{': return ctx.extract_token(leaf::dc_lbrace);
        case '}': return ctx.extract_token(leaf::dc_rbrace);
        case '[': return ctx.extract_token(leaf::dc_lbracket);
        case ']': return ctx.extract_token(leaf::dc_rbracket);
        case ',': return ctx.extract_token(leaf::dc_comma);
        case ';': return ctx.extract_token(leaf::dc_terminator);
        case ':': return ctx.extract_token(leaf::dc_colon);
        }

        // Don't know what this could be.
        return ctx.extract_token(leaf::unknown);
    }

    static result
    tokenize_double(state& ctx, leaf type) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // If the character isn't doubled, jump.
        if (ctx.prev_src_char() != ctx.curr_src_char())
            return tokenize_equals(ctx, type);
        ctx.read_src_char();
        switch (type) {
        // Only true doubles.
        case leaf::op_add: return ctx.extract_token(leaf::op_inc);
        case leaf::op_sub: return ctx.extract_token(leaf::op_dec);

        // Only known triple.
        case leaf::op_access: return tokenize_triple(ctx, leaf::op_cascade);

        // Possible equals.
        case leaf::op_bitand:  return tokenize_equals(ctx, leaf::op_logand);
        case leaf::op_bitor:   return tokenize_equals(ctx, leaf::op_logor);
        case leaf::op_logless: return tokenize_equals(ctx, leaf::op_bitlsh);
        case leaf::op_logmore: return tokenize_equals(ctx, leaf::op_bitrsh);
        default: break;
        }

        // Don't know what this could be.
        return ctx.extract_token(leaf::unknown);
    }

    static result
    tokenize_triple(state& ctx, leaf type) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // If the character isn't tripled, jump.
        if (ctx.prev_src_char() != ctx.curr_src_char())
            return ctx.extract_token(type);
        ctx.read_src_char();
        return ctx.extract_token(leaf::op_ellipsis);
    }

    static result
    tokenize_equals(state& ctx, leaf type) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        if (ctx.curr_src_char() != '=')
            return ctx.extract_token(type);
        ctx.read_src_char();
        switch (type) {
        case leaf::op_add:     return ctx.extract_token(leaf::op_add_eq);
        case leaf::op_sub:     return ctx.extract_token(leaf::op_sub_eq);
        case leaf::op_mul:     return ctx.extract_token(leaf::op_mul_eq);
        case leaf::op_div:     return ctx.extract_token(leaf::op_div_eq);
        case leaf::op_mod:     return ctx.extract_token(leaf::op_mod_eq);
        case leaf::op_assign:  return ctx.extract_token(leaf::op_logequals);
        case leaf::op_bitnot:  return ctx.extract_token(leaf::op_bitnot_eq);
        case leaf::op_bitand:  return ctx.extract_token(leaf::op_bitand_eq);
        case leaf::op_bitor:   return ctx.extract_token(leaf::op_bitor_eq);
        case leaf::op_bitxor:  return ctx.extract_token(leaf::op_bitxor_eq);
        case leaf::op_bitlsh:  return ctx.extract_token(leaf::op_bitlsh_eq);
        case leaf::op_bitrsh:  return ctx.extract_token(leaf::op_bitrsh_eq);
        case leaf::op_lognot:  return ctx.extract_token(leaf::op_lognot_eq);
        case leaf::op_logand:  return ctx.extract_token(leaf::op_logand_eq);
        case leaf::op_logor:   return ctx.extract_token(leaf::op_logor_eq);
        case leaf::op_logless: return ctx.extract_token(leaf::op_logless_eq);
        case leaf::op_logmore: return ctx.extract_token(leaf::op_logmore_eq);
        default: break;
        }

        // Don't know what this could be.
        return ctx.extract_token(leaf::unknown);
    }
};

} // namespace simular::cherry::lex