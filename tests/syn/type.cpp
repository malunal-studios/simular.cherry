#include <gtest/gtest.h>
#include "parser.hpp"


using namespace cherry;


TEST(SynTypeTests, type_parser_correctly_parses_raw) {
    struct type_mock_lexer final {
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

    using mock_lexer = type_mock_lexer;
    syn::type_parser<mock_lexer> parser;
    syn::state<mock_lexer> state;

    // Read one token because that's how it would come into the parser.
    state.next_token();

    auto expected = std::make_unique<ast::type>();
    auto segm = std::make_unique<ast::segment::generic>();
    segm->name = "std";
    expected->segments.emplace_back(std::move(segm));

    segm = std::make_unique<ast::segment::generic>();
    segm->name = "io";
    expected->segments.emplace_back(std::move(segm));

    segm = std::make_unique<ast::segment::generic>();
    segm->name = "file";
    expected->segments.emplace_back(std::move(segm));

    ASSERT_EQ(*expected, *parser.parse(state).value());
}


TEST(SynTypeTests, type_parser_correctly_parses_fn) {
    struct type_mock_lexer final {
        static lex::result
        tokenize(lex::state& ctx) noexcept {
            static int8_t k_index{0};
            static vec_t<lex::token> k_tokens {
                lex::token{ .lexeme = "std",     .type = leaf::identifier, .line = 0, .column =  0 },
                lex::token{ .lexeme = ".",       .type = leaf::op_access,  .line = 0, .column =  3 },
                lex::token{ .lexeme = "io",      .type = leaf::identifier, .line = 0, .column =  4 },
                lex::token{ .lexeme = ".",       .type = leaf::op_access,  .line = 0, .column =  6 },
                lex::token{ .lexeme = "console", .type = leaf::identifier, .line = 0, .column =  7 },
                lex::token{ .lexeme = ".",       .type = leaf::op_access,  .line = 0, .column = 14 },
                lex::token{ .lexeme = "write",   .type = leaf::identifier, .line = 0, .column = 15 },
                lex::token{ .lexeme = "(",       .type = leaf::dc_lparen,  .line = 0, .column = 20 },
                lex::token{ .lexeme = "string",  .type = leaf::kw_string,  .line = 0, .column = 21 },
                lex::token{ .lexeme = ")",       .type = leaf::dc_rparen,  .line = 0, .column = 27 },
                lex::token{ .lexeme = ":",       .type = leaf::dc_colon,   .line = 0, .column = 29 },
                lex::token{ .lexeme = "void",    .type = leaf::kw_void,    .line = 0, .column = 31 }
            };
            return k_tokens[k_index++];
        }
    };

    using mock_lexer = type_mock_lexer;
    syn::type_parser<mock_lexer> parser;
    syn::state<mock_lexer> state;

    // Read one token because that's how it would come into the parser.
    state.next_token();

    auto expected = std::make_unique<ast::type::fn>();
    auto segm = std::make_unique<ast::segment::generic>();
    segm->name = "std";
    expected->segments.emplace_back(std::move(segm));

    segm = std::make_unique<ast::segment::generic>();
    segm->name = "io";
    expected->segments.emplace_back(std::move(segm));

    segm = std::make_unique<ast::segment::generic>();
    segm->name = "console";
    expected->segments.emplace_back(std::move(segm));

    segm = std::make_unique<ast::segment::generic>();
    segm->name = "write";
    expected->segments.emplace_back(std::move(segm));

    auto input = std::make_unique<ast::type>();
    auto prim  = std::make_unique<ast::segment::primitive>(ast::segment::primitive::p_string);
    input->segments.emplace_back(std::move(prim));
    expected->inputs.emplace_back(std::move(input));
    
    auto output = std::make_unique<ast::type>();
    prim = std::make_unique<ast::segment::primitive>(ast::segment::primitive::p_void);
    output->segments.emplace_back(std::move(prim));
    expected->output = std::move(output);

    ASSERT_EQ(*expected, *parser.parse(state).value());
}


TEST(SynTypeTests, type_parser_correctly_parses_arr) {
    FAIL() << "Unimplemented";
}


TEST(SynTypeTests, type_parser_correctly_parses_ref) {
    struct type_mock_lexer final {
        static lex::result
        tokenize(lex::state& ctx) noexcept {
            static int8_t k_index{0};
            static vec_t<lex::token> k_tokens {
                lex::token{ .lexeme = "int32", .type = leaf::kw_int32,  .line = 0, .column =  0 },
                lex::token{ .lexeme = "*",     .type = leaf::op_mul,    .line = 0, .column =  5 },
                lex::token{ .lexeme = "*",     .type = leaf::op_mul,    .line = 0, .column =  6 },
                lex::token{ .lexeme = "&",     .type = leaf::op_bitand, .line = 0, .column =  7 },
                lex::token{ .lexeme = "&",     .type = leaf::op_bitand, .line = 0, .column =  8 },
                lex::token{ .lexeme = "*",     .type = leaf::op_mul,    .line = 0, .column =  9 },
                lex::token{ .lexeme = "&",     .type = leaf::op_bitand, .line = 0, .column = 10 },
            };
            return k_tokens[k_index++];
        }
    };

    using mock_lexer = type_mock_lexer;
    syn::type_parser<mock_lexer> parser;
    syn::state<mock_lexer> state;

    // Read one token because that's how it would come into the parser.
    state.next_token();

    auto expected = std::make_unique<ast::type::ref>();
    auto segm = std::make_unique<ast::segment::primitive>(ast::segment::primitive::p_int32);
    expected->segments.emplace_back(std::move(segm));
    expected->depth.emplace_back(1);
    expected->depth.emplace_back(1);
    expected->depth.emplace_back(0);
    expected->depth.emplace_back(0);
    expected->depth.emplace_back(1);
    expected->depth.emplace_back(0);

    ASSERT_EQ(*expected, *parser.parse(state).value());
}