#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace cherry;


TEST(LexOperatorTests, litmus_returns_true_when_provided_correct_input) {
    lex::state state;
    state.code = "+-*/%=.?~&|^<>!(){}[],;:";

    lex::operator_rule rule;
    for (auto index = 0; index < state.code.length(); index++) {
        ASSERT_TRUE(rule.litmus(state.remaining_source()));
        state.read_src_char();
    }
}


TEST(LexOperatorTests, litmus_returns_false_when_provided_incorrect_input) {
    lex::state state;
    state.code = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "abcdefghijklmnopqrstuvwxyz"
                 "0123456789`#@\\'\"";
    
    lex::operator_rule rule;
    for (auto index = 0; index < state.code.length(); index++) {
        ASSERT_FALSE(rule.litmus(state.remaining_source()));
        state.read_src_char();
    }
}


TEST(LexOperatorTests, tokenize_produces_correct_result) {
    static dict_t<strview_t, lex::token_type>
    k_operator_mapped_types {
        { "+",   lex::token_type::op_add        },
        { "-",   lex::token_type::op_sub        },
        { "*",   lex::token_type::op_mul        },
        { "/",   lex::token_type::op_div        },
        { "%",   lex::token_type::op_mod        },
        { "+=",  lex::token_type::op_add_eq     },
        { "-=",  lex::token_type::op_sub_eq     },
        { "*=",  lex::token_type::op_mul_eq     },
        { "/=",  lex::token_type::op_div_eq     },
        { "%=",  lex::token_type::op_mod_eq     },
        { "++",  lex::token_type::op_inc        },
        { "--",  lex::token_type::op_dec        },
        { "=",   lex::token_type::op_assign     },
        { ".",   lex::token_type::op_access     },
        { "?",   lex::token_type::op_ternary    },
        { "..",  lex::token_type::op_cascade    },
        { "...", lex::token_type::op_ellipsis   },
        { "~",   lex::token_type::op_bitnot     },
        { "&",   lex::token_type::op_bitand     },
        { "|",   lex::token_type::op_bitor      },
        { "^",   lex::token_type::op_bitxor     },
        { "<<",  lex::token_type::op_bitlsh     },
        { ">>",  lex::token_type::op_bitrsh     },
        { "~=",  lex::token_type::op_bitnot_eq  },
        { "&=",  lex::token_type::op_bitand_eq  },
        { "|=",  lex::token_type::op_bitor_eq   },
        { "^=",  lex::token_type::op_bitxor_eq  },
        { "<<=", lex::token_type::op_bitlsh_eq  },
        { ">>=", lex::token_type::op_bitrsh_eq  },
        { "!",   lex::token_type::op_lognot     },
        { "&&",  lex::token_type::op_logand     },
        { "||",  lex::token_type::op_logor      },
        { "<",   lex::token_type::op_logless    },
        { ">",   lex::token_type::op_logmore    },
        { "==",  lex::token_type::op_logequals  },
        { "!=",  lex::token_type::op_lognot_eq  },
        { "&&=", lex::token_type::op_logand_eq  },
        { "||=", lex::token_type::op_logor_eq   },
        { "<=",  lex::token_type::op_logless_eq },
        { ">=",  lex::token_type::op_logmore_eq },
    };

    for (auto pair : k_operator_mapped_types) {
        lex::state state;
        state.code = pair.first;

        lex::operator_rule rule;
        lex::token expected {
            .lexeme = pair.first,
            .type = pair.second,
            .line = 0,
            .column = 0
        };

        ASSERT_EQ(expected, rule.tokenize(state).value());
    }
}
