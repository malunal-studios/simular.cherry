#pragma once


namespace simular::cherry::lex {

/// @brief   Contains the state of lexical analysis of a single lexing context.
/// @details The state is responsible for providing the lexer information on
///          where the last state of the analysis was, as lexer's in `cherry`
///          do not retain the state themselves. The reason that `cherry` does
///          not retain this information within the lexer is because it makes
///          it decoupled (obviously), allowing the information to be handled
///          by any lexer from any thread. This provides the ability to pause
///          analysis on one thread and pick it up on another through a queue
///          if that level of control is necessary. It also makes the data more
///          readily accessible to other components, allowing them to mutate
///          the data if necessary, but primarily making it easier for the
///          syntactic and semantic analysis to operate with.
struct state final {
    /// @brief   The source code that we're currently analyzing.
    /// @details This should be retained throughout the compilation process for
    ///          multiple reasons. Obviously, it's needed for the purpose of
    ///          lexical analysis to begin with; including lexeme extraction.
    ///          It is also needed for error reporting, similarily to the path
    ///          of the source code.
    /// @sa      token::lexeme
    string_t code{};

    /// @brief   The current line of source code that is being analyzed.
    /// @details The line should be incremented anytime that the lexer, or a
    ///          lexical rule, encounters a newline `\\n` within the source
    ///          code. It should ignore/skip carraige return `\\r` and form
    ///          feed `\\f` characters. Using the `read()` method will do this
    ///          automatically.
    uint64_t line{0};

    /// @brief   The current character, within the current line, of source code
    ///          that is being analyzed.
    /// @details The column should be incremented anytime that the lexer, or a
    ///          lexical rule, reads a character of the source using the method
    ///          `read()` provided by this state.
    uint64_t column{0};

    /// @brief   The current index within the source code string.
    /// @details The line and column are for error reporting purposes, for the
    ///          most part, but this is for actually tracking the location
    ///          within the source code that the lexer is currently at.
    uint64_t index{0};

    /// @brief   Utility function for lexical analysis to check whether the end
    ///          of the source code has been reached.
    /// @returns True if the tracked index is equal to the length of the source
    ///          code; false otherwise.
    bool
    end_of_source() const noexcept {
        return index == code.length();
    }

    /// @brief   Utility function for lexical analysis to check the next source
    ///          code character.
    /// @returns The character at the next index of the source code.
    int32_t
    next_src_char() const noexcept {
        return static_cast<int32_t>(code[index + 1]);
    }

    /// @brief   Utility function for lexical analysis to check the current
    ///          source code character.
    /// @returns The character at the current index of the source code.
    int32_t
    curr_src_char() const noexcept {
        return static_cast<int32_t>(code[index]);
    }

    /// @brief   Utility function for lexical analysis to check the previous
    ///          source code character.
    /// @returns The character at the previous index of the source code.
    int32_t
    prev_src_char() const noexcept {
        return static_cast<int32_t>(code[index - 1]);
    }

    /// @brief   Utility function for lexical analysis to read the a character
    ///          from the source code that the state is managing.
    /// @details This allows the lexical rules to compare the character of the
    ///          source to what they expected to see next. Often this means that
    ///          the lexical rule is done analyzing and should extract the token.
    /// @returns The character that was read from the source code.
    int32_t
    read_src_char() noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // We've hit the end already.
        if (index == code.length())
            return '\0';

        auto res = code[index++];
        if (res == '\n') {
            line++;
            column = 0;
        } else column++;

        return res;
    }

    /// @brief   Utility function for lexical analysis to get the remaining
    ///          source code for further lexical analysis.
    /// @returns A view of the remaining source code, starting from the current
    ///          index and ending at the end of the source.
    strview_t
    remaining_source() const noexcept {
        return strview_t(
            code.cbegin() + index,
            code.cend()
        );
    }
    
    /// @brief   Utility function for lexical analysis to track the beginning
    ///          of a token for the extraction of that token.
    /// @details This will record the starting line and starting column within
    ///          the starting line, that the token exists as well as start
    ///          tracking the lexeme of the token.
    void
    start_token() noexcept {
        token_.line   = line;
        token_.column = column;
        start_lexeme();
    }

    /// @brief   Utility function for lexical analysis to extract a token from
    ///          the source code that is being analyzed.
    /// @details This will stop tracking the lexeme of the token and set the
    ///          type of the token the value provided by the caller. It will
    ///          then provide the token that was recorded since calling the
    ///          `start_token()` function.
    /// @param   type The type of the token the caller has determined the
    ///          recorded token to be.
    /// @returns The token that was recorded.
    token
    extract_token(leaf type) noexcept {
        token_.lexeme = extract_lexeme();
        token_.type   = type;
        return token_;
    }

private:
    token    token_{};
    uint64_t lexeme_start_{0};

    void
    start_lexeme() noexcept {
        lexeme_start_ = index;
    }

    strview_t
    extract_lexeme() const noexcept {
        return strview_t(
            code.cbegin() + lexeme_start_,
            code.cbegin() + index
        );
    }
};

} // namespace simular::cherry::lex