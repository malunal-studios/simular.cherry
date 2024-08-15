#pragma once


namespace simular::cherry::lex {

/// @brief   A lexical rule that can test the source code input for a string
///          literal and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce a string
///          literal from the input. A string literal is any lexeme from the
///          input that starts with a double quote and ends with a double quote.
struct string_rule final {
    /// @brief   Checks the `source` if it starts with a double quote..
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a double quote; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return source.starts_with("r\"\"\"") ||
               source.starts_with('\"');
    }

    /// @brief   Tokenizes a string literal by reading the input until it no
    ///          longer recognizes the input as a string literal.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the string literal that was tokenized; an
    ///          error may be produced if lexically the format of the string
    ///          literal is not allowed.
    result
    tokenize(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // What type of string are we processing?
        auto index = ctx.index;
        ctx.start_token();
        
        // Determine if raw/multiline, and flush extra characters.
        auto mode = string_mode::lv;
        if (ctx.curr_src_char() == 'r') {
            // Flush 'r'
            ctx.read_src_char();

            // We have already verified 3 quotes at this point.
            // So we can ignore the value of handle_opener().
            handle_opener(ctx);
            mode = string_mode::raw;
        } else {
            mode = handle_opener(ctx);
            if (ctx.index - index == 2)
                return ctx.extract_token(leaf::lv_raw_string);
        }


        // Read until we hit the end of the line or the closing quote.
        leaf string_type = leaf::lv_raw_string;
        switch (mode) {
        case string_mode::lv:  return analyze_literal(ctx, string_type);
        case string_mode::ml:  return analyze_multiline(ctx, string_type);
        case string_mode::raw: return analyze_raw(ctx, string_type);
        }

        return std::unexpected(errc::unrecoverable);
    }

private:
    enum class string_mode : int8_t {
        lv, ml, raw
    };

    static bool
    should_continue(char ch) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return ch != '\n' && ch != '"';
    }

    static void
    handle_interpolation(state& ctx, leaf& type) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        if (ctx.curr_src_char() == '{' && ctx.prev_src_char() != '\\') {
            switch (type) {
            case leaf::lv_raw_string:
                type = leaf::lv_int_string;
                break;
            case leaf::lv_ml_string:
                type = leaf::lv_mli_string;
                break;
            }
        }
        
        ctx.read_src_char();
    }

    static string_mode
    handle_opener(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        auto count = 0;
        while (ctx.curr_src_char() == '\"' && count < 4) {
            ctx.read_src_char();
            count++;
        }

        return count == 3 
            ? string_mode::ml
            : string_mode::lv;
    }

    static bool
    handle_closure(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        auto count = 0;
        while (ctx.curr_src_char() == '\"' && count < 4) {
            ctx.read_src_char();
            count++;
        }

        // The two cases where this is used require this.
        return count == 3;
    }

    static result
    analyze_literal(state& ctx, leaf& type) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        while (!ctx.end_of_source() && should_continue(ctx.curr_src_char()))
            handle_interpolation(ctx, type);

        // If we stopped because it's a newline character, that's an error.
        // Strings must be terminated before the end of the line.
        if (ctx.end_of_source() || ctx.curr_src_char() == '\n')
            return std::unexpected(errc::invalid_raw_string);

        ctx.read_src_char(); // This should be the ending double quote.
        return ctx.extract_token(type);
    }

    static result
    analyze_multiline(state& ctx, leaf& type) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // Must set the type.
        type = leaf::lv_ml_string;
        while (!ctx.end_of_source() && ctx.curr_src_char() != '\"')
            handle_interpolation(ctx, type);

        // The string must be terminated!
        if (ctx.end_of_source() || handle_closure(ctx) == false)
            return std::unexpected(errc::invalid_ml_string);

        return ctx.extract_token(type);
    }

    static result
    analyze_raw(state& ctx, leaf& type) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        while (!ctx.end_of_source() && ctx.curr_src_char() != '\"')
            handle_interpolation(ctx, type);

        // If we stopped because it's a newline character, that's an error.
        // Strings must be terminated before the end of the line.
        if (ctx.end_of_source() || handle_closure(ctx) == false)
            return std::unexpected(errc::invalid_raw_string);

        return ctx.extract_token(type);
    }
};

} // namespace simular::cherry::lex