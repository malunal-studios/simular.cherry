#pragma once


namespace simular::cherry::lex {

/// @brief   A lexical rule that can test the source code input for a binary
///          number and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce a binary
///          number from the input. A binary number is any lexeme from the input
///          that starts with '0b' and is followed by a set of zeros and ones.
struct binary_rule final {
    /// @brief   Checks the `source` if it starts with '0b'.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a '0b'; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return source.starts_with("0b");
    }

    /// @brief   Tokenizes a binary number by reading the input until it no
    ///          longer recognizes the input as a binary number.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the binary number that was tokenized; an
    ///          error may be produced if lexically the format of the number is
    ///          not allowed.
    result
    tokenize(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // Flush the '0b' from the buffer.
        ctx.start_token();
        ctx.read_src_char();
        ctx.read_src_char();

        // If we don't have a binary character after flushing the '0b' that is
        // a lexical error that the user should fix. 
        if (ctx.end_of_source() || !is_binary(ctx.next_src_char()))
            return std::unexpected(errc::invalid_binary);
        
        // Keep reading until we don't have a binary character.
        while (!ctx.end_of_source() && is_binary(ctx.read_src_char()))
            continue;
        return ctx.extract_token(leaf::lv_signed);
    }

private:
    static bool
    is_binary(char ch) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return ch == '0' || ch == '1';
    }
};

} // namespace simular::cherry::lex