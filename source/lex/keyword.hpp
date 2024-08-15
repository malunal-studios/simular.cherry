#pragma once


namespace simular::cherry::lex {

/// @brief   A lexical rule that can test the source code input for a keyword
///          and tokenize it when the litmus is successful.
/// @details This rule, along with tokenizing keywords, will also tokenize
///          identifiers. This is because only words can be keywords, and all
///          identifiers are words. So, if the lexeme cannot be typed as any
///          keywords, it will be typed as an identifier instead.
struct keyword_rule final {
    /// @brief   Checks the `source` if it starts with a letter or underscore.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a letter or an underscore; false
    ///          otherwise.
    bool
    litmus(strview_t source) const noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return std::isalpha(source[0]) ||
               source.starts_with('_');
    }

    /// @brief   Tokenizes the keyword, or identifier, by reading the input
    ///          until the input is no longer recognized as being a word.
    /// @details More specifically, the input from this point forth, will be
    ///          read and accepted so long as the input is alphanumeric or an
    ///          underscore.
    /// @param   ctx The current state of lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the keyword that was tokenized; no error
    ///          will be produced from this rule.
    result
    tokenize(state& ctx) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;

        // Just loop until we hit something we don't expect.
        ctx.start_token();
        ctx.read_src_char();
        while (!ctx.end_of_source() && is_keyword_char(ctx.curr_src_char()))
            ctx.read_src_char();

        auto tkn = ctx.extract_token(leaf::unknown);
        tkn.type = get_token_type(tkn.lexeme);
        return tkn;
    }

private:
    static bool
    is_keyword_char(char ch) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
        return std::isalnum(ch) || ch == '_';
    }

    static leaf
    get_token_type(strview_t lexeme) noexcept {
        SIMULAR_TOOLING_MEASURE_FUNCTION;
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

        auto itr = k_keyword_mapped_types.find(lexeme);
        return itr == k_keyword_mapped_types.cend()
            ? leaf::identifier
            : itr->second;
    }
};

} // namespace simular::cherry::lex