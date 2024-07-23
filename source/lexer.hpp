#pragma once
#include <array>
#include <concepts>
#include <expected>
#include <filesystem>
#include "types.hpp"


namespace cherry {

/// @brief   Defines the different types of tokens.
/// @details When a token is extracted from the source code, it needs to be
///          identified as some type because the parser needs to know this in
///          order to perform parsing operations.
/// @remarks This enum gets it's name because of the fact that all tokens are
///          leaves of a grammar (in other words, terminals). It doesn't make
///          sense to create another enum with these values redefined in the
///          grammar header file.
enum class leaf : int16_t {
    /// @brief   Represents the end of the source code.
    /// @details Used to indicate to the parser that the end of the source code
    ///          has been reached.
    eos = -1,

    /// @brief   The token type is unknown.
    /// @details This is mostly a utility type; if the token type cannot be
    ///          determined at all, this type may be used. It should represent
    ///          an error within the source.
    unknown,

    /// @brief   The token type is a comment.
    /// @details A rule may produce this if it is responsible for tokenizing a
    ///          comment from the source. Commonly, these tokens are ignored but
    ///          they are required to be tokenized because of the design of the
    ///          lexical analyzer. With this design however, it may be useful to
    ///          be able to obtain them for documentation creation.
    comment,

    /// @brief   The token is an identifier.
    /// @details An identifier is any string of alphanumeric characters and
    ///          underscores that does not make up a known keyword. The often
    ///          represent the name of user defined objects, including but not
    ///          limited to: modules, user-types, variables, and functions.
    identifier,

    /**************************************************************************/
    /*                             LITERAL VALUES                             */
    /**************************************************************************/
    /// @brief   Defines a literal value that is a signed integer.
    /// @details Integer values, when not explicitly unsigned, are signed and
    ///          will be processed as a 64bit integer. If assigned to a fixed
    ///          width integer, it will be automatically casted to that type.
    lv_signed,

    /// @brief   Defines a literal value that is an unsigned integer.
    /// @details Unsigned integers must be explicitly declared using the 'u'
    ///          postfix on binary, octal, integral, and hexadecimal numbers.
    ///          The value will be processed as a 64bit integer, but it will
    ///          be automatically casted to a fixed width integer when assigned.
    lv_unsigned,

    /// @brief   Defines a literal value that is a decimal number.
    /// @details Decimal numbers are treated as double precision, but will be
    ///          casted to it's appropriate precision or truncated depending on
    ///          the type it's being assigned to.
    lv_decimal,

    /// @brief   Defines a literal value that is a wide character.
    /// @details Character literals are treated automatically as a wide
    ///          character to support extended character sets, unicode, emojis,
    ///          and the like.
    lv_character,

    /// @brief   Defines a literal value that is a *raw* wide string.
    /// @details String literals are treated automatically as a wide string to
    ///          support extended character sets, unicode, emojis, and the like.
    lv_raw_string,

    /// @brief   Defines a literal value that is a *interpolated* wide string.
    /// @details This is the same as a *raw* string, but this string contains
    ///          pieces that should be parsed and evaluated before the string
    ///          is finalized.
    lv_int_string,

    /// @brief   Defines a literal value that is null.
    /// @details A null value is useful for working with pointers, references,
    ///          and other nullable types as null represents an absent value.
    lv_null,

    /// @brief   Defines a literal value that is true (1).
    /// @details A true value is useful for logical comparisons and value
    ///          assignments to boolean types.
    lv_true,

    /// @brief   Defines a literal value that is false (0).
    /// @details A false value is useful for logical comparisons and value
    ///          assignments to boolean types.
    lv_false,

    /**************************************************************************/
    /*                                KEYWORDS                                */
    /**************************************************************************/
    kw_var,
    kw_const,
    kw_static,
    kw_object,
    kw_extend,
    kw_def,
    kw_alias,
    kw_bool,
    kw_char,
    kw_int8,
    kw_int16,
    kw_int32,
    kw_int64,
    kw_uint8,
    kw_uint16,
    kw_uint32,
    kw_uint64,
    kw_single,
    kw_double,
    kw_string,
    kw_void,
    kw_using,
    kw_module,
    kw_extern,

    /**************************************************************************/
    /*                              CONTROL FLOW                              */
    /**************************************************************************/
    cf_if,
    cf_else,
    cf_for,
    cf_do,
    cf_while,
    cf_match,
    cf_next,
    cf_break,
    cf_as,
    cf_is,
    cf_return,

    /**************************************************************************/
    /*                                OPERATORS                               */
    /**************************************************************************/
    op_add,
    op_sub,
    op_mul,
    op_div,
    op_mod,
    op_add_eq,
    op_sub_eq,
    op_mul_eq,
    op_div_eq,
    op_mod_eq,
    op_inc,
    op_dec,
    op_assign,
    op_access,
    op_ternary,
    op_cascade,
    op_ellipsis,
    op_bitnot,
    op_bitand,
    op_bitor,
    op_bitxor,
    op_bitlsh,
    op_bitrsh,
    op_bitnot_eq,
    op_bitand_eq,
    op_bitor_eq,
    op_bitxor_eq,
    op_bitlsh_eq,
    op_bitrsh_eq,
    op_lognot,
    op_logand,
    op_logor,
    op_logless,
    op_logmore,
    op_logequals,
    op_lognot_eq,
    op_logand_eq,
    op_logor_eq,
    op_logless_eq,
    op_logmore_eq,

    /**************************************************************************/
    /*                               DELIMITERS                               */
    /**************************************************************************/
    dc_lparen,
    dc_rparen,
    dc_lbracket,
    dc_rbracket,
    dc_lbrace,
    dc_rbrace,
    dc_comma,
    dc_terminator,
    dc_colon,
};

/// @brief   Writes the token type to an output stream.
/// @param   oss  The output stream to write the token type to.
/// @param   type The token type to write to the output stream.
/// @returns The output stream with the token type written to it. 
inline std::ostream&
operator<<(std::ostream& oss, leaf type) noexcept {
    switch (type) {
    case leaf::eos:           oss << "eos("           << (int)type << ')'; break;
    case leaf::unknown:       oss << "unknown("       << (int)type << ')'; break;
    case leaf::comment:       oss << "comment("       << (int)type << ')'; break;
    case leaf::identifier:    oss << "identifier("    << (int)type << ')'; break;
    case leaf::lv_signed:     oss << "lv_signed("     << (int)type << ')'; break;
    case leaf::lv_unsigned:   oss << "lv_unsigned("   << (int)type << ')'; break;
    case leaf::lv_decimal:    oss << "lv_decimal("    << (int)type << ')'; break;
    case leaf::lv_character:  oss << "lv_character("  << (int)type << ')'; break;
    case leaf::lv_raw_string: oss << "lv_raw_string(" << (int)type << ')'; break;
    case leaf::lv_int_string: oss << "lv_int_string(" << (int)type << ')'; break;
    case leaf::lv_null:       oss << "lv_null("       << (int)type << ')'; break;
    case leaf::lv_true:       oss << "lv_true("       << (int)type << ')'; break;
    case leaf::lv_false:      oss << "lv_false("      << (int)type << ')'; break;
    case leaf::kw_var:        oss << "kw_var("        << (int)type << ')'; break;
    case leaf::kw_const:      oss << "kw_const("      << (int)type << ')'; break;
    case leaf::kw_static:     oss << "kw_static("     << (int)type << ')'; break;
    case leaf::kw_object:     oss << "kw_object("     << (int)type << ')'; break;
    case leaf::kw_extend:     oss << "kw_extend("     << (int)type << ')'; break;
    case leaf::kw_def:        oss << "kw_def("        << (int)type << ')'; break;
    case leaf::kw_alias:      oss << "kw_alias("      << (int)type << ')'; break;
    case leaf::kw_bool:       oss << "kw_bool("       << (int)type << ')'; break;
    case leaf::kw_char:       oss << "kw_char("       << (int)type << ')'; break;
    case leaf::kw_int8:       oss << "kw_int8("       << (int)type << ')'; break;
    case leaf::kw_int16:      oss << "kw_int16("      << (int)type << ')'; break;
    case leaf::kw_int32:      oss << "kw_int32("      << (int)type << ')'; break;
    case leaf::kw_int64:      oss << "kw_int64("      << (int)type << ')'; break;
    case leaf::kw_uint8:      oss << "kw_uint8("      << (int)type << ')'; break;
    case leaf::kw_uint16:     oss << "kw_uint16("     << (int)type << ')'; break;
    case leaf::kw_uint32:     oss << "kw_uint32("     << (int)type << ')'; break;
    case leaf::kw_uint64:     oss << "kw_uint64("     << (int)type << ')'; break;
    case leaf::kw_single:     oss << "kw_single("     << (int)type << ')'; break;
    case leaf::kw_double:     oss << "kw_double("     << (int)type << ')'; break;
    case leaf::kw_string:     oss << "kw_string("     << (int)type << ')'; break;
    case leaf::kw_void:       oss << "kw_void("       << (int)type << ')'; break;
    case leaf::kw_using:      oss << "kw_using("      << (int)type << ')'; break;
    case leaf::kw_module:     oss << "kw_module("     << (int)type << ')'; break;
    case leaf::kw_extern:     oss << "kw_extern("     << (int)type << ')'; break;
    case leaf::cf_if:         oss << "cf_if("         << (int)type << ')'; break;
    case leaf::cf_else:       oss << "cf_else("       << (int)type << ')'; break;
    case leaf::cf_for:        oss << "cf_for("        << (int)type << ')'; break;
    case leaf::cf_do:         oss << "cf_do("         << (int)type << ')'; break;
    case leaf::cf_while:      oss << "cf_while("      << (int)type << ')'; break;
    case leaf::cf_match:      oss << "cf_match("      << (int)type << ')'; break;
    case leaf::cf_next:       oss << "cf_next("       << (int)type << ')'; break;
    case leaf::cf_break:      oss << "cf_break("      << (int)type << ')'; break;
    case leaf::cf_as:         oss << "cf_as("         << (int)type << ')'; break;
    case leaf::cf_is:         oss << "cf_is("         << (int)type << ')'; break;
    case leaf::cf_return:     oss << "cf_return("     << (int)type << ')'; break;
    case leaf::op_add:        oss << "op_add("        << (int)type << ')'; break;
    case leaf::op_sub:        oss << "op_sub("        << (int)type << ')'; break;
    case leaf::op_mul:        oss << "op_mul("        << (int)type << ')'; break;
    case leaf::op_div:        oss << "op_div("        << (int)type << ')'; break;
    case leaf::op_mod:        oss << "op_mod("        << (int)type << ')'; break;
    case leaf::op_add_eq:     oss << "op_add_eq("     << (int)type << ')'; break;
    case leaf::op_sub_eq:     oss << "op_sub_eq("     << (int)type << ')'; break;
    case leaf::op_mul_eq:     oss << "op_mul_eq("     << (int)type << ')'; break;
    case leaf::op_div_eq:     oss << "op_div_eq("     << (int)type << ')'; break;
    case leaf::op_mod_eq:     oss << "op_mod_eq("     << (int)type << ')'; break;
    case leaf::op_inc:        oss << "op_inc("        << (int)type << ')'; break;
    case leaf::op_dec:        oss << "op_dec("        << (int)type << ')'; break;
    case leaf::op_assign:     oss << "op_assign("     << (int)type << ')'; break;
    case leaf::op_access:     oss << "op_access("     << (int)type << ')'; break;
    case leaf::op_ternary:    oss << "op_ternary("    << (int)type << ')'; break;
    case leaf::op_cascade:    oss << "op_cascade("    << (int)type << ')'; break;
    case leaf::op_ellipsis:   oss << "op_ellipsis("   << (int)type << ')'; break;
    case leaf::op_bitnot:     oss << "op_bitnot("     << (int)type << ')'; break;
    case leaf::op_bitand:     oss << "op_bitand("     << (int)type << ')'; break;
    case leaf::op_bitor:      oss << "op_bitor("      << (int)type << ')'; break;
    case leaf::op_bitxor:     oss << "op_bitxor("     << (int)type << ')'; break;
    case leaf::op_bitlsh:     oss << "op_bitlsh("     << (int)type << ')'; break;
    case leaf::op_bitrsh:     oss << "op_bitrsh("     << (int)type << ')'; break;
    case leaf::op_bitnot_eq:  oss << "op_bitnot_eq("  << (int)type << ')'; break;
    case leaf::op_bitand_eq:  oss << "op_bitand_eq("  << (int)type << ')'; break;
    case leaf::op_bitor_eq:   oss << "op_bitor_eq("   << (int)type << ')'; break;
    case leaf::op_bitxor_eq:  oss << "op_bitxor_eq("  << (int)type << ')'; break;
    case leaf::op_bitlsh_eq:  oss << "op_bitlsh_eq("  << (int)type << ')'; break;
    case leaf::op_bitrsh_eq:  oss << "op_bitrsh_eq("  << (int)type << ')'; break;
    case leaf::op_lognot:     oss << "op_lognot("     << (int)type << ')'; break;
    case leaf::op_logand:     oss << "op_logand("     << (int)type << ')'; break;
    case leaf::op_logor:      oss << "op_logor("      << (int)type << ')'; break;
    case leaf::op_logless:    oss << "op_logless("    << (int)type << ')'; break;
    case leaf::op_logmore:    oss << "op_logmore("    << (int)type << ')'; break;
    case leaf::op_logequals:  oss << "op_logequals("  << (int)type << ')'; break;
    case leaf::op_lognot_eq:  oss << "op_lognot_eq("  << (int)type << ')'; break;
    case leaf::op_logand_eq:  oss << "op_logand_eq("  << (int)type << ')'; break;
    case leaf::op_logor_eq:   oss << "op_logor_eq("   << (int)type << ')'; break;
    case leaf::op_logless_eq: oss << "op_logless_eq(" << (int)type << ')'; break;
    case leaf::op_logmore_eq: oss << "op_logmore_eq(" << (int)type << ')'; break;
    case leaf::dc_lparen:     oss << "dc_lparen("     << (int)type << ')'; break;
    case leaf::dc_rparen:     oss << "dc_rparen("     << (int)type << ')'; break;
    case leaf::dc_lbracket:   oss << "dc_lbracket("   << (int)type << ')'; break;
    case leaf::dc_rbracket:   oss << "dc_rbracket("   << (int)type << ')'; break;
    case leaf::dc_lbrace:     oss << "dc_lbrace("     << (int)type << ')'; break;
    case leaf::dc_rbrace:     oss << "dc_rbrace("     << (int)type << ')'; break;
    case leaf::dc_comma:      oss << "dc_comma("      << (int)type << ')'; break;
    case leaf::dc_terminator: oss << "dc_terminator(" << (int)type << ')'; break;
    case leaf::dc_colon:      oss << "dc_colon("      << (int)type << ')'; break;
    }
    return oss;
}


namespace lex {

/// @brief Defines the different error codes that can be produce during lexical
///        analysis when an unexpected problem is encountered.
enum class errc {
    /// @brief   An unrecoverable error has occurred.
    /// @details Operations that possibly fail should return this value when
    ///          they experience an unexpected error they cannot recover from.
    unrecoverable = -1,

    /// @brief   There is no error, the operation was successful.
    /// @remarks Functions that possibly fail should return this value when
    ///          they succeed.
    success = EXIT_SUCCESS,

    /// @brief   Generic failure, no informaion could be provided.
    /// @remarks Unlike unrecoverable, this error is presumed to be recoverable.
    ///          Whether it is, is left to the developer to decide. 
    failure = EXIT_FAILURE,

    /// @brief   Provided by a lexical analyzer which cannot process the input.
    /// @details If a lexer that has at least one lexical rule fails all of it's
    ///          litmus tests to find a rule which can tokenize the input, then
    ///          it will provide this error code.
    not_my_token,

    /// @brief   Provided by a lexical analyzer which cannot process a binary
    ///          integer number because the prefix was not followed by a valid
    ///          set of binary characters.
    /// @details A valid binary number for `cherry` is in the form '0b...' where
    ///          '...' is any count of `0` or `1`. If when trying to read the
    ///          input, the prefix isn't followed by the set of binary
    ///          characters, the binary number is considered ill-formed. This
    ///          error is reported in this case.
    invalid_binary,

    /// @brief   Provided by a lexical analyzer which cannot process an octal
    ///          integer number because the prefix was not followed by a valid
    ///          set of octal characters.
    /// @details A valid octal number for `cherry` is in the form '0...' where
    ///          '...' is any count of `0-7`. If when trying to read the input,
    ///          the prefix isn't followed by the set of octal characters, the
    ///          octal number is considered ill-formed. This error is reported
    ///          in this case.
    invalid_octal,

    /// @brief   Provided by a lexical analzyer which cannot process a
    ///          hexadecimal number because the prefix was not followed by a
    ///          valid set of hexadecimal characters.
    /// @details A valid hexadecimal number for `cherry` is in the form '0x...'
    ///          where the '...' is any count of `a-fA-F0-9`. If when trying to
    ///          read the input, the prefix isn't followed by the set of
    ///          hexadecimal characters, the hexadecimal number is considered
    ///          ill-formed. This error is reported in this case.
    invalid_hexadecimal,

    /// @brief   Provided by a lexical analyzer which cannot process a character
    ///          literal because the embedded unicode character is longer than
    ///          what is normally allowed.
    /// @details Character literals allow unicode characters to be specified by
    ///          using the '\\u...' format, where '...' is a set of hexadecimal
    ///          digits to a count of four which would make the unicode larger
    ///          than what can be stored (this is because 4 hexadecimal digits
    ///          would make the value 16-bit which is UTF-16 compatible).
    invalid_unicode,

    /// @brief   Provided by a lexical analyzer which cannot process a character
    ///          literal because it was not closed with a single quote.
    /// @details A valid character literal is one that is opened by a single
    ///          quote and then closed by a single quote. A character literal is
    ///          variable in length because of the values that it can accept. A
    ///          lexical analyzer can predict its length based on what it reads
    ///          first. It can then read that number of characters of the source
    ///          code, and if it doesn't read the closing quote at the end, then
    ///          this error will be reported.
    invalid_character,

    /// @brief   Provided by a lexical analyzer which cannot process a raw
    ///          string literal because it was not closed with a double quote by
    ///          the end of a line.
    /// @details A valid string literal is one that is opened by a double quote
    ///          and then closed by a double quote by the end of the line. If
    ///          the string literal is not closed by the time the lexical
    ///          analyzer reaches the end of the line, it will report this
    ///          error.
    invalid_raw_string,
};

/// @brief   The error category for lexical analysis related error codes.
/// @details When an error code is created for the lexical analysis `errc`
///          values, there will be this error category to translate those values
///          to their appropriate messages.
struct error_category final : std::error_category {
    /// @brief   Provides the name of this error category.
    /// @details This is required by the interface of the standard error
    ///          category. It is useful for error reporting later on.
    /// @returns The name of this error category.
    const char*
    name() const noexcept override {
        return "cherry::lex::error_category";
    }


    /// @brief   Provides the specific error message for the provided code.
    /// @details This is required by the interface of the standard error
    ///          category. It is useful for error reporting later on.
    /// @param   code The error code that was raised and needs translation.
    /// @returns The message that corresponds to the error code provided.
    std::string
    message(int code) const override {
        switch (static_cast<errc>(code)) {
        case errc::unrecoverable:       return "Unrecoverable";
        case errc::success:             return "Success";
        case errc::failure:             return "Failure";
        case errc::invalid_binary:      return "Invalid Binary Number";
        case errc::invalid_octal:       return "Invalid Octal Number";
        case errc::invalid_hexadecimal: return "Invalid Hexadecimal Number";
        case errc::invalid_unicode:     return "Invalid Unicode Character";
        case errc::invalid_character:   return "Invalid Character Literal";
        case errc::invalid_raw_string:  return "Invalid String Literal";
        }

        return "Unknown";
    }
};


/// @brief   Contains the information needed to indicate the location and type
///          of a lexeme from the source being analyzed by a lexer.
/// @details A token is simply the combination of a lexeme with a token type and
///          line/column location. Tokens are typically read one at a time and
///          kept all the way through parsing until a full expression has been
///          realized by the parser. They mostly serve the purpose of indicating
///          to the parser what is being parsed and as a means to provide error
///          reporting when something unexpected is encountered.
struct token final {
    /// @brief   Contains the snippet of the source code being analyzed.
    /// @details This is a view of that source code. The source code string is
    ///          stored within the current state of the lexer, so it is safe to
    ///          merely take a view of that source. It takes very little memory
    ///          and provides a lot of context to the lexer/parser.
    strview_t lexeme;

    /// @brief   The type of the token as determined by the rule that extracted
    ///          the lexeme and produced this token.
    /// @details The type of the token is based on the lexeme that was extracted
    ///          by a lexical rule of a lexer. That rule will likely have some
    ///          internal function that compares the lexeme to a set of known
    ///          lexemes and produce a type based on the comparison.
    leaf type;

    /// @brief   The line from the source code this lexeme was found.
    /// @details This is primarily used for error reporting and debugging if
    ///          something where to fail near this token, but may be used for
    ///          other purposes as seen fit.
    uint64_t line;

    /// @brief   The column from the line of source code this lexeme was found.
    /// @details This is primarily used for error reporting and debugging if
    ///          something where to fail near this token, but may be used for
    ///          other purposes as seen fit.
    uint64_t column;
};

/// @brief   Compares two tokens.
/// @param   lhs The first token to compare.
/// @param   rhs The second token to compare.
/// @returns True if the tokens *are* equal by all properties; false
///          otherwise.
inline bool
operator==(const token& lhs, const token& rhs) noexcept {
    return lhs.lexeme == rhs.lexeme &&
           lhs.type   == rhs.type   &&
           lhs.line   == rhs.line   &&
           lhs.column == rhs.column;
}

/// @brief   Compares two tokens.
/// @param   lhs The first token to compare.
/// @param   rhs The second token to compare.
/// @returns True if the token *are not* equal by all properties; false
//           otherwise.
inline bool
operator!=(const token& lhs, const token& rhs) noexcept {
    return !(lhs == rhs);
}

/// @brief   Writes a token to a output stream.
/// @param   oss The output stream to write the token to.
/// @param   tkn The token to write to the output stream.
/// @returns The output stream that the token was written to. 
inline std::ostream&
operator<<(std::ostream& oss, const token& tkn) noexcept {
    return oss
        << "Token: {\n"
        << "  lexeme: \"" << tkn.lexeme << "\"\n"
        << "  type:    "  << tkn.type   << '\n'
        << "  line:    "  << tkn.line   << '\n'
        << "  column:  "  << tkn.column << '\n'
        << "}\n";
}


/// @brief   Contains the state of lexical analysis of a single lexing context.
/// @details The state is responsible for providing the lexer information on
///          where the last state of the analysis was, as lexer's in `cherry`
///          do not retain the state themselves. The reason that `cherry` does
///          not retain this information within the lexer is because it makes
///          it decoupled (obviously), allowing the information to be handled
///          by any lexer from any thread. This provides the ability to pause
///          analysis on one thread and pick it up on another through a queue
///          if that level of control is necessary. It also makes the data more
///          readily accessible to other components, allowing them to mutate
///          the data if necessary, but primarily making it easier for the
///          syntactic and semantic analysis to operate with.
struct state final {
    /// @brief   The source code that we're currently analyzing.
    /// @details This should be retained throughout the compilation process for
    ///          multiple reasons. Obviously, it's needed for the purpose of
    ///          lexical analysis to begin with; including lexeme extraction.
    ///          It is also needed for error reporting, similarily to the path
    ///          of the source code.
    /// @sa      token::lexeme
    string_t code{};

    /// @brief   The current line of source code that is being analyzed.
    /// @details The line should be incremented anytime that the lexer, or a
    ///          lexical rule, encounters a newline `\\n` within the source
    ///          code. It should ignore/skip carraige return `\\r` and form
    ///          feed `\\f` characters. Using the `read()` method will do this
    ///          automatically.
    uint64_t line{0};

    /// @brief   The current character, within the current line, of source code
    ///          that is being analyzed.
    /// @details The column should be incremented anytime that the lexer, or a
    ///          lexical rule, reads a character of the source using the method
    ///          `read()` provided by this state.
    uint64_t column{0};

    /// @brief   The current index within the source code string.
    /// @details The line and column are for error reporting purposes, for the
    ///          most part, but this is for actually tracking the location
    ///          within the source code that the lexer is currently at.
    uint64_t index{0};

    /// @brief   Utility function for lexical analysis to check whether the end
    ///          of the source code has been reached.
    /// @returns True if the tracked index is equal to the length of the source
    ///          code; false otherwise.
    bool
    end_of_source() const noexcept {
        return index == code.length();
    }

    /// @brief   Utility function for lexical analysis to check the next source
    ///          code character.
    /// @returns The character at the next index of the source code.
    int32_t
    next_src_char() const noexcept {
        return static_cast<int32_t>(code[index + 1]);
    }

    /// @brief   Utility function for lexical analysis to check the current
    ///          source code character.
    /// @returns The character at the current index of the source code.
    int32_t
    curr_src_char() const noexcept {
        return static_cast<int32_t>(code[index]);
    }

    /// @brief   Utility function for lexical analysis to check the previous
    ///          source code character.
    /// @returns The character at the previous index of the source code.
    int32_t
    prev_src_char() const noexcept {
        return static_cast<int32_t>(code[index - 1]);
    }

    /// @brief   Utility function for lexical analysis to read the a character
    ///          from the source code that the state is managing.
    /// @details This allows the lexical rules to compare the character of the
    ///          source to what they expected to see next. Often this means that
    ///          the lexical rule is done analyzing and should extract the token.
    /// @returns The character that was read from the source code.
    int32_t
    read_src_char() noexcept {
        // We've hit the end already.
        if (index == code.length())
            return '\0';

        auto res = code[index++];
        if (res == '\n') {
            line++;
            column = 0;
        } else column++;

        return res;
    }

    /// @brief   Utility function for lexical analysis to get the remaining
    ///          source code for further lexical analysis.
    /// @returns A view of the remaining source code, starting from the current
    ///          index and ending at the end of the source.
    strview_t
    remaining_source() const noexcept {
        return strview_t(
            code.cbegin() + index,
            code.cend()
        );
    }
    
    /// @brief   Utility function for lexical analysis to track the beginning
    ///          of a token for the extraction of that token.
    /// @details This will record the starting line and starting column within
    ///          the starting line, that the token exists as well as start
    ///          tracking the lexeme of the token.
    void
    start_token() noexcept {
        token_.line   = line;
        token_.column = column;
        start_lexeme();
    }

    /// @brief   Utility function for lexical analysis to extract a token from
    ///          the source code that is being analyzed.
    /// @details This will stop tracking the lexeme of the token and set the
    ///          type of the token the value provided by the caller. It will
    ///          then provide the token that was recorded since calling the
    ///          `start_token()` function.
    /// @param   type The type of the token the caller has determined the
    ///          recorded token to be.
    /// @returns The token that was recorded.
    token
    extract_token(leaf type) noexcept {
        token_.lexeme = extract_lexeme();
        token_.type   = type;
        return token_;
    }

private:
    token    token_{};
    uint64_t lexeme_start_{0};

    void
    start_lexeme() noexcept {
        lexeme_start_ = index;
    }

    strview_t
    extract_lexeme() const noexcept {
        return strview_t(
            code.cbegin() + lexeme_start_,
            code.cbegin() + index
        );
    }
};


/// @brief   Defines the result of lexical analysis by a lexer.
/// @details It is the expected result of a token, with the unexpected result
///          of an error code (which is specific to the lexical analysis). The
///          underlying type is provided by the standard library in C++23, and
///          simply represents a union of the expected type and the error type.
///          It is able to determine which value it actually contains, similar
///          to a `std::variant` but is much simpler than the latter.
using result = std::expected<token, errc>;


namespace detail {

/// @brief  Simplified function pointer declaration for a lexical rule litmus
///         function.
/// @tparam Rule The type of the lexical rule that should be providing the
///         litmus function that some objects may rely on.
template<typename Rule>
using lexrule_litmus_fn = bool(Rule::*)(strview_t) const noexcept;

/// @brief  Simplified function pointer declaration for a lexical rule tokenize
///         function.
/// @tparam Rule The type of the lexical rule that should be providing the
///         tokenize function that some objects may rely on.
template<typename Rule>
using lexrule_tokenize_fn = result(Rule::*)(state&) noexcept;

/// @brief   A concept that verifies that the provided type is a lexical rule.
/// @details The type is a valid lexical rule if it internally provides a
///          member function `litmus` and a member function `tokenize`. The
///          purpose of the lexical rule's `litmus` function is to test the
///          provided input and tell the lexer if that input can be tokenized
///          by that lexical rule. Adjacently, the lexcial rule's `tokenize`
///          function is to actual produce a lexical token to the lexer which
///          will likely be passed to the parser.
/// @tparam  Rule The type that we're requiring to match the definition of a
///          lexical rule.
template<typename Rule>
concept LexicalRule = requires {
    { &Rule::litmus   } -> std::same_as<lexrule_litmus_fn<Rule>>;
    { &Rule::tokenize } -> std::same_as<lexrule_tokenize_fn<Rule>>;
};

} // namespace cherry::lex::detail


/// @brief   A lexical rule that can test the source code input for a comment
///          and tokenize it when the litmus is successful.
/// @details `Cherry` specifically defines a comment to be any piece of source
///          code which start with the hash symbol (`#`). It reads until the end
///          of the current line. Comments are not useful information typically,
///          and as such they are ignored and trashed by the parser. However,
///          the design of the lexer requires that they be tokenized.
struct comment_rule final {
    /// @brief   Checks the `source` if it starts with the hash symbol (`#`).
    /// @param   source The source code input that this rule should test.
    /// @returns True if the hash symbol is the first character of the source
    ///          code string, false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        return source.starts_with('#');
    }

    /// @brief   Tokenizes the comment by reading the input until the end of the
    ///          line of the source code that was received as input.
    /// @param   ctx The current state of lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the comment that was tokenized; no error
    ///          will be produced from this rule.
    result
    tokenize(state& ctx) noexcept {
        // Just loop until we hit a new line.
        ctx.start_token();
        while (should_continue(ctx)) continue;
        return ctx.extract_token(leaf::comment);
    }

private:
    static bool
    should_continue(state& ctx) noexcept {
        return !ctx.end_of_source() &&
                ctx.read_src_char() != '\n';
    }
};

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
        return std::isalnum(ch) || ch == '_';
    }

    static leaf
    get_token_type(strview_t lexeme) noexcept {
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

/// @brief   A lexical rule that can test the source code input for a binary
///          number and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce a binary
///          number from the input. A binary number is any lexeme from the input
///          that starts with '0b' and is followed by a set of zeros and ones.
struct binary_rule final {
    /// @brief   Checks the `source` if it starts with '0b'.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a '0b'; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        return source.starts_with("0b");
    }

    /// @brief   Tokenizes a binary number by reading the input until it no
    ///          longer recognizes the input as a binary number.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the binary number that was tokenized; an
    ///          error may be produced if lexically the format of the number is
    ///          not allowed.
    result
    tokenize(state& ctx) noexcept {
        // Flush the '0b' from the buffer.
        ctx.start_token();
        ctx.read_src_char();
        ctx.read_src_char();

        // If we don't have a binary character after flushing the '0b' that is
        // a lexical error that the user should fix. 
        if (ctx.end_of_source() || !is_binary(ctx.next_src_char()))
            return std::unexpected(errc::invalid_binary);
        
        // Keep reading until we don't have a binary character.
        while (!ctx.end_of_source() && is_binary(ctx.read_src_char()))
            continue;
        return ctx.extract_token(leaf::lv_signed);
    }

private:
    static bool
    is_binary(char ch) noexcept {
        return ch == '0' || ch == '1';
    }
};

/// @brief   A lexical rule that can test the source code input for an octal
///          number and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce an octal
///          number from the input. An octal number is any lexeme from the input
///          that starts with '0' and is followed by a set of numbers from zero
///          to seven.
struct octal_rule final {
    /// @brief   Checks the `source` if it starts with '0' and another octal.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a '0' and another octal; false
    ///          otherwise.
    bool
    litmus(strview_t source) const noexcept {
        return source.starts_with('0') &&
               source.size() > 1 &&
               is_octal(source[1]);
    }

    /// @brief   Tokenizes an octal number by reading the input until it no
    ///          longer recognizes the input as an octal number.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the octal number that was tokenized; an
    ///          error may be produced if lexically the format of the number is
    ///          not allowed.
    result
    tokenize(state& ctx) noexcept {
        // Flush '0' from buffer.
        ctx.start_token();
        ctx.read_src_char();

        // If we don't have an octal character after flushing the '0' that is
        // a lexical error that the user should fix.
        if (ctx.end_of_source() || !is_octal(ctx.code[ctx.index]))
            return std::unexpected(errc::invalid_octal);
        
        // Keep reading until we don't have an octal character.
        while (!ctx.end_of_source() && is_octal(ctx.read_src_char()))
            continue;
        return ctx.extract_token(leaf::lv_signed);
    }

private:
    static bool
    is_octal(char ch) noexcept {
        return ch >= '0' || ch <= '7';
    }
};

/// @brief   A lexical rule that can test the source code input for a decimal
///          number and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce a decimal
///          number from the input. A decimal number is any lexeme from the
///          input that starts with a number from one to nine and is followed by
///          a decimal point and/or a set of numbers from zero to nine.
/// @remarks A decimal number may start with 0 if and only if, it is the only
///          number at the start of the lexeme, or is followed by a decimal
///          point and a set of numbers from zero to nine.
struct decimal_rule final {
    /// @brief   Checks the `source` if it starts with an integer and maybe
    ///          continues with a decimal point.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with an integer and maybe continues
    ///          with a decimal point; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        // If it's a number, then make sure we can read the next character.
        // If the next character is a decimal point, then we are working with
        // a decimal number.
        if (!std::isdigit(source[0]))
            return false;
        
        // Make sure this isn't an octal number.
        if (source[0] == '0' && source.size() > 1)
            return source[1] != 'b' &&
                   source[1] != 'x' &&
                   !std::isdigit(source[1]);
        
        // This is a valid decimal number, we won't make an assumption here
        // about what the next character is.
        return true;
    }

    /// @brief   Tokenizes a decimal number by reading the input until it no
    ///          longer recognizes the input as a decimal number.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the decimal number that was tokenized; an
    ///          error may be produced if lexically the format of the number is
    ///          not allowed.
    result
    tokenize(state& ctx) noexcept {
        // We need to keep all characters of this type of number.
        // We'll read this differently so we can catch a '.' and stop immediately
        // instead of accidentally reading passed it.
        ctx.start_token();
        ctx.read_src_char();
        while (!ctx.end_of_source() && std::isdigit(ctx.curr_src_char()))
            ctx.read_src_char();
        
        // If there isn't a decimal point, the number was just an integer.
        if (ctx.curr_src_char() != '.')
            return ctx.extract_token(leaf::lv_signed);

        // Is there any more numbers after the decimal point?
        if (!std::isdigit(ctx.next_src_char()))
            return ctx.extract_token(leaf::lv_signed);

        // Grab the rest of the digits.
        ctx.read_src_char();
        while (!ctx.end_of_source() && std::isdigit(ctx.curr_src_char()))
            ctx.read_src_char();
        return ctx.extract_token(leaf::lv_decimal);
    }
};

/// @brief   A lexical rule that can test the source code input for a
///          hexadecimal number and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce a
///          hexadecimal number from the input. A hexadecimal number is any
///          lexeme from the input that starts with '0x' and is followed by a
///          set of numbers from zero to nine, or the letters 'a' to 'f'.
struct hexadecimal_rule final {
    /// @brief   Checks the `source` if it starts with '0x'.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a '0x'; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        return source.starts_with("0x");
    }

    /// @brief   Tokenizes a hexadecimal number by reading the input until it no
    ///          longer recognizes the input as a hexadecimal number.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the hexadecimal number that was tokenized;
    ///          an error may be produced if lexically the format of the number
    ///          is not allowed.
    result
    tokenize(state& ctx) noexcept {
        // Flush the '0x' from the buffer.
        ctx.start_token();
        ctx.read_src_char();
        ctx.read_src_char();

        // If we don't have a hexadecimal character after flushing the '0x' that
        // is a lexical error that the user should fix.
        if (ctx.end_of_source() || !is_hexadecimal(ctx.code[ctx.index]))
            return std::unexpected(errc::invalid_hexadecimal);
        
        // Keep reading until we don't have an hexadecimal character.
        while (!ctx.end_of_source() && is_hexadecimal(ctx.read_src_char()))
            continue;
        return ctx.extract_token(leaf::lv_signed);
    }

private:
    static bool
    is_upper_digit(char ch) noexcept {
        return  ch >= 'A' || ch <= 'F';
    }

    static bool
    is_lower_digit(char ch) noexcept {
        return ch >= 'a' || ch <= 'f';
    }

    static bool
    is_hexadecimal(char ch) noexcept {
        return std::isdigit(ch)  ||
               is_upper_digit(ch) ||
               is_lower_digit(ch);
    }
};

/// @brief   A lexical rule that can test the source code input for a character
///          literal and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce a
///          character literal from the input. A character literal is any lexeme
///          from the input that starts with a single quote and is followed by a
///          single character, escaped character, or unicode character, and ends
///          with a single quote.
struct character_rule final {
    /// @brief   Checks the `source` if it starts with a single quote.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a single quote; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        return source.starts_with('\'');
    }

    /// @brief   Tokenizes a character literal by reading the input until it no
    ///          longer recognizes the input as a character literal.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the character literal that was tokenized; an
    ///          error may be produced if lexically the format of the character
    ///          literal is not allowed.
    result
    tokenize(state& ctx) noexcept {
        ctx.start_token();
        ctx.read_src_char(); // Flush '\''.

        // Determine character type.
        if (ctx.curr_src_char() == '\\') {
            ctx.read_src_char(); // Flush backslash.
            if (ctx.curr_src_char() == 'u')
                return tokenize_unicode(ctx);
            
            // We can't have a orphaned escape character.
            if (ctx.curr_src_char() == '\'')
                return std::unexpected(errc::invalid_character);
        }
        
        // Read the next character.
        // If it's not the closing quote, that's an error.
        ctx.read_src_char();
        if (ctx.read_src_char() != '\'')
            return std::unexpected(errc::invalid_character);
        return ctx.extract_token(leaf::lv_character);
    }

private:
    static bool
    is_upper_digit(char ch) noexcept {
        return  ch >= 'A' || ch <= 'F';
    }

    static bool
    is_lower_digit(char ch) noexcept {
        return ch >= 'a' || ch <= 'f';
    }

    static bool
    is_hexadecimal(char ch) noexcept {
        return std::isdigit(ch)  ||
               is_upper_digit(ch) ||
               is_lower_digit(ch);
    }

    static result
    tokenize_unicode(state& ctx) noexcept {
        // Check this up front because it's illegal.
        ctx.read_src_char(); // Flush 'u'
        if (ctx.curr_src_char() == '\'')
            return std::unexpected(errc::invalid_unicode);

        // We can read up to 5 characters, we include the trailing '\''.
        for (auto index = 0; index < 5; index++) {
            if (ctx.read_src_char() == '\'')
                return ctx.extract_token(leaf::lv_character);
            
            if (!is_hexadecimal(ctx.curr_src_char()))
                return std::unexpected(errc::invalid_unicode);
        }

        // We didn't find the closing quote.
        return std::unexpected(errc::invalid_unicode);
    }
};

/// @brief   A lexical rule that can test the source code input for a string
///          literal and tokenize it when the litmus is successful.
/// @details This rule will only tokenize what is necessary to produce a string
///          literal from the input. A string literal is any lexeme from the
///          input that starts with a double quote and ends with a double quote.
struct string_rule final {
    /// @brief   Checks the `source` if it starts with a double quote..
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a double quote; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        return source.starts_with('"');
    }

    /// @brief   Tokenizes a string literal by reading the input until it no
    ///          longer recognizes the input as a string literal.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the string literal that was tokenized; an
    ///          error may be produced if lexically the format of the string
    ///          literal is not allowed.
    result
    tokenize(state& ctx) noexcept {
        // What type of string are we processing?
        ctx.start_token();
        ctx.read_src_char(); // Flush '"'

        // Read until we hit the end of the line or the closing quote.
        ctx.read_src_char();
        while (!ctx.end_of_source() && should_continue(ctx.curr_src_char()))
            ctx.read_src_char();
        
        // If we stopped because it's a newline character, that's an error.
        // Strings must be terminated before the end of the line.
        if (ctx.end_of_source() || ctx.curr_src_char() == '\n')
            return std::unexpected(errc::invalid_raw_string);
        
        ctx.read_src_char(); // This should be the ending double quote.
        return ctx.extract_token(leaf::lv_raw_string);
    }

private:
    static bool
    should_continue(char ch) noexcept {
        return ch != '\n' && ch != '"';
    }
};

/// @brief   A lexical rule that can test the source code input for a operator
///          character and tokenize it when the litmus is successful.
/// @details This rule is greedy and will tokenize the longest known operator
///          for any given input.
/// @remarks This will also handle delimiters!
struct operator_rule final {
    /// @brief   Checks the `source` if it starts with a known operator or
    ///          delimiting character.
    /// @param   source The source code input that this rule should test.
    /// @returns True if the input starts with a known operator or delimiting
    ///          character; false otherwise.
    bool
    litmus(strview_t source) const noexcept {
        static uset_t<wchar_t>
        k_known_ops {
            '+', '-', '*', '/', '%', '=',
            '.', '?', '~', '&', '|', '^',
            '<', '>', '!', '(', ')', '{',
            '}', '[', ']', ',', ';', ':'
        };

        return k_known_ops.find(source[0])
            != k_known_ops.cend();
    }

    /// @brief   Tokenizes an operator or delimiting character by reading the
    ///          input until it no longer recognizes the input as a operator or
    ///          delimiting character.
    /// @param   ctx The current state of the lexical analysis that this rule is
    ///          being applied to.
    /// @returns A token containing the operator or delimiting character that
    ///          was tokenized; an error will not be produced from this lexical
    ///          analyzer.
    result
    tokenize(state& ctx) noexcept {
        return tokenize_single(ctx);
    }

private:
    static result
    tokenize_single(state& ctx) noexcept {
        ctx.start_token();
        switch (ctx.read_src_char()) {
        // Possible doubles or equals.
        case '+': return tokenize_double(ctx, leaf::op_add);
        case '-': return tokenize_double(ctx, leaf::op_sub);
        case '&': return tokenize_double(ctx, leaf::op_bitand);
        case '|': return tokenize_double(ctx, leaf::op_bitor);
        case '<': return tokenize_double(ctx, leaf::op_logless);
        case '>': return tokenize_double(ctx, leaf::op_logmore);
        case '.': return tokenize_double(ctx, leaf::op_access);
        case '*': return tokenize_equals(ctx, leaf::op_mul);
        case '/': return tokenize_equals(ctx, leaf::op_div);
        case '%': return tokenize_equals(ctx, leaf::op_mod);
        case '=': return tokenize_equals(ctx, leaf::op_assign);
        case '~': return tokenize_equals(ctx, leaf::op_bitnot);
        case '^': return tokenize_equals(ctx, leaf::op_bitxor);
        case '!': return tokenize_equals(ctx, leaf::op_lognot);
        
        // Actual singles.
        case '?': return ctx.extract_token(leaf::op_ternary);
        case '(': return ctx.extract_token(leaf::dc_lparen);
        case ')': return ctx.extract_token(leaf::dc_rparen);
        case '{': return ctx.extract_token(leaf::dc_lbrace);
        case '}': return ctx.extract_token(leaf::dc_rbrace);
        case '[': return ctx.extract_token(leaf::dc_lbracket);
        case ']': return ctx.extract_token(leaf::dc_rbracket);
        case ',': return ctx.extract_token(leaf::dc_comma);
        case ';': return ctx.extract_token(leaf::dc_terminator);
        case ':': return ctx.extract_token(leaf::dc_colon);
        }

        // Don't know what this could be.
        return ctx.extract_token(leaf::unknown);
    }

    static result
    tokenize_double(state& ctx, leaf type) noexcept {
        // If the character isn't doubled, jump.
        if (ctx.prev_src_char() != ctx.curr_src_char())
            return tokenize_equals(ctx, type);
        ctx.read_src_char();
        switch (type) {
        // Only true doubles.
        case leaf::op_add: return ctx.extract_token(leaf::op_inc);
        case leaf::op_sub: return ctx.extract_token(leaf::op_dec);

        // Only known triple.
        case leaf::op_access: return tokenize_triple(ctx, leaf::op_cascade);

        // Possible equals.
        case leaf::op_bitand:  return tokenize_equals(ctx, leaf::op_logand);
        case leaf::op_bitor:   return tokenize_equals(ctx, leaf::op_logor);
        case leaf::op_logless: return tokenize_equals(ctx, leaf::op_bitlsh);
        case leaf::op_logmore: return tokenize_equals(ctx, leaf::op_bitrsh);
        default: break;
        }

        // Don't know what this could be.
        return ctx.extract_token(leaf::unknown);
    }

    static result
    tokenize_triple(state& ctx, leaf type) noexcept {
        // If the character isn't tripled, jump.
        if (ctx.prev_src_char() != ctx.curr_src_char())
            return ctx.extract_token(type);
        ctx.read_src_char();
        return ctx.extract_token(leaf::op_ellipsis);
    }

    static result
    tokenize_equals(state& ctx, leaf type) noexcept {
        if (ctx.curr_src_char() != '=')
            return ctx.extract_token(type);
        ctx.read_src_char();
        switch (type) {
        case leaf::op_add:     return ctx.extract_token(leaf::op_add_eq);
        case leaf::op_sub:     return ctx.extract_token(leaf::op_sub_eq);
        case leaf::op_mul:     return ctx.extract_token(leaf::op_mul_eq);
        case leaf::op_div:     return ctx.extract_token(leaf::op_div_eq);
        case leaf::op_mod:     return ctx.extract_token(leaf::op_mod_eq);
        case leaf::op_assign:  return ctx.extract_token(leaf::op_logequals);
        case leaf::op_bitnot:  return ctx.extract_token(leaf::op_bitnot_eq);
        case leaf::op_bitand:  return ctx.extract_token(leaf::op_bitand_eq);
        case leaf::op_bitor:   return ctx.extract_token(leaf::op_bitor_eq);
        case leaf::op_bitxor:  return ctx.extract_token(leaf::op_bitxor_eq);
        case leaf::op_bitlsh:  return ctx.extract_token(leaf::op_bitlsh_eq);
        case leaf::op_bitrsh:  return ctx.extract_token(leaf::op_bitrsh_eq);
        case leaf::op_lognot:  return ctx.extract_token(leaf::op_lognot_eq);
        case leaf::op_logand:  return ctx.extract_token(leaf::op_logand_eq);
        case leaf::op_logor:   return ctx.extract_token(leaf::op_logor_eq);
        case leaf::op_logless: return ctx.extract_token(leaf::op_logless_eq);
        case leaf::op_logmore: return ctx.extract_token(leaf::op_logmore_eq);
        default: break;
        }

        // Don't know what this could be.
        return ctx.extract_token(leaf::unknown);
    }
};

} // namespace cherry::lex
} // namespace cherry


namespace cherry {

/// @brief   Defines a generic lexer which operates based on the definitions of
///          the rules provided to it.
/// @tparam  ...Rules A variadic set of lexical analysis rules which provide the
///          lexer with the ability to tokenize a given source code input string.
/// @remarks The order of the rules matters! The lexical analyzer will
///          automatically skip whitespace.
template<lex::detail::LexicalRule... Rules>
class lexical_analyzer final {
    static void
    skip_whitespace(lex::state& ctx) noexcept {
        while (std::isspace(ctx.curr_src_char()))
            ctx.read_src_char();
    }

    template<size_t Index = 0>
    static std::enable_if_t<Index == sizeof...(Rules), lex::result>
    try_tokenize(lex::state& ctx, std::tuple<Rules...>& t) noexcept {
        return std::unexpected(lex::errc::not_my_token);
    }

    template<size_t Index = 0>
    static std::enable_if_t<Index < sizeof...(Rules), lex::result>
    try_tokenize(lex::state& ctx, std::tuple<Rules...>& t) noexcept {
        skip_whitespace(ctx);
        auto& rule = std::get<Index>(t);
        return rule.litmus(ctx.remaining_source())
            ? rule.tokenize(ctx)
            : try_tokenize<Index + 1>(ctx, t);
    }

public:
    /// @brief   Tokenizes the input as specified by the state.
    /// @param   ctx The information about the current lexical analysis.
    /// @returns The result of the tokenization; if the token not extracted it
    ///          will be an error code.
    static lex::result
    tokenize(lex::state& ctx) noexcept {
        auto t = std::make_tuple(Rules()...);
        return try_tokenize(ctx, t);
    }
};


/// @brief   Type definition for the `cherry` specific lexical analyzer.
/// @details This simply makes it easier to create an instance of the `cherry`
///          spcific lexical analyzer by type defining it with all of the rules
///          that cherry uses to tokenize a given input.
using lexer = lexical_analyzer<
    lex::comment_rule,
    lex::keyword_rule,
    lex::binary_rule,
    lex::octal_rule,
    lex::decimal_rule,
    lex::hexadecimal_rule,
    lex::character_rule,
    lex::string_rule,
    lex::operator_rule
>;


} // namespace cherry