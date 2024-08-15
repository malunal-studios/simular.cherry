#pragma once


namespace simular::cherry {

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

    /// @brief   Defines a literal value that is an *interpolated* wide string.
    /// @details This is the same as a *raw* string, but this string contains
    ///          pieces that should be parsed and evaluated before the string
    ///          is finalized.
    lv_int_string,

    /// @brief   Defines a literal value that is a *multiline* wide string.
    /// @details This is slightly different from *raw* string, as the whitespace
    ///          will be trimmed during evaluation.
    lv_ml_string,
    
    /// @brief   Defines a literal value that is a *multiline interpolated* wide
    //           string.
    /// @details This is slightly different from *raw* string, as the whitespace
    ///          will be trimmed during evaluation. However, this is also an
    ///          interpolated string, which follows those rules.
    lv_mli_string,

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
    case leaf::lv_ml_string:  oss << "lv_ml_string("  << (int)type << ')'; break;
    case leaf::lv_mli_string: oss << "lv_mli_string(" << (int)type << ')'; break;
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

} // namespace simular::cherry