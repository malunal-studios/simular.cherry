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
class symbol final {
    /// @brief   The value of this terminal.
    /// @details This tells informs the grammar and the parser whether the
    ///          symbol is terminal or non-terminal.
    int16_t value;

public:
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
    switch ((int16_t)sym) {
    case -1: oss << "ε"; return oss;
    case -2: oss << "$"; return oss;
    }

    if (sym.is_leaf())
        oss << static_cast<leaf>((int16_t)sym);
    else oss << static_cast<root>((int16_t)sym);
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
    using symb_seq_t = vec_t<symbol>;
    using symb_span_t = std::span<const symbol>;

    static symb_seq_t
    firsts_of(
        const symb_span_t& sequence,
        const symb_sets_t& firsts
    ) noexcept {
        symb_seq_t result;
        for (const auto& sym : sequence) {
            if (sym.is_leaf()) {
                result.emplace_back(sym);
                return result;
            }

            const auto& set = firsts.at(sym);
            for (const auto& first : set)
                result.emplace_back(first);
            
            if (!set.contains(k_epsilon))
                return result;
            
            // Remove epsilon if contained.
            result.erase(std::remove(
                result.begin(),
                result.end(),
                k_epsilon
            ));
        }

        result.emplace_back(k_epsilon);
        return result;
    }

    static void
    update_firsts(
        const symbol&      head,
        const symb_seq_t&  body,
        const prod_sets_t& prods,
              symb_sets_t& results
    ) noexcept {
        auto& set = results[head];
        auto  sym = body[0];
        if (sym.is_leaf()) {
            set.emplace(sym);
            return;
        }

        // Recursively add symbols to rules we depend on.
        if (!results.contains(sym)) {
            const auto bucket = prods.equal_range(sym);
            for (auto itr = bucket.first; itr != bucket.second; itr++)
                update_firsts(sym, itr->second, prods, results);
        }

        // Already inserted, so just get it.
        for (const auto& val : results[sym])
            set.emplace(val);
    }

    static bool
    update_follows(
        const symbol&      head,
        const symb_seq_t&  body,
        const prod_sets_t& prods,
        const symb_sets_t& firsts,
              symb_sets_t& results
    ) noexcept {
        bool proceed = false;
        for (auto index = 0; index < body.size(); index++) {
            const auto& curr = body[index];
            if (curr.is_leaf())
                continue;

            // Get follow set for the current nonterminal. 
            auto& curr_follows = results[curr];

            // Slice the body at current index and get all firsts for that
            // sequence.
            const auto& slice_of_body   = std::span{body}.subspan(index + 1);
            const auto& firsts_sequence = firsts_of(slice_of_body, firsts);
            for (const auto& first : firsts_sequence) {
                if (first == k_epsilon)
                    continue;
                
                if (curr_follows.emplace(first).second)
                    proceed = true;
            }

            // Check if that firsts contains epsilon, if it does, add all of the follow
            // for the head into the current follows.
            const auto itr = std::find(
                firsts_sequence.cbegin(),
                firsts_sequence.cend(),
                k_epsilon
            );

            if (itr != firsts_sequence.cend()) {
                auto& this_follows = results[head];
                for (const auto& follow : this_follows) {
                    if (curr_follows.emplace(follow).second)
                        proceed = true;
                }
            }
        }

        return proceed;
    }

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

    /// @brief   Generates the FIRST sets for all of the grammar rules.
    /// @details The FIRST set of any grammar rule represents what symbols can
    ///          start that rule. If the rule starts with a non-terminal, then
    ///          the rules must be traversed in a depth first manner to find
    ///          where the rules terminate. Those terminals that are found, are
    ///          the firsts of the rule actually being processed.
    /// @returns The generated sets.
    static const symb_sets_t&
    first_sets() noexcept {
        static symb_sets_t result;
        if (result.size() != 0)
            return result;
        
        const auto& prods = prod_sets();
        for (const auto& pair : prods)
            update_firsts(pair.first, pair.second, prods, result);
        return result;
    }

    /// @brief 
    /// @return 
    static const symb_sets_t&
    follow_sets() noexcept {
        static symb_sets_t result;
        if (result.size() != 0)
            return result;

        // Insert the final symbol as follow of start symbol.
        result[detail::leaf_upper_limit() + 1].emplace(k_final);

        // Iterate each production rule of the grammar and
        // update the follow sets.
        const auto& prods = prod_sets();
        const auto& firsts = first_sets();

        bool proceed;
        do {
            // Handle in reverse because it'll actually catch everything properly.
            for (auto itr = prods.crbegin(); itr != prods.crend(); itr++) {
                const auto& head = itr->first;
                const auto& body = itr->second;
                proceed = update_follows(head, body, prods, firsts, result);
            }
        } while (proceed);

        return result;
    }
};

} // namespace cherry

