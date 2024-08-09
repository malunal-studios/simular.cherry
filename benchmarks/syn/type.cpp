#include <benchmark/benchmark.h>
#include "parser.hpp"

static void
BM_RawTypeFixedParser(benchmark::State& state) {
    using namespace cherry;
    struct type_mock_lexer final {
        static lex::result
        tokenize(lex::state& ctx) noexcept {
            static int8_t k_index{0};
            static vec_t<lex::token> k_tokens {
                lex::token{ .lexeme = "std",  .type = leaf::identifier, .line = 0, .column =  0 },
                lex::token{ .lexeme = ".",    .type = leaf::op_access,  .line = 0, .column =  3 },
                lex::token{ .lexeme = "io",   .type = leaf::identifier, .line = 0, .column =  4 },
                lex::token{ .lexeme = ".",    .type = leaf::op_access,  .line = 0, .column =  6 },
                lex::token{ .lexeme = "file", .type = leaf::identifier, .line = 0, .column =  7 }
            };

            if (k_index == k_tokens.size())
                k_index = 0;
            return k_tokens[k_index++];
        }
    };

    for (auto _ : state) {
        state.PauseTiming();
            using mock_lexer = type_mock_lexer;
            using parser = syn::type_parser<mock_lexer>;
            syn::state<mock_lexer> ctx;
            ctx.next_token();
        state.ResumeTiming();

        auto res = parser::parse(ctx);
        (void)res;
    }
}

static void
BM_FuncTypeFixedParser(benchmark::State& state) {
    using namespace cherry;
    struct type_mock_lexer final {
        static lex::result
        tokenize(lex::state& ctx) noexcept {
            static int8_t k_index{0};
            static vec_t<lex::token> k_tokens {
                lex::token{ .lexeme = "std",     .type = leaf::identifier, .line = 0, .column =  0 },
                lex::token{ .lexeme = ".",       .type = leaf::op_access,  .line = 0, .column =  3 },
                lex::token{ .lexeme = "io",      .type = leaf::identifier, .line = 0, .column =  4 },
                lex::token{ .lexeme = ".",       .type = leaf::op_access,  .line = 0, .column =  6 },
                lex::token{ .lexeme = "console", .type = leaf::identifier, .line = 0, .column =  7 },
                lex::token{ .lexeme = ".",       .type = leaf::op_access,  .line = 0, .column = 14 },
                lex::token{ .lexeme = "write",   .type = leaf::identifier, .line = 0, .column = 15 },
                lex::token{ .lexeme = "(",       .type = leaf::dc_lparen,  .line = 0, .column = 20 },
                lex::token{ .lexeme = "string",  .type = leaf::kw_string,  .line = 0, .column = 21 },
                lex::token{ .lexeme = ")",       .type = leaf::dc_rparen,  .line = 0, .column = 27 },
                lex::token{ .lexeme = ":",       .type = leaf::dc_colon,   .line = 0, .column = 29 },
                lex::token{ .lexeme = "void",    .type = leaf::kw_void,    .line = 0, .column = 31 }
            };

            if (k_index == k_tokens.size())
                k_index = 0;
            return k_tokens[k_index++];
        }
    };

    for (auto _ : state) {
        state.PauseTiming();
            using mock_lexer = type_mock_lexer;
            using parser = syn::type_parser<mock_lexer>;
            syn::state<mock_lexer> ctx;
            ctx.next_token();
        state.ResumeTiming();

        auto res = parser::parse(ctx);
        (void)res;
    }
}

static void
BM_RefTypeFixedParser(benchmark::State& state) {
    using namespace cherry;
    struct type_mock_lexer final {
        static lex::result
        tokenize(lex::state& ctx) noexcept {
            static int8_t k_index{0};
            static vec_t<lex::token> k_tokens {
                lex::token{ .lexeme = "int32", .type = leaf::kw_int32,  .line = 0, .column =  0 },
                lex::token{ .lexeme = "*",     .type = leaf::op_mul,    .line = 0, .column =  5 },
                lex::token{ .lexeme = "*",     .type = leaf::op_mul,    .line = 0, .column =  6 },
                lex::token{ .lexeme = "&",     .type = leaf::op_bitand, .line = 0, .column =  7 },
                lex::token{ .lexeme = "&",     .type = leaf::op_bitand, .line = 0, .column =  8 },
                lex::token{ .lexeme = "*",     .type = leaf::op_mul,    .line = 0, .column =  9 },
                lex::token{ .lexeme = "&",     .type = leaf::op_bitand, .line = 0, .column = 10 },
            };

            if (k_index == k_tokens.size())
                k_index = 0;
            return k_tokens[k_index++];
        }
    };

    for (auto _ : state) {
        state.PauseTiming();
            using mock_lexer = type_mock_lexer;
            using parser = syn::type_parser<mock_lexer>;
            syn::state<mock_lexer> ctx;
            ctx.next_token();
        state.ResumeTiming();

        auto res = parser::parse(ctx);
        (void)res;
    }
}

BENCHMARK(BM_RawTypeFixedParser);
BENCHMARK(BM_FuncTypeFixedParser);
BENCHMARK(BM_RefTypeFixedParser);
