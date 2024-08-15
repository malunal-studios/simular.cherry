#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace simular::cherry;


TEST(LexOctalTests, litmus_returns_true_when_provided_correct_input) {
    lex::state state;
    state.code = "0304";

    lex::octal_rule rule;
    ASSERT_TRUE(rule.litmus(state.remaining_source()));
}


TEST(LexOctalTests, litmus_returns_false_when_provided_incorrect_input) {
    lex::state state;
    state.code = "1234";

    lex::octal_rule rule;
    ASSERT_FALSE(rule.litmus(state.remaining_source()));
}


TEST(LexOctalTests, tokenize_produces_correct_result) {
    lex::state state;
    state.code = "0304";

    lex::octal_rule rule;
    lex::token expected {
        .lexeme = "0304",
        .type = leaf::lv_signed,
        .line = 0,
        .column = 0
    };

    ASSERT_EQ(expected, rule.tokenize(state).value());
}


// This case is likely to never happen but we should make sure it works
// as a sanity check.
TEST(LexOctalTests, tokenize_produces_errc_invalid_octal) {
    lex::state state;
    state.code = "0";

    lex::octal_rule rule;
    ASSERT_EQ(lex::errc::invalid_octal, rule.tokenize(state).error());
}
