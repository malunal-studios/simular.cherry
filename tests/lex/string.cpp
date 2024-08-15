#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace simular::cherry;


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


TEST(LexStringTests, tokenize_produces_correct_literal_result) {
    static vec_t<pair_t<strview_t, leaf>>
    k_valid_strings {
        { "\"My String\"",   leaf::lv_raw_string },
        { "\"My {String}\"", leaf::lv_int_string }
    };

    for (auto valid_pair : k_valid_strings) {
        lex::state state;
        state.code = valid_pair.first;

        lex::string_rule rule;
        lex::token expected {
            .lexeme = valid_pair.first,
            .type = valid_pair.second,
            .line = 0,
            .column = 0
        };
        
        ASSERT_EQ(expected, rule.tokenize(state).value());
    }
}


TEST(LexStringTests, tokenize_produces_correct_multiline_result) {
    static vec_t<pair_t<strview_t, leaf>>
    k_valid_strings {
        { "\"\"\"My \nString\"\"\"", leaf::lv_ml_string },
        { "\"\"\"My \n{String}\"\"\"", leaf::lv_mli_string }
    };

    for (auto valid_pair : k_valid_strings) {
        lex::state state;
        state.code = valid_pair.first;

        lex::string_rule rule;
        lex::token expected {
            .lexeme = valid_pair.first,
            .type = valid_pair.second,
            .line = 0,
            .column = 0
        };
        
        ASSERT_EQ(expected, rule.tokenize(state).value());
    }
}


TEST(LexStringTests, tokenize_produces_correct_raw_literal_result) {
    static vec_t<pair_t<strview_t, leaf>>
    k_valid_strings {
        // NOTE THE 'r'!
        { "r\"\"\"My \nString\"\"\"", leaf::lv_raw_string },
        { "r\"\"\"My \n{String}\"\"\"", leaf::lv_int_string }
    };

    for (auto valid_pair : k_valid_strings) {
        lex::state state;
        state.code = valid_pair.first;

        lex::string_rule rule;
        lex::token expected {
            .lexeme = valid_pair.first,
            .type = valid_pair.second,
            .line = 0,
            .column = 0
        };
        
        ASSERT_EQ(expected, rule.tokenize(state).value());
    }
}


TEST(LexStringTests, tokenize_produces_errc_invalid_multiline_string) {
    static vec_t<strview_t>
    k_invalid_strings {
        // Technically, this is actually acceptable.
        // The first two quotes constitutes a completed literal.
        // "\"\"My \nString\"\"\"",
        
        // Unacceptable
        "\"\"\"My \nString\"\"",
        "\"\"\"My \nString\"",
        "\"\"\"My \nString",
    };

    for (auto mystring : k_invalid_strings) {
        lex::state state;
        state.code = mystring;

        lex::string_rule rule;
        ASSERT_EQ(lex::errc::invalid_ml_string, rule.tokenize(state).error());
    }
}

TEST(LexStringTests, tokenize_produces_errc_invalid_raw_string) {
    static vec_t<strview_t>
    k_invalid_strings {
        "\"My String",
        "\"My String\n void",
        "r\"\"\"My \nString\"",
        "r\"\"\"My \nString\"\""
    };

    for (auto mystring : k_invalid_strings) {
        lex::state state;
        state.code = mystring;

        lex::string_rule rule;
        ASSERT_EQ(lex::errc::invalid_raw_string, rule.tokenize(state).error());
    }
}