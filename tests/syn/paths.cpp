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
    ASSERT_EQ(syn::errc::expected_identifier, parser.parse(state).error());
}


TEST(SynPathsTests, segment_parser_correctly_parses_primitives) {
    struct segment_mock_lexer final {
        static lex::result
        tokenize(lex::state& ctx) noexcept {
            static int8_t k_index{0};
            static vec_t<lex::token> k_tokens {
                lex::token{ .lexeme = "bool",   .type = leaf::kw_bool,   .line = 0, .column = 0 },
                lex::token{ .lexeme = "char",   .type = leaf::kw_char,   .line = 0, .column = 0 },
                lex::token{ .lexeme = "int8",   .type = leaf::kw_int8,   .line = 0, .column = 0 },
                lex::token{ .lexeme = "int16",  .type = leaf::kw_int16,  .line = 0, .column = 0 },
                lex::token{ .lexeme = "int32",  .type = leaf::kw_int32,  .line = 0, .column = 0 },
                lex::token{ .lexeme = "int64",  .type = leaf::kw_int64,  .line = 0, .column = 0 },
                lex::token{ .lexeme = "uint8",  .type = leaf::kw_uint8,  .line = 0, .column = 0 },
                lex::token{ .lexeme = "uint16", .type = leaf::kw_uint16, .line = 0, .column = 0 },
                lex::token{ .lexeme = "uint32", .type = leaf::kw_uint32, .line = 0, .column = 0 },
                lex::token{ .lexeme = "uint64", .type = leaf::kw_uint64, .line = 0, .column = 0 },
                lex::token{ .lexeme = "single", .type = leaf::kw_single, .line = 0, .column = 0 },
                lex::token{ .lexeme = "double", .type = leaf::kw_double, .line = 0, .column = 0 },
                lex::token{ .lexeme = "string", .type = leaf::kw_string, .line = 0, .column = 0 },
            };
            return k_tokens[k_index++];
        }
    };

    using mock_lexer = segment_mock_lexer;
    syn::segment_parser<mock_lexer> parser;
    syn::state<mock_lexer> state;

    // Read one!
    state.next_token();

    using basic = ast::segment::primitive;
    constexpr uint8_t limit = (uint8_t)basic::k_last_primitive;
    for (auto index = 0; index < limit; index++) {
        auto result = parser.parse(state).value();
        ASSERT_TRUE(std::holds_alternative<basic>(result.variant));
        ASSERT_EQ((basic)index, std::get<basic>(result.variant));
    }
}


TEST(SynPathsTests, path_expr_parser_correctly_parses_nongeneric_path) {
    struct path_expr_mock_lexer final {
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

    using mock_lexer = path_expr_mock_lexer;
    syn::path_expr_parser<mock_lexer> parser;
    syn::state<mock_lexer> state;

    // Read one.
    state.next_token();
    
    auto expected = ast::path_expr{};
    expected.segments.emplace_back(ast::segment{
        .variant = ast::segment::generic{ .name = "std" }
    });
    expected.segments.emplace_back(ast::segment{
        .variant = ast::segment::generic{ .name = "io" }
    });
    expected.segments.emplace_back(ast::segment{
        .variant = ast::segment::generic{ .name = "file" }
    });

    ASSERT_EQ(expected, parser.parse(state).value());
}


// TEST(SynPathsTests, path_expr_parser_correctly_parses_generic_path) {

// }