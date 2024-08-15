#include <random>
#include <benchmark/benchmark.h>
#include "lexer.hpp"


static void
BM_CommentFixedLexing(benchmark::State& state) {
    using namespace simular::cherry;
    for (auto _ : state) {
        // We only care about calling the tokenize function.
        state.PauseTiming();
            lex::state ctx;
            ctx.code = "# My benchmarking comment which is moderate length";
        state.ResumeTiming();

        auto res = lexer::tokenize(ctx);
        (void)res;
    }
}


static void
BM_CommentRandomLexing(benchmark::State& state) {
    using namespace simular::cherry;
    static vec_t<strview_t> k_test_comments {
        "# Lorem ipsum odor amet, consectetuer adipiscing elit.",
        "# Nam adipiscing dapibus nam donec senectus augue maximus augue.",
        "# Ligula dictum primis nullam felis class.",
        "# Per dapibus inceptos primis vitae taciti eget lacinia fames velit.",
        "# Eros felis cubilia tellus nibh metus tempus egestas cras.",
        "# Amet nisi potenti vitae sem pretium erat metus.",
        "# Sit tincidunt litora viverra facilisis ex mauris interdum euismod.",
        "# Praesent mus eros amet, nam quis habitant.",
        "# Etiam vulputate non ante rutrum ipsum ut risus dictumst.",
        "# Ipsum torquent dignissim odio ligula at fringilla risus?",
        "# Ridiculus ridiculus bibendum augue eu ipsum odio scelerisque sapien risus.",
        "# Fames platea mattis montes praesent pellentesque.",
        "# Ornare lectus commodo facilisi, facilisis nibh quam viverra.",
        "# Sit interdum vel vulputate imperdiet iaculis.",
        "# Massa ipsum pulvinar molestie suscipit senectus interdum neque duis vehicula.",
        "# Est urna mollis ex facilisi porta habitasse.",
        "# Ex dapibus dis mauris ultricies fames conubia ullamcorper ridiculus.",
        "# Ad sem aliquet metus aenean mattis.",
        "# Parturient dolor malesuada, augue lacinia interdum ultrices.",
        "# Mi nec litora varius porttitor curabitur.",
        "# Porta augue arcu inceptos sapien metus malesuada laoreet.",
        "# Venenatis ipsum aptent sapien enim cursus vulputate taciti.",
        "# Malesuada tortor proin malesuada elementum quam velit.",
        "# Malesuada molestie feugiat magnis; adipiscing praesent egestas.",
        "# Enim porttitor tristique molestie sapien, natoque platea ornare turpis commodo.",
        "# Himenaeos odio nascetur; ornare feugiat ridiculus eros vestibulum arcu aliquam.",
        "# Tincidunt id iaculis rutrum elit ridiculus eleifend.",
        "# Pretium efficitur aliquet nec, nascetur aliquet volutpat finibus per nostra.",
        "# Ullamcorper arcu dolor elit nascetur mus penatibus ex.",
        "# Dolor duis amet potenti suspendisse efficitur nam justo justo.",
        "# Rutrum nullam nisl a metus molestie dapibus senectus.",
        "# Curae non montes mollis lacus aliquam adipiscing.",
        "# Condimentum ullamcorper molestie quis viverra dictumst per eleifend.",
        "# Suscipit ridiculus elementum netus eleifend pellentesque.",
    };

    std::random_device dev;
    std::mt19937 prng(dev());
    std::uniform_int_distribution<std::mt19937::result_type>
        dist(0, k_test_comments.size());

    for (auto _ : state) {
        state.PauseTiming();
            lex::state ctx;
            ctx.code = k_test_comments[dist(prng)];
        state.ResumeTiming();

        auto res = lexer::tokenize(ctx);
        (void)res;
    }
}


BENCHMARK(BM_CommentFixedLexing);
BENCHMARK(BM_CommentRandomLexing);