#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace cherry;


TEST(LexCommentTests, litmus_returns_true_when_provided_correct_input) {
    lex::state state;
    state.code = "# This is a comment";
    
    lex::comment_rule rule;
    ASSERT_TRUE(rule.litmus(state.remaining_source()));
}


TEST(LexCommentTests, litmus_returns_false_when_provided_incorrect_input) {
    lex::state state;
    state.code = "This is not a comment";
    
    lex::comment_rule rule;
    ASSERT_FALSE(rule.litmus(state.remaining_source()));
}


TEST(LexCommentTests, tokenize_produces_correct_result) {
    lex::state state;
    state.code = "# Comment";
    
    lex::comment_rule rule;
    lex::token expected {
        .lexeme = "# Comment",
        .type = lex::leaf::comment,
        .line = 0,
        .column = 0
    };
    
    ASSERT_EQ(expected, rule.tokenize(state).value());
}