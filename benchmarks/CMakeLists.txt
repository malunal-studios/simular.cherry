macro(create_bench TESTNAME TESTFILE)
    add_executable(${TESTNAME} ${TESTFILE})
    target_link_libraries(
        ${TESTNAME}
        PRIVATE benchmark::benchmark
        PRIVATE benchmark::benchmark_main

        # Currently header only, may not be forever
        PRIVATE simular::cherry
    )
endmacro()

# Lexical analysis benchmarks
create_bench(lex.comment.bench lex/comment.cpp)
create_bench(lex.keyword.bench lex/keyword.cpp)
# create_test(lex.binary.bench lex/binary.cpp)
# create_test(lex.octal.bench lex/octal.cpp)
# create_test(lex.decimal.bench lex/decimal.cpp)
# create_test(lex.hexadecimal.bench lex/hexadecimal.cpp)
# create_test(lex.character.bench lex/character.cpp)
# create_test(lex.string.bench lex/string.cpp)
# create_test(lex.operator.bench lex/operator.cpp)
# create_test(lex.lexer.bench lex/lexer.cpp)

# Syntactic analysis benchmarks
create_bench(syn.import.bench syn/import.cpp)
create_bench(syn.type.bench syn/type.cpp)
