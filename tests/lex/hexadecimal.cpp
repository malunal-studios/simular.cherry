#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace cherry;


TEST(LexHexadecimalTests, litmus_returns_true_when_provided_correct_input) {
    lex::state state;
    state.code = "0xDEADbeef";

    lex::hexadecimal_rule rule;
    ASSERT_TRUE(rule.litmus(state.remaining_source()));
}


TEST(LexHexadecimalTests, litmus_returns_false_when_provided_incorrect_input) {
    lex::state state;
    state.code = "11110000";

    lex::hexadecimal_rule rule;
    ASSERT_FALSE(rule.litmus(state.remaining_source()));
}


TEST(LexHexadecimalTests, tokenize_produces_correct_result) {
    lex::state state;
    state.code = "0xDEADbeef";

    lex::hexadecimal_rule rule;
    lex::token expected {
        .lexeme = "0xDEADbeef",
        .type = lex::token_type::lv_signed,
        .line = 0,
        .column = 0
    };
    
    ASSERT_EQ(expected, rule.tokenize(state).value());
}


TEST(LexHexadecimalTests, tokenize_produces_errc_invalid_hexadecimal) {
    lex::state state;
    state.code = "0x";

    lex::hexadecimal_rule rule;
    ASSERT_EQ(lex::errc::invalid_hexadecimal, rule.tokenize(state).error());
}
