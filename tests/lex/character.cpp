#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace cherry;


TEST(LexCharacterTests, litmus_returns_true_when_provided_correct_input) {
    lex::state state;
    state.code = "\'a\'";

    lex::character_rule rule;
    ASSERT_TRUE(rule.litmus(state.remaining_source()));
}


TEST(LexCharacterTests, litmus_returns_false_when_provided_incorrect_input) {
    lex::state state;
    state.code = "\"a\"";

    lex::character_rule rule;
    ASSERT_FALSE(rule.litmus(state.remaining_source()));
}


TEST(LexCharacterTests, tokenize_can_read_single_character) {
    lex::state state;
    state.code = "\'a\'";

    lex::character_rule rule;
    lex::token expected {
        .lexeme = "\'a\'",
        .type = lex::leaf::lv_character,
        .line = 0,
        .column = 0
    };

    ASSERT_EQ(expected, rule.tokenize(state).value());
}


TEST(LexCharacterTests, tokenize_can_read_escape_character) {
    lex::state state;
    state.code = "\'\\n\'";

    lex::character_rule rule;
    lex::token expected {
        .lexeme = "\'\\n\'",
        .type = lex::leaf::lv_character,
        .line = 0,
        .column = 0
    };

    ASSERT_EQ(expected, rule.tokenize(state).value());
}


TEST(LexCharacterTests, tokenize_can_read_unicode_character) {
    static vec_t<strview_t>
    k_unicode_characters {
        "\'\\uB\'",
        "\'\\uBe\'",
        "\'\\uBee\'",
        "\'\\uBeeF\'"
    };

    for (auto unicode : k_unicode_characters) {
        lex::state state;
        state.code = unicode;

        lex::character_rule rule;
        lex::token expected {
            .lexeme = unicode,
            .type = lex::leaf::lv_character,
            .line = 0,
            .column = 0
        };

        ASSERT_EQ(expected, rule.tokenize(state).value());
    }
}


TEST(LexCharacterTests, tokenize_produces_errc_invalid_character) {
    static vec_t<strview_t>
    k_invalid_characters {
        "\'",
        "\'a",
        "\'\\",
        "\'\\\'"
    };

    for (auto character : k_invalid_characters) {
        lex::state state;
        state.code = character;

        lex::character_rule rule;
        ASSERT_EQ(lex::errc::invalid_character, rule.tokenize(state).error());
    }
}


TEST(LexCharacterTests, tokenize_produces_errc_invalid_unicode) {
    static vec_t<strview_t>
    k_invalid_unicode {
        "\'\\u",
        "\'\\uN",
        "\'\\uBEEF",
        "\'\\uDEADBEEF\'"
    };

    for (auto unicode : k_invalid_unicode) {
        lex::state state;
        state.code = unicode;

        lex::character_rule rule;
        ASSERT_EQ(lex::errc::invalid_unicode, rule.tokenize(state).error());
    }
}
