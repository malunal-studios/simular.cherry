#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace cherry;


TEST(LexDecimalTests, litmus_returns_true_when_provided_zero) {
    lex::state state;
    state.code = "0";

    lex::decimal_rule rule;
    ASSERT_TRUE(rule.litmus(state.remaining_source()));
}


TEST(LexDecimalTests, litmus_returns_true_when_provided_integers) {
    lex::state state;
    state.code = "1234";

    lex::decimal_rule rule;
    ASSERT_TRUE(rule.litmus(state.remaining_source()));
}


TEST(LexDecimalTests, litmus_returns_true_when_provided_floats) {
    lex::state state;
    state.code = "12.34";

    lex::decimal_rule rule;
    ASSERT_TRUE(rule.litmus(state.remaining_source()));
}


TEST(LexDecimalTests, litmus_returns_false_when_provided_incorrect_input) {
    static vec_t<strview_t>
    k_incorrect_inputs {
        "0123",
        "0xBEEF",
        "0b0101"
    };

    for (auto input : k_incorrect_inputs) {
        lex::state state;
        state.code = input;

        lex::decimal_rule rule;
        ASSERT_FALSE(rule.litmus(state.remaining_source()));
    }
}


TEST(LexDecimalTests, tokenize_can_read_zero_correctly) {
    lex::state state;
    state.code = "0";

    lex::decimal_rule rule;
    lex::token expected {
        .lexeme = "0",
        .type = lex::token_type::lv_signed,
        .line = 0,
        .column = 0
    };

    ASSERT_EQ(expected, rule.tokenize(state).value());
}


TEST(LexDecimalTests, tokenize_can_read_integers_correctly) {
    lex::state state;
    state.code = "1234";

    lex::decimal_rule rule;
    lex::token expected {
        .lexeme = "1234",
        .type = lex::token_type::lv_signed,
        .line = 0,
        .column = 0
    };

    ASSERT_EQ(expected, rule.tokenize(state).value());
}


TEST(LexDecimalTests, tokenize_can_read_floats_correctly) {
    lex::state state;
    state.code = "12.34";

    lex::decimal_rule rule;
    lex::token expected {
        .lexeme = "12.34",
        .type = lex::token_type::lv_decimal,
        .line = 0,
        .column = 0
    };

    ASSERT_EQ(expected, rule.tokenize(state).value());
}
