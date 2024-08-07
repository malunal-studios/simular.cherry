#include <random>
#include <benchmark/benchmark.h>
#include "lexer.hpp"

static void
BM_KeywordFixedLexing(benchmark::State& state) {
    using namespace cherry;
    for (auto _ : state) {
        state.PauseTiming();
            lex::state ctx;
            ctx.code = "extern";
        state.ResumeTiming();

        auto res = lexer::tokenize(ctx);
        (void)res;
    }
}

static void
BM_KeywordRandomLexing(benchmark::State& state) {
    using namespace cherry;
    static vec_t<strview_t> k_test_keywords {
        "null", "true", "false", "var", "const", "static",
        "object", "extend", "def", "alias", "bool", "char",
        "int8", "int16", "int32", "int64", "uint8", "uint16",
        "uint32", "uint64", "single", "double", "string", "void",
        "using", "module", "extern", "if", "else", "for",
        "do", "while", "match", "next", "break", "as", "is",
        "return",
    };

    std::random_device dev;
    std::mt19937 prng(dev());
    std::uniform_int_distribution<std::mt19937::result_type>
        dist(0, k_test_keywords.size());
    for (auto _ : state) {
        state.PauseTiming();
            lex::state ctx;
            ctx.code = k_test_keywords[dist(prng)];
        state.ResumeTiming();

        auto res = lexer::tokenize(ctx);
        (void)res;
    }
}

static void
BM_KeywordMixedLexing(benchmark::State& state) {
    using namespace cherry;
    static vec_t<strview_t> k_random_list {
        "true", "do", "bool", "_huw6bqdjpyt", "uint16",
        "while", "iyl1dm8tz3xa", "_xywcd74bpai", "zu407m1xd6bn", "_3iof4ewq5s9",
        "wagh1fy4qlcv", "apsbmi6u7qgk", "aoq8ztly07kj", "as", "gz9rdwhpnua5",
        "_zfuhbsmqd0l", "ilb79ewuoyp6", "owjvikefum4q", "m8kxj1uo93ac", "zutxy85asg1r",
        "return", "int8", "jp216q03wmfb", "_9v0ljauhsby", "c63zkt1p4gew",
        "eaor0876lsmn", "extern", "char", "c90gkjy6xpwn", "break",
        "extend", "t924mphaqxey", "uint8", "using", "else",
        "pa83g5nkv7j0", "_1hvmf7jc9yw", "_vbrhn6zmpsl", "gjq6f8oy1sl5", "match",
        "void", "uint64", "object", "alias", "false",
        "static", "am4suokc7y9d", "_tpumxy0ks7f", "_vwoayhc0eid", "fiuc4bdzn5q1",
        "string", "_ugo07wl2jrz", "single", "b5nz7vahj1o3", "def",
        "var", "next", "if", "z8ketbdlpnu0", "double",
        "rezqjtcdiya3", "t04bfcezr5ym", "_rcyiza758nl", "int64", "_3wr5lcdhsvq",
        "yth0dw27a6s3", "null", "roxvednk7p54", "_oi1xkezjatn", "_mt2r7i18pbx",
        "int32", "uint32", "module", "ekgcq14jvyb6", "for",
        "ptr6hmb4so2n", "fizns90tor4x", "is", "tufvwdahcxp5", "_npgd0fvo2ir",
        "benrj0iv4ftk", "z7qhw04xu3v6", "int16", "qip86d79aru5", "const",
        "gqy140b76p5r", "vza0wpl9eo4n", "i5vg3nm72ej0",
    };

    std::random_device dev;
    std::mt19937 prng(dev());
    std::uniform_int_distribution<std::mt19937::result_type>
        dist(0, k_random_list.size());
    for (auto _ : state) {
        state.PauseTiming();
            lex::state ctx;
            ctx.code = k_random_list[dist(prng)];
        state.ResumeTiming();

        auto res = lexer::tokenize(ctx);
        (void)res;
    }
}

static void
BM_IdentifierFixedLexing(benchmark::State& state) {
    using namespace cherry;
    for (auto _ : state) {
        state.PauseTiming();
            lex::state ctx;
            ctx.code = "my_unique_identifier";
        state.ResumeTiming();

        auto res = lexer::tokenize(ctx);
        (void)res;
    }
}

static void
BM_IdentifierRandomLexing(benchmark::State& state) {
    using namespace cherry;
    static vec_t<strview_t> k_test_identifiers {
        "_xywcd74bpai", "_3iof4ewq5s9", "ptr6hmb4so2n", "_ugo07wl2jrz", "_zfuhbsmqd0l",
        "ilb79ewuoyp6", "fizns90tor4x", "t04bfcezr5ym", "ekgcq14jvyb6", "zutxy85asg1r",
        "_1hvmf7jc9yw", "qip86d79aru5", "gz9rdwhpnua5", "_vwoayhc0eid", "c63zkt1p4gew",
        "_vbrhn6zmpsl", "fiuc4bdzn5q1", "zu407m1xd6bn", "c90gkjy6xpwn", "vza0wpl9eo4n",
        "i5vg3nm72ej0", "t924mphaqxey", "_9v0ljauhsby", "_tpumxy0ks7f", "benrj0iv4ftk",
        "eaor0876lsmn", "_rcyiza758nl", "_oi1xkezjatn", "yth0dw27a6s3", "z7qhw04xu3v6",
        "m8kxj1uo93ac", "b5nz7vahj1o3", "z8ketbdlpnu0", "owjvikefum4q", "wagh1fy4qlcv",
        "_npgd0fvo2ir", "apsbmi6u7qgk", "_mt2r7i18pbx", "_3wr5lcdhsvq", "_huw6bqdjpyt",
        "am4suokc7y9d", "roxvednk7p54", "iyl1dm8tz3xa", "aoq8ztly07kj", "gjq6f8oy1sl5",
        "jp216q03wmfb", "pa83g5nkv7j0", "tufvwdahcxp5", "gqy140b76p5r", "rezqjtcdiya3",
    };

    std::random_device dev;
    std::mt19937 prng(dev());
    std::uniform_int_distribution<std::mt19937::result_type>
        dist(0, k_test_identifiers.size());
    for (auto _ : state) {
        state.PauseTiming();
            lex::state ctx;
            ctx.code = k_test_identifiers[dist(prng)];
        state.ResumeTiming();

        auto res = lexer::tokenize(ctx);
        (void)res;
    }
}

BENCHMARK(BM_KeywordFixedLexing);
BENCHMARK(BM_KeywordRandomLexing);
BENCHMARK(BM_IdentifierFixedLexing);
BENCHMARK(BM_IdentifierRandomLexing);
BENCHMARK(BM_KeywordMixedLexing);