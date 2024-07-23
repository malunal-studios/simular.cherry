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
    static udict_t<strview_t, lex::leaf>
    k_keyword_mapped_types {
        { "null",   lex::leaf::lv_null   },
        { "true",   lex::leaf::lv_true   },
        { "false",  lex::leaf::lv_false  },
        { "var",    lex::leaf::kw_var    },
        { "const",  lex::leaf::kw_const  },
        { "static", lex::leaf::kw_static },
        { "object", lex::leaf::kw_object },
        { "extend", lex::leaf::kw_extend },
        { "def",    lex::leaf::kw_def    },
        { "alias",  lex::leaf::kw_alias  },
        { "bool",   lex::leaf::kw_bool   },
        { "char",   lex::leaf::kw_char   },
        { "int8",   lex::leaf::kw_int8   },
        { "int16",  lex::leaf::kw_int16  },
        { "int32",  lex::leaf::kw_int32  },
        { "int64",  lex::leaf::kw_int64  },
        { "uint8",  lex::leaf::kw_uint8  },
        { "uint16", lex::leaf::kw_uint16 },
        { "uint32", lex::leaf::kw_uint32 },
        { "uint64", lex::leaf::kw_uint64 },
        { "single", lex::leaf::kw_single },
        { "double", lex::leaf::kw_double },
        { "string", lex::leaf::kw_string },
        { "void",   lex::leaf::kw_void   },
        { "using",  lex::leaf::kw_using  },
        { "module", lex::leaf::kw_module },
        { "extern", lex::leaf::kw_extern },
        { "if",     lex::leaf::cf_if     },
        { "else",   lex::leaf::cf_else   },
        { "for",    lex::leaf::cf_for    },
        { "do",     lex::leaf::cf_do     },
        { "while",  lex::leaf::cf_while  },
        { "match",  lex::leaf::cf_match  },
        { "next",   lex::leaf::cf_next   },
        { "break",  lex::leaf::cf_break  },
        { "as",     lex::leaf::cf_as     },
        { "is",     lex::leaf::cf_is     },
        { "return", lex::leaf::cf_return }
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