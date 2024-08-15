#pragma once


namespace simular::cherry::lex {

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

    /// @brief   Provided by a lexical analyzer which cannot process a multiline
    ///          string literal because it was not closed with a triple quote by
    ///          the end of a line.
    /// @details A valid string literal is one that is opened by a triple quote
    ///          and then closed by a triple quote by the end of the line. If
    ///          the string literal is not closed by the time the lexical
    ///          analyzer reaches the end of the line, it will report this
    ///          error.
    invalid_ml_string,
};

/// @brief   Writes the error code to an output stream.
/// @param   oss The output stream to write the error code to.
/// @param   e   The error code to write to the output stream.
/// @returns The output stream with the error code written to it.
inline std::ostream&
operator<<(std::ostream& oss, errc e) noexcept {
    switch (e) {
    case errc::unrecoverable:       oss << "unrecoverable";       break;
    case errc::success:             oss << "success";             break;
    case errc::failure:             oss << "failure";             break;
    case errc::not_my_token:        oss << "not_my_token";        break;
    case errc::invalid_binary:      oss << "invalid_binary";      break;
    case errc::invalid_octal:       oss << "invalid_octal";       break;
    case errc::invalid_hexadecimal: oss << "invalid_hexadecimal"; break;
    case errc::invalid_unicode:     oss << "invalid_unicode";     break;
    case errc::invalid_character:   oss << "invalid_character";   break;
    case errc::invalid_raw_string:  oss << "invalid_raw_string";  break;
    case errc::invalid_ml_string:   oss << "invalid_ml_string";   break;
    default:                        oss << "unknown(" << (int)e << ')'; break;
    }
    return oss;
}

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
        case errc::invalid_ml_string:   return "Invalid Multiline Literal";
        }

        return "Unknown";
    }
};

} // namespace simular::cherry::lex