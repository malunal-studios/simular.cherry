#include <gtest/gtest.h>
#include "lexer.hpp"


using namespace cherry;


TEST(LexKeywordTests, litmus_returns_true_when_provided_correct_input) {
    lex::state state;
    state.code = "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    lex::keyword_rule rule;
    for (auto index = 0; index < state.code.length(); index++) {
        ASSERT_TRUE(rule.litmus(state.remaining_source()));
        state.read_src_char();
    }
}


TEST(LexKeywordTests, litmus_returns_false_when_provided_incorrect_input) {
    lex::state state;
    state.code = "0123456789`~!@#$%^&*()-=+{}[]<>|\\;:,./?'\"";

    lex::keyword_rule rule;
    for (auto index = 0; index < state.code.length(); index++) {
        ASSERT_FALSE(rule.litmus(state.remaining_source()));
        state.read_src_char();
    }
}


TEST(LexKeywordTests, tokenize_produces_correct_result) {
    // TODO: maybe move this to some kind of provider.
    static udict_t<strview_t, leaf>
    k_keyword_mapped_types {
        { "null",   leaf::lv_null   },
        { "true",   leaf::lv_true   },
        { "false",  leaf::lv_false  },
        { "var",    leaf::kw_var    },
        { "const",  leaf::kw_const  },
        { "static", leaf::kw_static },
        { "object", leaf::kw_object },
        { "extend", leaf::kw_extend },
        { "def",    leaf::kw_def    },
        { "alias",  leaf::kw_alias  },
        { "bool",   leaf::kw_bool   },
        { "char",   leaf::kw_char   },
        { "int8",   leaf::kw_int8   },
        { "int16",  leaf::kw_int16  },
        { "int32",  leaf::kw_int32  },
        { "int64",  leaf::kw_int64  },
        { "uint8",  leaf::kw_uint8  },
        { "uint16", leaf::kw_uint16 },
        { "uint32", leaf::kw_uint32 },
        { "uint64", leaf::kw_uint64 },
        { "single", leaf::kw_single },
        { "double", leaf::kw_double },
        { "string", leaf::kw_string },
        { "void",   leaf::kw_void   },
        { "using",  leaf::kw_using  },
        { "module", leaf::kw_module },
        { "extern", leaf::kw_extern },
        { "if",     leaf::cf_if     },
        { "else",   leaf::cf_else   },
        { "for",    leaf::cf_for    },
        { "do",     leaf::cf_do     },
        { "while",  leaf::cf_while  },
        { "match",  leaf::cf_match  },
        { "next",   leaf::cf_next   },
        { "break",  leaf::cf_break  },
        { "as",     leaf::cf_as     },
        { "is",     leaf::cf_is     },
        { "return", leaf::cf_return }
    };

    // Try each of the pairs.
    for (auto pair : k_keyword_mapped_types) {
        lex::state state;
        state.code = pair.first;

        lex::token expected {
            .lexeme = pair.first,
            .type = pair.second,
            .line = 0,
            .column = 0
        };

        lex::keyword_rule rule;
        ASSERT_EQ(expected, rule.tokenize(state).value());
    }
}