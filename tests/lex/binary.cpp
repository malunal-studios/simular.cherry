#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace cherry;


TEST(LexBinaryTests, litmus_returns_true_when_provided_correct_input) {
    lex::state state;
    state.code = "0b00001111";

    lex::binary_rule rule;
    ASSERT_TRUE(rule.litmus(state.remaining_source()));
}


TEST(LexBinaryTests, litmus_returns_false_when_provided_incorrect_input) {
    lex::state state;
    state.code = "11110000";

    lex::binary_rule rule;
    ASSERT_FALSE(rule.litmus(state.remaining_source()));
}


TEST(LexBinaryTests, tokenize_produces_correct_result) {
    lex::state state;
    state.code = "0b01010101";

    lex::binary_rule rule;
    lex::token expected {
        .lexeme = "0b01010101",
        .type = lex::leaf::lv_signed,
        .line = 0,
        .column = 0
    };
    
    ASSERT_EQ(expected, rule.tokenize(state).value());
}


TEST(LexBinaryTests, tokenize_produces_errc_invalid_binary) {
    lex::state state;
    state.code = "0b";

    lex::binary_rule rule;
    ASSERT_EQ(lex::errc::invalid_binary, rule.tokenize(state).error());
}
