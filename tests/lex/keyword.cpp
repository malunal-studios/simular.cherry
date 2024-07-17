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
    static dict_t<strview_t, lex::token_type>
    k_keyword_mapped_types {
        { "null",   lex::token_type::lv_null   },
        { "true",   lex::token_type::lv_true   },
        { "false",  lex::token_type::lv_false  },
        { "var",    lex::token_type::kw_var    },
        { "const",  lex::token_type::kw_const  },
        { "static", lex::token_type::kw_static },
        { "object", lex::token_type::kw_object },
        { "extend", lex::token_type::kw_extend },
        { "def",    lex::token_type::kw_def    },
        { "alias",  lex::token_type::kw_alias  },
        { "bool",   lex::token_type::kw_bool   },
        { "char",   lex::token_type::kw_char   },
        { "int8",   lex::token_type::kw_int8   },
        { "int16",  lex::token_type::kw_int16  },
        { "int32",  lex::token_type::kw_int32  },
        { "int64",  lex::token_type::kw_int64  },
        { "uint8",  lex::token_type::kw_uint8  },
        { "uint16", lex::token_type::kw_uint16 },
        { "uint32", lex::token_type::kw_uint32 },
        { "uint64", lex::token_type::kw_uint64 },
        { "single", lex::token_type::kw_single },
        { "double", lex::token_type::kw_double },
        { "string", lex::token_type::kw_string },
        { "void",   lex::token_type::kw_void   },
        { "using",  lex::token_type::kw_using  },
        { "module", lex::token_type::kw_module },
        { "extern", lex::token_type::kw_extern },
        { "if",     lex::token_type::cf_if     },
        { "else",   lex::token_type::cf_else   },
        { "for",    lex::token_type::cf_for    },
        { "do",     lex::token_type::cf_do     },
        { "while",  lex::token_type::cf_while  },
        { "match",  lex::token_type::cf_match  },
        { "next",   lex::token_type::cf_next   },
        { "break",  lex::token_type::cf_break  },
        { "as",     lex::token_type::cf_as     },
        { "is",     lex::token_type::cf_is     },
        { "return", lex::token_type::cf_return }
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