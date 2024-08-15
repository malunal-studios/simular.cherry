#pragma once


namespace simular::cherry::lex {

/// @brief   Contains the information needed to indicate the location and type
///          of a lexeme from the source being analyzed by a lexer.
/// @details A token is simply the combination of a lexeme with a token type and
///          line/column location. Tokens are typically read one at a time and
///          kept all the way through parsing until a full expression has been
///          realized by the parser. They mostly serve the purpose of indicating
///          to the parser what is being parsed and as a means to provide error
///          reporting when something unexpected is encountered.
struct token final {
    /// @brief   Contains the snippet of the source code being analyzed.
    /// @details This is a view of that source code. The source code string is
    ///          stored within the current state of the lexer, so it is safe to
    ///          merely take a view of that source. It takes very little memory
    ///          and provides a lot of context to the lexer/parser.
    strview_t lexeme;

    /// @brief   The type of the token as determined by the rule that extracted
    ///          the lexeme and produced this token.
    /// @details The type of the token is based on the lexeme that was extracted
    ///          by a lexical rule of a lexer. That rule will likely have some
    ///          internal function that compares the lexeme to a set of known
    ///          lexemes and produce a type based on the comparison.
    leaf type;

    /// @brief   The line from the source code this lexeme was found.
    /// @details This is primarily used for error reporting and debugging if
    ///          something where to fail near this token, but may be used for
    ///          other purposes as seen fit.
    uint64_t line;

    /// @brief   The column from the line of source code this lexeme was found.
    /// @details This is primarily used for error reporting and debugging if
    ///          something where to fail near this token, but may be used for
    ///          other purposes as seen fit.
    uint64_t column;
};

/// @brief   Compares two tokens.
/// @param   lhs The first token to compare.
/// @param   rhs The second token to compare.
/// @returns True if the tokens *are* equal by all properties; false
///          otherwise.
inline bool
operator==(const token& lhs, const token& rhs) noexcept {
    return lhs.lexeme == rhs.lexeme &&
           lhs.type   == rhs.type   &&
           lhs.line   == rhs.line   &&
           lhs.column == rhs.column;
}

/// @brief   Compares two tokens.
/// @param   lhs The first token to compare.
/// @param   rhs The second token to compare.
/// @returns True if the token *are not* equal by all properties; false
//           otherwise.
inline bool
operator!=(const token& lhs, const token& rhs) noexcept {
    return !(lhs == rhs);
}

/// @brief   Writes a token to a output stream.
/// @param   oss The output stream to write the token to.
/// @param   tkn The token to write to the output stream.
/// @returns The output stream that the token was written to. 
inline std::ostream&
operator<<(std::ostream& oss, const token& tkn) noexcept {
    return oss
        << "Token: {\n"
        << "  lexeme: \"" << tkn.lexeme << "\"\n"
        << "  type:    "  << tkn.type   << '\n'
        << "  line:    "  << tkn.line   << '\n'
        << "  column:  "  << tkn.column << '\n'
        << "}\n";
}

} // namespace simular::cherry::lex