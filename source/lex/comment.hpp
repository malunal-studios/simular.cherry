#pragma once


namespace simular::cherry::lex {

/// @brief   A lexical rule that can test the source code input for a comment
///          and tokenize it when the litmus is successful.
/// @details `Cherry` specifically defines a comment to be any piece of source
///          code which start with the hash symbol (`#`). It reads until the end
///          of the current line. Comments are not useful information typically,
///          and as such they are ignored and trashed by the parser. However,
///          the design of the lexer requires that they be tokenized.
struct comment_rule final {
    /// @brief   Checks the `source` if it starts with the hash symbol (`#`).
    /// @param   source The source code input that this rule should test.
    /// @returns True if the hash symbol is the first character of the source
    ///          code string, false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return source.starts_with('#');
    }

    /// @brief   Tokenizes the comment by reading the input until the end of the
    ///          line of the source code that was received as input.
    /// @param   ctx The current state of lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the comment that was tokenized; no error
    ///          will be produced from this rule.
    result
    tokenize(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // Just loop until we hit a new line.
        ctx.start_token();
        ctx.read_src_char();
        while (!ctx.end_of_source() && ctx.curr_src_char() != '\n')
            ctx.read_src_char();
        return ctx.extract_token(leaf::comment);
    }
};

} // namespace simular::cherry::lex