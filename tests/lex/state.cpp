#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace simular::cherry;


TEST(LexStateTests, read_src_char_produces_correct_char) {
    lex::state state;
    state.code = "TEST";
    ASSERT_EQ('T', state.read_src_char());
}


TEST(LexStateTests, curr_src_char_produces_correct_char) {
    lex::state state;
    state.code = "TEST";
    ASSERT_EQ('T', state.curr_src_char());

    state.read_src_char();
    ASSERT_EQ('E', state.curr_src_char());
}


TEST(LexStateTests, prev_src_char_produces_correct_char) {
    lex::state state;
    state.code = "TEST";
    state.read_src_char();
    ASSERT_EQ('T', state.prev_src_char());
}


TEST(LexStateTests, next_src_char_produces_correct_char) {
    lex::state state;
    state.code = "TEST";
    ASSERT_EQ('E', state.next_src_char());

    state.read_src_char();
    ASSERT_EQ('S', state.next_src_char());
}


TEST(LexStateTests, remaining_source_produces_correct_string) {
    lex::state state;
    state.code = "TEST";
    state.index = 2;
    ASSERT_EQ("ST", state.remaining_source());
}


TEST(LexStateTests, extract_token_produces_correct_result) {
    lex::state state;
    state.code = "TEST";
    state.start_token();
    for (auto index = 0; index < state.code.size(); index++)
        state.read_src_char();

    auto expected = lex::token {
        .lexeme = "TEST",
        .type   = leaf::unknown,
        .line   = 0,
        .column = 0
    };

    auto actual = state.extract_token(leaf::unknown);
    ASSERT_EQ(expected, actual);
}
