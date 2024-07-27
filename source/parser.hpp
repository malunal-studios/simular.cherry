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
template<typename Node>
using result = std::expected<Node, errc>;


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

// TODO: Simplify token expectations.
//       Always doing if (ctx.current.type != leaf::identifier) or similar
//       check is tedious and common. Should be able to return error if not
//       matched as well.

/// @brief   Capable of parsing a simple path.
/// @details A simple path is a single identifier followed by any number of
///          other identifiers preceded by the access operator. For example,
///          `std.io.file`.
/// @tparam  Lexer The type of the lexer that will provide the tokens for the
///          parser to syntactically analyze.
template<detail::LexicalAnalyzer Lexer>
struct simple_path_parser final {
    static result<ast::simple_path>
    parse(state<Lexer>& ctx) noexcept {
        if (ctx.current.type != leaf::identifier)
            return std::unexpected(errc::not_my_syntax);
        auto node = ast::simple_path{};
        node.segments.push_back(ctx.current.lexeme);

        // While there is an access operator, keep adding segments.
        ctx.next_token();
        while (ctx.current.type == leaf::op_access) {
            ctx.next_token();
            if (ctx.current.type != leaf::identifier)
                // TODO: return appropriate error code.
                return std::unexpected(errc::failure);
            node.segments.push_back(ctx.current.lexeme);
            ctx.next_token();
        }
        return node;
    }
};

/// @brief   Capable of paring an import statement.
/// @details An import statement is the `using` keyword followed by a simple
///          path then followed by the statement termination token (`;`).
/// @tparam  Lexer The type of the lexer that will provide the tokens for the
///          parser to syntactically analyze.
template<detail::LexicalAnalyzer Lexer>
struct import_parser final {
    static result<ast::import>
    parse(state<Lexer>& ctx) noexcept {
        if (ctx.current.type != leaf::kw_using)
            return std::unexpected(errc::not_my_syntax);
        auto node = ast::import{};
        ctx.next_token();

        // Parse path.
        auto result = simple_path_parser<Lexer>::parse(ctx);
        if (!result.has_value())
            // TODO: return appropriate error code.
            return std::unexpected(errc::failure);
        node.path = std::move(result.value());
        
        // Parse terminator.
        if (ctx.current.type != leaf::dc_terminator)
            // TODO: return appropriate error code.
            return std::unexpected(errc::failure);
        ctx.next_token();
        return node;
    }
};

template<detail::LexicalAnalyzer Lexer>
struct package_parser final {
    static ast::node
    parse(state<Lexer>& ctx) noexcept {
        return nullptr;
    }
};

} // namespace cherry::syn
} // namespace cherry
