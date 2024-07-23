#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace cherry;

static int32_t k_rule_call_count;
struct test_rule1 final {
    bool
    litmus(strview_t) const noexcept {
        k_rule_call_count++;
        return false;
    }

    lex::result
    tokenize(lex::state&) noexcept {
        return lex::token {};
    }
};

struct test_rule2 final {
    bool
    litmus(strview_t) const noexcept {
        k_rule_call_count++;
        return false;
    }

    lex::result
    tokenize(lex::state&) noexcept {
        return lex::token {};
    }
};


TEST(LexLexerTests, lexical_analyer_can_iterate_multiple_rules) {
    using lexer = lexical_analyzer<test_rule1, test_rule2>;
    
    lex::state state;
    k_rule_call_count = 0;
    ASSERT_EQ(lex::errc::not_my_token, lexer().tokenize(state).error());
    ASSERT_EQ(2, k_rule_call_count);
}


TEST(LexLexerTests, lexical_analyzer_integration_test) {
    static vec_t<lex::token>
    k_expected_tokens {
        lex::token{ .lexeme = "using",             .type = leaf::kw_using,      .line = 0, .column =  0 },
        lex::token{ .lexeme = "std",               .type = leaf::identifier,    .line = 0, .column =  6 },
        lex::token{ .lexeme = ";",                 .type = leaf::dc_terminator, .line = 0, .column =  9 },
        lex::token{ .lexeme = "module",            .type = leaf::kw_module,     .line = 1, .column =  0 },
        lex::token{ .lexeme = "sample",            .type = leaf::identifier,    .line = 1, .column =  7 },
        lex::token{ .lexeme = ".",                 .type = leaf::op_access,     .line = 1, .column = 13 },
        lex::token{ .lexeme = "hello",             .type = leaf::identifier,    .line = 1, .column = 14 },
        lex::token{ .lexeme = ";",                 .type = leaf::dc_terminator, .line = 1, .column = 19 },
        lex::token{ .lexeme = "entry",             .type = leaf::identifier,    .line = 2, .column =  0 },
        lex::token{ .lexeme = "(",                 .type = leaf::dc_lparen,     .line = 2, .column =  5 },
        lex::token{ .lexeme = "args",              .type = leaf::identifier,    .line = 2, .column =  6 },
        lex::token{ .lexeme = ":",                 .type = leaf::dc_colon,      .line = 2, .column = 10 },
        lex::token{ .lexeme = "...",               .type = leaf::op_ellipsis,   .line = 2, .column = 12 },
        lex::token{ .lexeme = "string",            .type = leaf::kw_string,     .line = 2, .column = 15 },
        lex::token{ .lexeme = ")",                 .type = leaf::dc_rparen,     .line = 2, .column = 21 },
        lex::token{ .lexeme = ":",                 .type = leaf::dc_colon,      .line = 2, .column = 23 },
        lex::token{ .lexeme = "void",              .type = leaf::kw_void,       .line = 2, .column = 25 },
        lex::token{ .lexeme = "{",                 .type = leaf::dc_lbrace,     .line = 2, .column = 30 },
        lex::token{ .lexeme = "console",           .type = leaf::identifier,    .line = 3, .column =  4 },
        lex::token{ .lexeme = ".",                 .type = leaf::op_access,     .line = 3, .column = 11 },
        lex::token{ .lexeme = "print",             .type = leaf::identifier,    .line = 3, .column = 12 },
        lex::token{ .lexeme = "(",                 .type = leaf::dc_lparen,     .line = 3, .column = 17 },
        lex::token{ .lexeme = "\"Hello, World!\"", .type = leaf::lv_raw_string, .line = 3, .column = 18 },
        lex::token{ .lexeme = ")",                 .type = leaf::dc_rparen,     .line = 3, .column = 33 },
        lex::token{ .lexeme = ";",                 .type = leaf::dc_terminator, .line = 3, .column = 34 },
        lex::token{ .lexeme = "}",                 .type = leaf::dc_rbrace,     .line = 4, .column =  0 }
    };

    lexer l;
    lex::state state;
    state.code = "using std;\n"
                 "module sample.hello;\n"
                 "entry(args: ...string) : void {\n"
                 "    console.print(\"Hello, World!\");\n"
                 "}";
    
    for (auto index = 0; index < k_expected_tokens.size(); index++)
        ASSERT_EQ(k_expected_tokens[index], l.tokenize(state).value());
}