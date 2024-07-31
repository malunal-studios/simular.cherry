#pragma once


namespace cherry::syn {

/// @brief   Capable of parsing a type expression.
/// @details A type expression is one where a type is being defined and is
///          either annotating another expression, or the value of another
///          expression.
/// @tparam  Lexer The type of the lexer that will provide the tokens for the
///          parser to syntactically analyze.
template<detail::LexicalAnalyzer Lexer>
struct type_parser final {
    static result<ast::type>
    parse(state<Lexer>& ctx) noexcept;
};


} // namespace cherry::syn