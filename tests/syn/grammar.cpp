#include <gtest/gtest.h>
#include "grammar.hpp"


using namespace cherry;


enum struct leaves : int16_t {
    ident, add, mul, lparen, rparen
};

enum struct roots : int16_t {
    start_from = detail::leaf_upper_limit(),
    E, EP, T, TP, F
};

struct expr_rule final {
    prod_sets_t
    productions() const noexcept {
        return prod_sets_t {
            { (int16_t)roots::E, { (int16_t)roots::T, (int16_t)roots::EP } }
        };
    }
};

struct expr_prime_rule final {
    prod_sets_t
    productions() const noexcept {
        return prod_sets_t {
            { (int16_t)roots::EP, { (int16_t)leaves::add, (int16_t)roots::T, (int16_t)roots::EP } },
            { (int16_t)roots::EP, { (int16_t)k_epsilon } }
        };
    }
};

struct term_rule final {
    prod_sets_t
    productions() const noexcept {
        return prod_sets_t {
            { (int16_t)roots::T, { (int16_t)roots::F, (int16_t)roots::TP } }
        };
    };
};

struct term_prime_rule final {
    prod_sets_t
    productions() const noexcept {
        return prod_sets_t {
            { (int16_t)roots::TP, { (int16_t)leaves::mul, (int16_t)roots::F, (int16_t)roots::TP } },
            { (int16_t)roots::TP, { (int16_t)k_epsilon } }
        };
    }
};

struct fact_rule final {
    prod_sets_t
    productions() const noexcept {
        return prod_sets_t {
            { (int16_t)roots::F, { (int16_t)leaves::lparen, (int16_t)roots::E, (int16_t)leaves::rparen } },
            { (int16_t)roots::F, { (int16_t)leaves::ident } }
        };
    }
};


using test_grammar = lr1_grammar<
    expr_rule,
    expr_prime_rule,
    term_rule,
    term_prime_rule,
    fact_rule
>;


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