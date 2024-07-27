#pragma once
#include "kernel.hpp"
#include "lexer.hpp"


namespace cherry {
namespace syn {

/// @brief 
enum class errc {
    unrecoverable = -1,
    success = EXIT_SUCCESS,
    failure = EXIT_FAILURE,

    /// @brief   Provided by a syntax analyzer which cannot process the token
    ///          stream provided to it.
    /// @details If a parser has at least one syntax rule and fails all of its
    ///          litmus tests to find a rule which can parse the input, it will
    ///          provide this error code.
    not_my_syntax,
};


/// @brief 
using result = std::expected<ast::node, errc>;


namespace detail {

/// @brief  Defines the type of the lexer tokenize function.
/// @tparam Lexer The type that is expected to have a tokenize function. 
template<typename Lexer>
using lexer_tokenize_fn = lex::result(*)(lex::state&) noexcept;

/// @brief   Verifies that the type given to it conforms to a lexer.
/// @details A type conforms to being a lexer if it provides a static function
///          that returns a lexical analysis result (either a token or an error)
///          and takes in a lexical analysis state.
/// @tparam  Lexer The type that we are verifying as a lexer.
template<typename Lexer>
concept LexicalAnalyzer = requires {
    { &Lexer::tokenize } -> std::same_as<lexer_tokenize_fn<Lexer>>;
};

} // namespace cherry::syn::detail


/// @brief   Contains the state of syntax analysis for a single parsing context.
/// @details
template<detail::LexicalAnalyzer Lexer>
struct state final {
    using lexstate_t = lex::state;
    using lextoken_t = lex::token;

    /// @brief   The current state of the lexical analysis for the source code
    ///          that was read from `path`.
    /// @details ...
    lexstate_t lex_state{};

    /// @brief   The path to the source code on the system.
    /// @details This should be retained throughout the compilation process for
    ///          the purposes of error reporting.
    string_t path{};

    lextoken_t current{};

    /// @brief   Reads the next token of the source code.
    /// @details This will attempt to read a token. If it is unable to, it will
    ///          provide the error from the lexer. If it successfully reads a
    ///          token, then the current token will be set to the result of the
    ///          tokenization process. The result in this case is success.
    /// @returns An error from the lexer if the token could not be read;
    ///          otherwise success.
    lex::errc
    next_token() noexcept {
        auto result = Lexer::tokenize(lex_state);
        if (!result.has_value())
            return result.error();
        current = result.value();
        return lex::errc::success;
    }
};


template<detail::LexicalAnalyzer Lexer>
struct package_parser final {
    static ast::node
    parse(syn::state<Lexer>& ctx) noexcept {
        // Read tokens produce node
    }
};


} // namespace cherry
