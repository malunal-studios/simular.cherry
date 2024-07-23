#include <gtest/gtest.h>
#include "temps.hpp"


TEST(SynGrammarTests, collects_production_rules_correctly) {
    static prod_sets_t expected {
        { (int16_t)roots::E,  { (int16_t)roots::T, (int16_t)roots::EP } },
        { (int16_t)roots::EP, { (int16_t)leaves::add, (int16_t)roots::T, (int16_t)roots::EP } },
        { (int16_t)roots::EP, { (int16_t)k_epsilon } },
        { (int16_t)roots::T,  { (int16_t)roots::F, (int16_t)roots::TP } },
        { (int16_t)roots::TP, { (int16_t)leaves::mul, (int16_t)roots::F, (int16_t)roots::TP } },
        { (int16_t)roots::TP, { (int16_t)k_epsilon } },
        { (int16_t)roots::F,  { (int16_t)leaves::lparen, (int16_t)roots::E, (int16_t)leaves::rparen } },
        { (int16_t)roots::F,  { (int16_t)leaves::ident } }
    };

    ASSERT_EQ(expected, test_grammar::prod_sets());
}


TEST(SynGrammarTests, collects_firsts_sets_correctly) {
    static symb_sets_t expected {
        { (int16_t)roots::E,  { (int16_t)leaves::ident, (int16_t)leaves::lparen } },
        { (int16_t)roots::EP, { (int16_t)k_epsilon, (int16_t)leaves::add } },
        { (int16_t)roots::T,  { (int16_t)leaves::ident, (int16_t)leaves::lparen } },
        { (int16_t)roots::TP, { (int16_t)k_epsilon, (int16_t)leaves::mul } },
        { (int16_t)roots::F,  { (int16_t)leaves::ident, (int16_t)leaves::lparen } },
    };

    ASSERT_EQ(expected, test_grammar::first_sets());
}


TEST(SynGrammarTests, collects_follows_sets_correctly) {
    static symb_sets_t expected {
        { (int16_t)roots::E,  { (int16_t)k_final, (int16_t)leaves::rparen } },
        { (int16_t)roots::EP, { (int16_t)k_final, (int16_t)leaves::rparen } },
        { (int16_t)roots::T,  { (int16_t)k_final, (int16_t)leaves::add, (int16_t)leaves::rparen } },
        { (int16_t)roots::TP, { (int16_t)k_final, (int16_t)leaves::add, (int16_t)leaves::rparen } },
        { (int16_t)roots::F,  { (int16_t)k_final, (int16_t)leaves::add, (int16_t)leaves::mul, (int16_t)leaves::rparen } },
    };

    ASSERT_EQ(expected, test_grammar::follow_sets());
}


TEST(SynGrammarTests, cherry_grammar_firsts_is_correct) {
    static symb_sets_t expected {
        { root::document, { k_epsilon, leaf::kw_using, leaf::kw_module, leaf::kw_var, leaf::kw_object } },
        { root::docbody,  { k_epsilon, leaf::kw_module, leaf::kw_var, leaf::kw_object } },
        { root::includes, { k_epsilon, leaf::kw_using } },
        { root::import,   { leaf::kw_using  } },
        { root::module,   { leaf::kw_module } },
        { root::variable, { leaf::kw_var    } },
        { root::object,   { leaf::kw_object } },
        { root::objbody,  { leaf::dc_lbrace } },
        { root::objcont,  { k_epsilon } },
        { root::idchain,  { leaf::identifier } },
        { root::accchain, { k_epsilon, leaf::op_access } },
        { root::vartype,  {
            leaf::identifier,
            leaf::kw_bool,
            leaf::kw_char,
            leaf::kw_int8,
            leaf::kw_int16,
            leaf::kw_int32,
            leaf::kw_int64,
            leaf::kw_uint8,
            leaf::kw_uint16,
            leaf::kw_uint32,
            leaf::kw_uint64,
            leaf::kw_single,
            leaf::kw_double,
            leaf::kw_string,
            leaf::kw_void,
        } }
    };

    const auto& actual = grammar::first_sets();
    ASSERT_EQ(expected, actual) << actual;
}
