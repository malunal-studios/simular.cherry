#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace cherry;


TEST(LexStringTests, litmus_returns_true_when_provided_correct_input) {
    lex::state state;
    state.code = "\"My string\"";

    lex::string_rule rule;
    ASSERT_TRUE(rule.litmus(state.remaining_source()));
}


TEST(LexStringTests, litmus_returns_false_when_provided_incorrect_input) {
    lex::state state;
    state.code = "My String";
    
    lex::string_rule rule;
    ASSERT_FALSE(rule.litmus(state.remaining_source()));
}


TEST(LexStringTests, tokenize_produces_correct_result) {
    lex::state state;
    state.code = "\"My String\"";

    lex::string_rule rule;
    lex::token expected {
        .lexeme = "\"My String\"",
        .type = lex::token_type::lv_raw_string,
        .line = 0,
        .column = 0
    };

    ASSERT_EQ(expected, rule.tokenize(state).value());
}


TEST(LexStringTests, tokenize_produces_errc_invalid_raw_string) {
    static vec_t<strview_t>
    k_invalid_strings {
        "\"My String",
        "\"My String\n void",
    };

    for (auto mystring : k_invalid_strings) {
        lex::state state;
        state.code = mystring;

        lex::string_rule rule;
        ASSERT_EQ(lex::errc::invalid_raw_string, rule.tokenize(state).error());
    }
}
