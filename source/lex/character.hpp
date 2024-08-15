#pragma once


namespace simular::cherry::lex {

/// @brief   A lexical rule that can test the source code input for a character
///          literal and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce a
///          character literal from the input. A character literal is any lexeme
///          from the input that starts with a single quote and is followed by a
///          single character, escaped character, or unicode character, and ends
///          with a single quote.
struct character_rule final {
    /// @brief   Checks the `source` if it starts with a single quote.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a single quote; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return source.starts_with('\'');
    }

    /// @brief   Tokenizes a character literal by reading the input until it no
    ///          longer recognizes the input as a character literal.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the character literal that was tokenized; an
    ///          error may be produced if lexically the format of the character
    ///          literal is not allowed.
    result
    tokenize(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        ctx.start_token();
        ctx.read_src_char(); // Flush '\''.

        // Determine character type.
        if (ctx.curr_src_char() == '\\') {
            ctx.read_src_char(); // Flush backslash.
            if (ctx.curr_src_char() == 'u')
                return tokenize_unicode(ctx);
            
            // We can't have a orphaned escape character.
            if (ctx.curr_src_char() == '\'')
                return std::unexpected(errc::invalid_character);
        }
        
        // Read the next character.
        // If it's not the closing quote, that's an error.
        ctx.read_src_char();
        if (ctx.read_src_char() != '\'')
            return std::unexpected(errc::invalid_character);
        return ctx.extract_token(leaf::lv_character);
    }

private:
    static bool
    is_upper_digit(char ch) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return  ch >= 'A' || ch <= 'F';
    }

    static bool
    is_lower_digit(char ch) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return ch >= 'a' || ch <= 'f';
    }

    static bool
    is_hexadecimal(char ch) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return std::isdigit(ch)   ||
               is_upper_digit(ch) ||
               is_lower_digit(ch);
    }

    static result
    tokenize_unicode(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // Check this up front because it's illegal.
        ctx.read_src_char(); // Flush 'u'
        if (ctx.curr_src_char() == '\'')
            return std::unexpected(errc::invalid_unicode);

        // We can read up to 5 characters, we include the trailing '\''.
        for (auto index = 0; index < 5; index++) {
            if (ctx.read_src_char() == '\'')
                return ctx.extract_token(leaf::lv_character);
            
            if (!is_hexadecimal(ctx.curr_src_char()))
                return std::unexpected(errc::invalid_unicode);
        }

        // We didn't find the closing quote.
        return std::unexpected(errc::invalid_unicode);
    }
};

} // namespace simular::cherry::lex