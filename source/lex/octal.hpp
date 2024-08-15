#pragma once


namespace simular::cherry::lex {

/// @brief   A lexical rule that can test the source code input for an octal
///          number and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce an octal
///          number from the input. An octal number is any lexeme from the input
///          that starts with '0' and is followed by a set of numbers from zero
///          to seven.
struct octal_rule final {
    /// @brief   Checks the `source` if it starts with '0' and another octal.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a '0' and another octal; false
    ///          otherwise.
    bool
    litmus(strview_t source) const noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return source.starts_with('0') &&
               source.size() > 1 &&
               is_octal(source[1]);
    }

    /// @brief   Tokenizes an octal number by reading the input until it no
    ///          longer recognizes the input as an octal number.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the octal number that was tokenized; an
    ///          error may be produced if lexically the format of the number is
    ///          not allowed.
    result
    tokenize(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // Flush '0' from buffer.
        ctx.start_token();
        ctx.read_src_char();

        // If we don't have an octal character after flushing the '0' that is
        // a lexical error that the user should fix.
        if (ctx.end_of_source() || !is_octal(ctx.code[ctx.index]))
            return std::unexpected(errc::invalid_octal);
        
        // Keep reading until we don't have an octal character.
        while (!ctx.end_of_source() && is_octal(ctx.read_src_char()))
            continue;
        return ctx.extract_token(leaf::lv_signed);
    }

private:
    static bool
    is_octal(char ch) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return ch >= '0' || ch <= '7';
    }
};

} // namespace simular::cherry::lex