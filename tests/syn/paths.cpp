#include <gtest/gtest.h>
#include "parser.hpp"


using namespace cherry;


TEST(SynPathsTests, simple_path_parser_correctly_parses) {
    struct simple_paths_mock_lexer final {
        static lex::result
        tokenize(lex::state& ctx) noexcept {
            static int8_t k_index{0};
            static vec_t<lex::token> k_tokens {
                lex::token{ .lexeme = "std",  .type = leaf::identifier, .line = 0, .column =  0 },
                lex::token{ .lexeme = ".",    .type = leaf::op_access,  .line = 0, .column =  3 },
                lex::token{ .lexeme = "io",   .type = leaf::identifier, .line = 0, .column =  4 },
                lex::token{ .lexeme = ".",    .type = leaf::op_access,  .line = 0, .column =  6 },
                lex::token{ .lexeme = "file", .type = leaf::identifier, .line = 0, .column =  7 },
                lex::token{ .lexeme = "$",    .type = leaf::eos,        .line = 0, .column = 11 }
            };
            return k_tokens[k_index++];
        }
    };

    using mock_lexer = simple_paths_mock_lexer;
    syn::simple_path_parser<mock_lexer> parser;
    syn::state<mock_lexer> state;

    // Read one token because that's how it would come into the parser.
    state.next_token();

    auto expected = ast::simple_path{};
    expected.segments.push_back("std");
    expected.segments.push_back("io");
    expected.segments.push_back("file");
    ASSERT_EQ(expected, parser.parse(state).value());
}


TEST(SynPathsTests, simple_path_parser_returns_parse_error) {
    struct simple_paths_mock_lexer final {
        static lex::result
        tokenize(lex::state& ctx) noexcept {
            static int8_t k_index{0};
            static vec_t<lex::token> k_tokens {
                lex::token{ .lexeme = "std", .type = leaf::identifier, .line = 0, .column =  0 },
                lex::token{ .lexeme = ".",   .type = leaf::op_access,  .line = 0, .column =  3 },
                lex::token{ .lexeme = ".",   .type = leaf::op_access,  .line = 0, .column =  4 },
            };
            return k_tokens[k_index++];
        }
    };

    using mock_lexer = simple_paths_mock_lexer;
    syn::simple_path_parser<mock_lexer> parser;
    syn::state<mock_lexer> state;

    // Read one token because that's how it would come into the parser.
    state.next_token();
    ASSERT_EQ(syn::errc::failure, parser.parse(state).error());
}