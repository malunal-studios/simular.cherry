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
    static udict_t<strview_t, lex::leaf>
    k_operator_mapped_types {
        { "+",   lex::leaf::op_add        },
        { "-",   lex::leaf::op_sub        },
        { "*",   lex::leaf::op_mul        },
        { "/",   lex::leaf::op_div        },
        { "%",   lex::leaf::op_mod        },
        { "+=",  lex::leaf::op_add_eq     },
        { "-=",  lex::leaf::op_sub_eq     },
        { "*=",  lex::leaf::op_mul_eq     },
        { "/=",  lex::leaf::op_div_eq     },
        { "%=",  lex::leaf::op_mod_eq     },
        { "++",  lex::leaf::op_inc        },
        { "--",  lex::leaf::op_dec        },
        { "=",   lex::leaf::op_assign     },
        { ".",   lex::leaf::op_access     },
        { "?",   lex::leaf::op_ternary    },
        { "..",  lex::leaf::op_cascade    },
        { "...", lex::leaf::op_ellipsis   },
        { "~",   lex::leaf::op_bitnot     },
        { "&",   lex::leaf::op_bitand     },
        { "|",   lex::leaf::op_bitor      },
        { "^",   lex::leaf::op_bitxor     },
        { "<<",  lex::leaf::op_bitlsh     },
        { ">>",  lex::leaf::op_bitrsh     },
        { "~=",  lex::leaf::op_bitnot_eq  },
        { "&=",  lex::leaf::op_bitand_eq  },
        { "|=",  lex::leaf::op_bitor_eq   },
        { "^=",  lex::leaf::op_bitxor_eq  },
        { "<<=", lex::leaf::op_bitlsh_eq  },
        { ">>=", lex::leaf::op_bitrsh_eq  },
        { "!",   lex::leaf::op_lognot     },
        { "&&",  lex::leaf::op_logand     },
        { "||",  lex::leaf::op_logor      },
        { "<",   lex::leaf::op_logless    },
        { ">",   lex::leaf::op_logmore    },
        { "==",  lex::leaf::op_logequals  },
        { "!=",  lex::leaf::op_lognot_eq  },
        { "&&=", lex::leaf::op_logand_eq  },
        { "||=", lex::leaf::op_logor_eq   },
        { "<=",  lex::leaf::op_logless_eq },
        { ">=",  lex::leaf::op_logmore_eq },
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
