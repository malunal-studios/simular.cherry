#pragma once
#include "common.hpp"
#include "lex/leaf.hpp"
#include "lex/error.hpp"
#include "lex/token.hpp"
#include "lex/state.hpp"
#include "lex/detail.hpp"
#include "lex/comment.hpp"
#include "lex/keyword.hpp"
#include "lex/binary.hpp"
#include "lex/octal.hpp"
#include "lex/decimal.hpp"
#include "lex/hexadecimal.hpp"
#include "lex/character.hpp"
#include "lex/string.hpp"
#include "lex/operator.hpp"


namespace simular::cherry {

/// @brief   Defines a generic lexer which operates based on the definitions of
///          the rules provided to it.
/// @tparam  ...Rules A variadic set of lexical analysis rules which provide the
///          lexer with the ability to tokenize a given source code input string.
/// @remarks The order of the rules matters! The lexical analyzer will
///          automatically skip whitespace.
template<lex::detail::LexicalRule... Rules>
class lexical_analyzer final {
    static void
    skip_whitespace(lex::state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        while (std::isspace(ctx.curr_src_char()))
            ctx.read_src_char();
    }

    template<size_t Index = 0>
    static std::enable_if_t<Index == sizeof...(Rules), lex::result>
    try_tokenize(
        [[maybe_unused]] lex::state&           ctx,
        [[maybe_unused]] std::tuple<Rules...>& rules
    ) noexcept {
        return std::unexpected(lex::errc::not_my_token);
    }

    template<size_t Index = 0>
    static std::enable_if_t<Index < sizeof...(Rules), lex::result>
    try_tokenize(lex::state& ctx, std::tuple<Rules...>& rules) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        skip_whitespace(ctx);
        auto& rule = std::get<Index>(rules);
        return rule.litmus(ctx.remaining_source())
            ? rule.tokenize(ctx)
            : try_tokenize<Index + 1>(ctx, rules);
    }

public:
    /// @brief   Tokenizes the input as specified by the state.
    /// @param   ctx The information about the current lexical analysis.
    /// @returns The result of the tokenization; if the token not extracted it
    ///          will be an error code.
    static lex::result
    tokenize(lex::state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        auto t = std::make_tuple(Rules()...);
        return try_tokenize(ctx, t);
    }
};


/// @brief   Type definition for the `cherry` specific lexical analyzer.
/// @details This simply makes it easier to create an instance of the `cherry`
///          spcific lexical analyzer by type defining it with all of the rules
///          that cherry uses to tokenize a given input.
using lexer = lexical_analyzer<
    lex::comment_rule,
    lex::keyword_rule,
    lex::binary_rule,
    lex::octal_rule,
    lex::decimal_rule,
    lex::hexadecimal_rule,
    lex::character_rule,
    lex::string_rule,
    lex::operator_rule
>;


} // namespace simular::cherry