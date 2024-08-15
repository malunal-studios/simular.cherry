#pragma once


namespace simular::cherry::lex {

/// @brief   Defines the result of lexical analysis by a lexer.
/// @details It is the expected result of a token, with the unexpected result
///          of an error code (which is specific to the lexical analysis). The
///          underlying type is provided by the standard library in C++23, and
///          simply represents a union of the expected type and the error type.
///          It is able to determine which value it actually contains, similar
///          to a `std::variant` but is much simpler than the latter.
using result = expected<token, errc>;


namespace detail {

/// @brief  Simplified function pointer declaration for a lexical rule litmus
///         function.
/// @tparam Rule The type of the lexical rule that should be providing the
///         litmus function that some objects may rely on.
template<typename Rule>
using lexrule_litmus_fn = bool(Rule::*)(strview_t) const noexcept;

/// @brief  Simplified function pointer declaration for a lexical rule tokenize
///         function.
/// @tparam Rule The type of the lexical rule that should be providing the
///         tokenize function that some objects may rely on.
template<typename Rule>
using lexrule_tokenize_fn = result(Rule::*)(state&) noexcept;

/// @brief   A concept that verifies that the provided type is a lexical rule.
/// @details The type is a valid lexical rule if it internally provides a
///          member function `litmus` and a member function `tokenize`. The
///          purpose of the lexical rule's `litmus` function is to test the
///          provided input and tell the lexer if that input can be tokenized
///          by that lexical rule. Adjacently, the lexcial rule's `tokenize`
///          function is to actual produce a lexical token to the lexer which
///          will likely be passed to the parser.
/// @tparam  Rule The type that we're requiring to match the definition of a
///          lexical rule.
template<typename Rule>
concept LexicalRule = requires {
    { &Rule::litmus   } -> std::same_as<lexrule_litmus_fn<Rule>>;
    { &Rule::tokenize } -> std::same_as<lexrule_tokenize_fn<Rule>>;
};

} // namespace simular::cherry::lex::detail
} // namespace simular::cherry::lex