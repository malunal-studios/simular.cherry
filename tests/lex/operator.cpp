#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace simular::cherry;


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
    static udict_t<strview_t, leaf>
    k_operator_mapped_types {
        { "+",   leaf::op_add        },
        { "-",   leaf::op_sub        },
        { "*",   leaf::op_mul        },
        { "/",   leaf::op_div        },
        { "%",   leaf::op_mod        },
        { "+=",  leaf::op_add_eq     },
        { "-=",  leaf::op_sub_eq     },
        { "*=",  leaf::op_mul_eq     },
        { "/=",  leaf::op_div_eq     },
        { "%=",  leaf::op_mod_eq     },
        { "++",  leaf::op_inc        },
        { "--",  leaf::op_dec        },
        { "=",   leaf::op_assign     },
        { ".",   leaf::op_access     },
        { "?",   leaf::op_ternary    },
        { "..",  leaf::op_cascade    },
        { "...", leaf::op_ellipsis   },
        { "~",   leaf::op_bitnot     },
        { "&",   leaf::op_bitand     },
        { "|",   leaf::op_bitor      },
        { "^",   leaf::op_bitxor     },
        { "<<",  leaf::op_bitlsh     },
        { ">>",  leaf::op_bitrsh     },
        { "~=",  leaf::op_bitnot_eq  },
        { "&=",  leaf::op_bitand_eq  },
        { "|=",  leaf::op_bitor_eq   },
        { "^=",  leaf::op_bitxor_eq  },
        { "<<=", leaf::op_bitlsh_eq  },
        { ">>=", leaf::op_bitrsh_eq  },
        { "!",   leaf::op_lognot     },
        { "&&",  leaf::op_logand     },
        { "||",  leaf::op_logor      },
        { "<",   leaf::op_logless    },
        { ">",   leaf::op_logmore    },
        { "==",  leaf::op_logequals  },
        { "!=",  leaf::op_lognot_eq  },
        { "&&=", leaf::op_logand_eq  },
        { "||=", leaf::op_logor_eq   },
        { "<=",  leaf::op_logless_eq },
        { ">=",  leaf::op_logmore_eq },
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
