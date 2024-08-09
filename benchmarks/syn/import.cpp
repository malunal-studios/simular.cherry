#include <benchmark/benchmark.h>
#include "parser.hpp"

static void
BM_ImportFixedParse(benchmark::State& state) {
    using namespace cherry;
    struct import_mock_lexer final {
        static lex::result
        tokenize(lex::state& ctx) noexcept {
            static int8_t k_index{0};
            static vec_t<lex::token> k_tokens {
                lex::token{ .lexeme = "using", .type = leaf::kw_using,      .line = 0, .column = 0 },
                lex::token{ .lexeme = "std",   .type = leaf::identifier,    .line = 0, .column = 6 },
                lex::token{ .lexeme = ";",     .type = leaf::dc_terminator, .line = 0, .column = 9 },
            };

            if (k_index == k_tokens.size())
                k_index = 0;
            return k_tokens[k_index++];
        }
    };

    for (auto _ : state) {
        state.PauseTiming();
            using mock_lexer = import_mock_lexer;
            using parser = syn::import_parser<mock_lexer>;
            syn::state<mock_lexer> ctx;
            ctx.next_token();
        state.ResumeTiming();

        auto res = parser::parse(ctx);
        (void)res;
    }
}

static void
BM_ImportLongParse(benchmark::State& state) {
    using namespace cherry;
    struct import_mock_lexer final {
        static lex::result
        tokenize(lex::state& ctx) noexcept {
            static int8_t k_index{0};
            static vec_t<lex::token> k_tokens {
                lex::token{ .lexeme = "using",  .type = leaf::kw_using,      .line = 0, .column =  0 },
                lex::token{ .lexeme = "std",    .type = leaf::identifier,    .line = 0, .column =  6 },
                lex::token{ .lexeme = ".",      .type = leaf::identifier,    .line = 0, .column =  9 },
                lex::token{ .lexeme = "io",     .type = leaf::identifier,    .line = 0, .column = 10 },
                lex::token{ .lexeme = ".",      .type = leaf::identifier,    .line = 0, .column = 12 },
                lex::token{ .lexeme = "file",   .type = leaf::identifier,    .line = 0, .column = 13 },
                lex::token{ .lexeme = ".",      .type = leaf::identifier,    .line = 0, .column = 17 },
                lex::token{ .lexeme = "stream", .type = leaf::identifier,    .line = 0, .column = 18 },
                lex::token{ .lexeme = ";",      .type = leaf::dc_terminator, .line = 0, .column = 24 },
            };

            if (k_index == k_tokens.size())
                k_index = 0;
            return k_tokens[k_index++];
        }
    };

    for (auto _ : state) {
        state.PauseTiming();
            using mock_lexer = import_mock_lexer;
            using parser = syn::import_parser<mock_lexer>;
            syn::state<mock_lexer> ctx;
            ctx.next_token();
        state.ResumeTiming();

        auto res = parser::parse(ctx);
        (void)res;
    }
}

BENCHMARK(BM_ImportFixedParse);
BENCHMARK(BM_ImportLongParse);
