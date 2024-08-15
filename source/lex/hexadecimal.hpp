#pragma once


namespace simular::cherry::lex {

/// @brief   A lexical rule that can test the source code input for a
///          hexadecimal number and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce a
///          hexadecimal number from the input. A hexadecimal number is any
///          lexeme from the input that starts with '0x' and is followed by a
///          set of numbers from zero to nine, or the letters 'a' to 'f'.
struct hexadecimal_rule final {
    /// @brief   Checks the `source` if it starts with '0x'.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a '0x'; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return source.starts_with("0x");
    }

    /// @brief   Tokenizes a hexadecimal number by reading the input until it no
    ///          longer recognizes the input as a hexadecimal number.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the hexadecimal number that was tokenized;
    ///          an error may be produced if lexically the format of the number
    ///          is not allowed.
    result
    tokenize(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // Flush the '0x' from the buffer.
        ctx.start_token();
        ctx.read_src_char();
        ctx.read_src_char();

        // If we don't have a hexadecimal character after flushing the '0x' that
        // is a lexical error that the user should fix.
        if (ctx.end_of_source() || !is_hexadecimal(ctx.code[ctx.index]))
            return std::unexpected(errc::invalid_hexadecimal);
        
        // Keep reading until we don't have an hexadecimal character.
        while (!ctx.end_of_source() && is_hexadecimal(ctx.read_src_char()))
            continue;
        return ctx.extract_token(leaf::lv_signed);
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
        return std::isdigit(ch)  ||
               is_upper_digit(ch) ||
               is_lower_digit(ch);
    }
};

} // namespace simular::cherry::lex