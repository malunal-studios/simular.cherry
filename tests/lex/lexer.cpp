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
    constexpr strview_t k_comment = "# Test Comment";
    constexpr strview_t k_mlstr = "\"\"\"ML string\ntest\"\"\"";
    constexpr strview_t k_litstr = "\"Hello, World!\"";

    static vec_t<lex::token>
    k_expected_tokens {
        lex::token{ .lexeme = "using",   .type = leaf::kw_using,      .line = 0, .column =  0 },
        lex::token{ .lexeme = "std",     .type = leaf::identifier,    .line = 0, .column =  6 },
        lex::token{ .lexeme = ";",       .type = leaf::dc_terminator, .line = 0, .column =  9 },
        lex::token{ .lexeme = "module",  .type = leaf::kw_module,     .line = 1, .column =  0 },
        lex::token{ .lexeme = "sample",  .type = leaf::identifier,    .line = 1, .column =  7 },
        lex::token{ .lexeme = ".",       .type = leaf::op_access,     .line = 1, .column = 13 },
        lex::token{ .lexeme = "hello",   .type = leaf::identifier,    .line = 1, .column = 14 },
        lex::token{ .lexeme = ";",       .type = leaf::dc_terminator, .line = 1, .column = 19 },
        lex::token{ .lexeme = k_comment, .type = leaf::comment,       .line = 3, .column =  0 },
        lex::token{ .lexeme = "var",     .type = leaf::kw_var,        .line = 4, .column =  0 },
        lex::token{ .lexeme = "mystr",   .type = leaf::identifier,    .line = 4, .column =  4 },
        lex::token{ .lexeme = ":",       .type = leaf::dc_colon,      .line = 4, .column =  9 },
        lex::token{ .lexeme = "string",  .type = leaf::kw_string,     .line = 4, .column = 11 },
        lex::token{ .lexeme = "=",       .type = leaf::op_assign,     .line = 4, .column = 18 },
        lex::token{ .lexeme = k_mlstr,   .type = leaf::lv_ml_string,  .line = 4, .column = 20 },
        lex::token{ .lexeme = ";",       .type = leaf::dc_terminator, .line = 5, .column =  7 },
        lex::token{ .lexeme = "entry",   .type = leaf::identifier,    .line = 6, .column =  0 },
        lex::token{ .lexeme = "(",       .type = leaf::dc_lparen,     .line = 6, .column =  5 },
        lex::token{ .lexeme = "args",    .type = leaf::identifier,    .line = 6, .column =  6 },
        lex::token{ .lexeme = ":",       .type = leaf::dc_colon,      .line = 6, .column = 10 },
        lex::token{ .lexeme = "...",     .type = leaf::op_ellipsis,   .line = 6, .column = 12 },
        lex::token{ .lexeme = "string",  .type = leaf::kw_string,     .line = 6, .column = 15 },
        lex::token{ .lexeme = ")",       .type = leaf::dc_rparen,     .line = 6, .column = 21 },
        lex::token{ .lexeme = ":",       .type = leaf::dc_colon,      .line = 6, .column = 23 },
        lex::token{ .lexeme = "void",    .type = leaf::kw_void,       .line = 6, .column = 25 },
        lex::token{ .lexeme = "{",       .type = leaf::dc_lbrace,     .line = 6, .column = 30 },
        lex::token{ .lexeme = "console", .type = leaf::identifier,    .line = 7, .column =  4 },
        lex::token{ .lexeme = ".",       .type = leaf::op_access,     .line = 7, .column = 11 },
        lex::token{ .lexeme = "print",   .type = leaf::identifier,    .line = 7, .column = 12 },
        lex::token{ .lexeme = "(",       .type = leaf::dc_lparen,     .line = 7, .column = 17 },
        lex::token{ .lexeme = k_litstr,  .type = leaf::lv_raw_string, .line = 7, .column = 18 },
        lex::token{ .lexeme = ")",       .type = leaf::dc_rparen,     .line = 7, .column = 33 },
        lex::token{ .lexeme = ";",       .type = leaf::dc_terminator, .line = 7, .column = 34 },
        lex::token{ .lexeme = "}",       .type = leaf::dc_rbrace,     .line = 8, .column =  0 }
    };

    lexer l;
    lex::state state;
    state.code =
R"(using std;
module sample.hello;

# Test Comment
var mystr: string = """ML string
test""";
entry(args: ...string) : void {
    console.print("Hello, World!");
})";
    
    for (auto index = 0; index < k_expected_tokens.size(); index++)
        ASSERT_EQ(k_expected_tokens[index], l.tokenize(state).value());
}