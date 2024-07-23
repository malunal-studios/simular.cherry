#pragma once
#include <limits>
#include "lexer.hpp"


namespace cherry {
namespace detail {

/// @brief   Calculates the upper bounds limit for terminals of the grammar.
/// @returns The signed 16-bit maximum value halved.
inline constexpr int16_t
leaf_upper_limit() noexcept {
    using int16_limit = std::numeric_limits<int16_t>;
    return int16_limit::max() / 2;
}

} // namespace cherry::detail


/// @brief   Defines the non-terminals of the grammar.
/// @details ...
enum struct root : int16_t {
    STARTS_FROM = detail::leaf_upper_limit(),

    // Place non-terminals here.
};

/// @brief   Writes the root to an output stream.
/// @param   oss  The output stream to write the root to.
/// @param   type The root type to write to the output stream.
/// @returns The output stream with the token type written to it.
inline std::ostream&
operator<<(std::ostream& oss, root type) noexcept {
    switch (type) {
    default: break;
    }
    return oss;
}


/// @brief   Represents a grammar symbol, which is just an signed 16-bit
///          integer for the purposes of the language.
/// @details Symbols can be either terminal or non-terminal. Special symbols
///          like `ε` (epsilon) and `$` (final) are considered terminal and are
///          represented by the distinct values `-1` and `-2` respectively.
struct symbol final {
    /// @brief   The constant value of this terminal.
    /// @details This cannot be changed once the symbol is created. The idea is
    ///          that it should be easily copyable because of how small it is.
    const int16_t value;

    /// @brief Allows construction of a symbol with a given value.
    /// @param value The readonly value of the symbol.
    constexpr
    symbol(int16_t value)
        : value{ value }
    { }

    /// @brief Allows construction of a symbol with the given leaf value.
    /// @param value The readonly value of the symbol.
    constexpr
    symbol(leaf value)
        : symbol(static_cast<int16_t>(value))
    { }

    /// @brief Allows construction of a symbol with the given root value.
    /// @param value The readonly value of the symbol.
    constexpr
    symbol(root value)
        : symbol(static_cast<int16_t>(value))
    { }

    /// @brief   Allows this to be coverted implicitly to a 16-bit integer.
    /// @details This is provided as convenience to treat symbols like a 16-bit
    ///          integer.
    constexpr
    operator int16_t() const noexcept {
        return value;
    }

    /// @brief   Compares this symbol with another symbol.
    /// @param   other The symbol to compare to.
    /// @returns True if the symbols have the same value; false otherwise. 
    constexpr bool
    operator==(const symbol& other) const noexcept {
        return value == other.value;
    }

    /// @brief   Compares this symbol with another symbol.
    /// @param   other The symbol to compare to.
    /// @returns True if the symbols *do not* have the same value; false
    ///          otherwise.
    constexpr bool
    operator!=(const symbol& other) const noexcept {
        return !operator==(other);
    }

    /// @brief   Checks if the symbol is a leaf by comparing it to the defined
    ///          upper limit for leaf symbols.
    /// @returns True if the value is less than half of the signed 16-bit
    ///          integer maximum.
    constexpr bool
    is_leaf() const noexcept {
        return value < detail::leaf_upper_limit();
    }
};

/// @brief   Writes a symbol to an output stream.
/// @param   oss The output stream to write the symbol to.
/// @param   sym The symbol to write to the ouptut stream.
/// @returns The output stream with the written symbol.
inline std::ostream&
operator<<(std::ostream& oss, const symbol& sym) noexcept {
    switch (sym.value) {
    case -1: oss << "ε"; return oss;
    case -2: oss << "$"; return oss;
    }

    if (sym.is_leaf())
        oss << static_cast<leaf>(sym.value);
    else oss << static_cast<root>(sym.value);
    return oss;
}


/// @brief   The definition of the epsilon symbol.
/// @details Epsilon informs the grammar and parser that a grammar rule produces
///          nothing, which means it can be skipped when parsing.
inline constexpr symbol
k_epsilon = symbol(-1);

/// @brief   The definition of the final symbol.
/// @details Final informs the grammar and parser that parsing should end; it is
///          the end of the grammar. If there are tokens still unparsed, but
///          this symbol is found on the symbol stack, then parsing stopped
///          unexpectedly; as you can imagine, that's an error.
inline constexpr symbol
k_final = symbol(-2);


/// @brief   Alias for the type that will store the mapped grammar rule
///          production sets.
/// @details This is an ordered multimap with a symbol as the key and a vector
///          of symbols as the value of the key.
using prod_sets_t = omdict_t<symbol, vec_t<symbol>>;

/// @brief   Alias for the type that will store the mapped grammar symbol sets.
/// @details This is the basis for storing FIRST and FOLLOW sets of the grammar
///          of the language that will be parsed. It is an ordered map with a
///          symbol as the key and an ordered set of symbols as the value of the
///          key.
using symb_sets_t = odict_t<symbol, oset_t<symbol>>;


namespace detail {

/// @brief  Simplified function pointer declaration for a grammar rule's
///         production rules.
/// @tparam Rule The grammar rule that will provide the production rules
///         of itself to the grammar.
template<typename Rule>
using grammar_production_fn = prod_sets_t(Rule::*)() const noexcept;

/// @brief   A concept that verifies that the provided type is a grammar rule.
/// @details A type is a valid grammar rule if it provides a function for
///          obtaining its own production rules.
/// @tparam  Rule The type that we're requiring to match the definition of a
///          grammar rule.
template<typename Rule>
concept GrammarRule = requires {
    { &Rule::productions } -> std::same_as<grammar_production_fn<Rule>>;
};

} // namespace cherry::detail


/// @brief   Represents a LR1 grammar.
/// @details An LR1 grammar is responsible for accepting a set of rules and
///          providing the FIRST, FOLLOW, and LR1 items for the language to
///          a parser, such that the parser can create the parsing tables and
///          perform LR1, shift-reduce, parsing.
/// @tparam  ...Rules The grammar rules of the language.
template<detail::GrammarRule... Rules>
class lr1_grammar final {
public:
    /// @brief   Collects all of the grammar rule's production rules, such that
    ///          they can be easily iterated and provide utility for creating
    ///          the FIRST and FOLLOW sets.
    /// @returns The mapping of the grammar rule's head, to each of the bodies
    ///          that the production can expand to.
    static const prod_sets_t&
    prod_sets() noexcept {
        static prod_sets_t result;
        if (result.size() != 0)
            return result;
        
        // Generate the production sets.
        std::apply([](auto&&... args) {
            // Fold expression which handles mapping all the grammar rules.
            // The rule should provide all of its productions and we'll merge
            // them into our full map.
            (result.merge(args.productions()), ...);
        }, std::make_tuple(Rules()...));
        return result;
    }
};

} // namespace cherry

