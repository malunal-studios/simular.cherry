macro(create_test TESTNAME TESTFILE)
    add_executable(${TESTNAME} ${TESTFILE})
    target_link_libraries(
        ${TESTNAME}
        PRIVATE GTest::gtest_main
        PRIVATE simular::cherry
    )

    gtest_discover_tests(${TESTNAME})
endmacro()

# Lexical analysis tests.
create_test(lex.state.test lex/state.cpp)
create_test(lex.comment.test lex/comment.cpp)
create_test(lex.keyword.test lex/keyword.cpp)
create_test(lex.binary.test lex/binary.cpp)
create_test(lex.octal.test lex/octal.cpp)
create_test(lex.decimal.test lex/decimal.cpp)
create_test(lex.hexadecimal.test lex/hexadecimal.cpp)
create_test(lex.character.test lex/character.cpp)
create_test(lex.string.test lex/string.cpp)
create_test(lex.operator.test lex/operator.cpp)
create_test(lex.lexer.test lex/lexer.cpp)

# Syntactic analysis tests.
# create_test(syn.import.test syn/import.cpp)
# create_test(syn.paths.test syn/paths.cpp)
# create_test(syn.type.test syn/type.cpp)
