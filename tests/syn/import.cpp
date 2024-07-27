#include <gtest/gtest.h>
#include "parser.hpp"


using namespace cherry;


TEST(SynPathsTests, import_parser_correctly_parses) {
    struct import_mock_lexer final {
        static lex::result
        tokenize(lex::state& ctx) noexcept {
            static int8_t k_index{0};
            static vec_t<lex::token> k_tokens {
                lex::token{ .lexeme = "using", .type = leaf::kw_using,      .line = 0, .column = 0 },
                lex::token{ .lexeme = "std",   .type = leaf::identifier,    .line = 0, .column = 6 },
                lex::token{ .lexeme = ";",     .type = leaf::dc_terminator, .line = 0, .column = 9 },
            };
            return k_tokens[k_index++];
        }
    };

    using mock_lexer = import_mock_lexer;
    syn::import_parser<mock_lexer> parser;
    syn::state<mock_lexer> state;

    // Read one token because that's how it would come into the parser.
    state.next_token();

    auto expected = ast::import{};
    expected.path = ast::simple_path{};
    expected.path.segments.push_back("std");
    ASSERT_EQ(expected, parser.parse(state).value());
}