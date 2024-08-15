#pragma once


namespace simular::cherry::lex {

/// @brief   A lexical rule that can test the source code input for a decimal
///          number and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce a decimal
///          number from the input. A decimal number is any lexeme from the
///          input that starts with a number from one to nine and is followed by
///          a decimal point and/or a set of numbers from zero to nine.
/// @remarks A decimal number may start with 0 if and only if, it is the only
///          number at the start of the lexeme, or is followed by a decimal
///          point and a set of numbers from zero to nine.
struct decimal_rule final {
    /// @brief   Checks the `source` if it starts with an integer and maybe
    ///          continues with a decimal point.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with an integer and maybe continues
    ///          with a decimal point; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // If it's a number, then make sure we can read the next character.
        // If the next character is a decimal point, then we are working with
        // a decimal number.
        if (!std::isdigit(source[0]))
            return false;
        
        // Make sure this isn't an octal number.
        if (source[0] == '0' && source.size() > 1)
            return source[1] != 'b' &&
                   source[1] != 'x' &&
                   !std::isdigit(source[1]);
        
        // This is a valid decimal number, we won't make an assumption here
        // about what the next character is.
        return true;
    }

    /// @brief   Tokenizes a decimal number by reading the input until it no
    ///          longer recognizes the input as a decimal number.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the decimal number that was tokenized; an
    ///          error may be produced if lexically the format of the number is
    ///          not allowed.
    result
    tokenize(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // We need to keep all characters of this type of number.
        // We'll read this differently so we can catch a '.' and stop immediately
        // instead of accidentally reading passed it.
        ctx.start_token();
        ctx.read_src_char();
        while (!ctx.end_of_source() && std::isdigit(ctx.curr_src_char()))
            ctx.read_src_char();
        
        // If there isn't a decimal point, the number was just an integer.
        if (ctx.curr_src_char() != '.')
            return ctx.extract_token(leaf::lv_signed);

        // Is there any more numbers after the decimal point?
        if (!std::isdigit(ctx.next_src_char()))
            return ctx.extract_token(leaf::lv_signed);

        // Grab the rest of the digits.
        ctx.read_src_char();
        while (!ctx.end_of_source() && std::isdigit(ctx.curr_src_char()))
            ctx.read_src_char();
        return ctx.extract_token(leaf::lv_decimal);
    }
};

} // namespace simular::cherry::lex