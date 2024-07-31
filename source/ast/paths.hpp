#pragma once
#include "../types.hpp"


namespace cherry::ast {

// Forward declaration.
struct type;

/// @brief   Defines a simple path consisting of identifiers separated by the
///          access operator.
/// @details This is an abstract syntax tree node which provides the individual
///          segments of the path for semantic analysis.
struct simple_path final {
    /// @brief A list of the segments that were parsed from the source code
    ///        input that make up this simple path.
    /// @note  No need to store any other nodes, just store these directly.
    vec_t<strview_t> segments;
};

/// @brief   Defines a single segment of a generic path.
/// @details A segment will consist an identifier and a list of generic
///          arguments, if they were provided.
struct segment final {
    /// @brief   Defines the different supported primitive types.
    /// @details These match up with the leaf types that are defined within the
    ///          lexer, that way we can assure those keywords are treated properly.
    enum class primitive {
        p_bool,   p_char,
        p_int8,   p_int16,  p_int32,  p_int64,
        p_uint8,  p_uint16, p_uint32, p_uint64,
        p_single, p_double, p_string,

        k_last_primitive
    };

    /// @brief   A segment variant for generic identifiers.
    /// @details Generic segments may be a module (without generics), an
    ///          object, or function.
    struct generic final {
        /// @brief   The generic arguments provided along with the identifier if
        ///          there were any.
        /// @details This is shared between path expressions and type paths where
        ///          both need to drill down a path of module/object accesses. Some
        ///          objects may require generic arguments to them, this tracks what
        ///          the values where for those.
        vec_t<type> inputs;

        /// @brief   The name of the module/object that was accessed with this
        ///          segment.
        /// @details This comes directly from a token that was lexically and
        ///          syntactically analyzed from the source code.
        strview_t name;
    };

    /// @brief   Defines the variants that a segment may store.
    /// @details At the beginning of a path expression, or even a type
    ///          expression, the very first segment may well be a primitive
    ///          type instead of a generic/non-generic identifier. For this
    ///          reason a variant is provided to check which one was parsed.
    using segment_variant = std::variant<primitive, generic>;
    
    /// @brief   Stores the variant of this segment.
    /// @details Could be a primitive segment, or a generic segment.
    segment_variant variant;
};

/// @brief   Defines a path expression consisting of segments separated by the
///          access operator.
/// @details Each segment may be an identifier or an identifier with generic
///          arguments, as it may need to drill down through known types which
///          have generic parameters.
struct path_expr final {
    /// @brief   A list of the segments that were parsed from the source code
    ///          input that make up this path expression.
    vec_t<segment> segments;
};


/// @brief   Compares one simple path to another.
/// @param   lhs The left hand path to compare.
/// @param   rhs The right hand path to compare.
/// @returns True if the paths are the same reference or if they have the exact
///          same path segments; false otherwise.
inline bool
operator==(const simple_path& lhs, const simple_path& rhs) noexcept {
    return lhs.segments == rhs.segments;
}

/// @brief   Compares one simple path to another.
/// @param   lhs The left hand path to compare.
/// @param   rhs The right hand path to compare.
/// @returns True if the paths are *not* the same reference or if they *do not*
///          have the exact same path segments; false otherwise.
inline bool
operator!=(const simple_path& lhs, const simple_path& rhs) noexcept {
    return !operator==(lhs, rhs);
}

/// @brief   Writes the node to the output stream.
/// @param   oss  The output stream to write the node to.
/// @param   path The node to write to the output stream.
/// @returns The output stream with the node written to it.
inline std::ostream&
operator<<(std::ostream& oss, const simple_path& path) noexcept {
    auto index    = 0;
    auto original = oss.width();
    oss.width(original + 4);
    oss << "path\n";
    for (const auto& segment : path.segments) {
        if (index++ >= path.segments.size() - 1)
            break;
        
        oss.width(original + segment.length() + 3);
        oss << "|_ " << segment << std::endl;
    }
    
    const auto& last = path.segments[path.segments.size() - 1];
    oss.width(original + last.length() + 3);
    oss << "|_ " << last << std::endl;
    oss.width(original);
    return oss;
}


/// @brief   Compares one segment generic to another.
/// @param   lhs The left hand segment generic to compare.
/// @param   rhs The right hand segment generic to compare.
/// @returns True if both segments generics have the same name and inputs;
///          otherwise false.
inline bool
operator==(
    const segment::generic& lhs,
    const segment::generic& rhs
) noexcept {
    return lhs.name   == rhs.name &&
           lhs.inputs == rhs.inputs;
}

/// @brief   Compares one segment generic to another.
/// @param   lhs The left hand segment generic to compare.
/// @param   rhs The right hand segment generic to compare.
/// @returns True if both segments generics *do not* have the same name and
///          inputs; otherwise false.
inline bool
operator!=(
    const segment::generic& lhs,
    const segment::generic& rhs
) noexcept {
    return !operator==(lhs, rhs);
}


/// @brief   Compares one segment to another segment.
/// @param   lhs The left hand segment to compare.
/// @param   rhs The right hand segment to compare.
/// @returns True if both segments have the same variant; otherwise false.
inline bool
operator==(const segment& lhs, const segment& rhs) noexcept {
    return lhs.variant == rhs.variant;
}

/// @brief   Compares one segment to another segment.
/// @param   lhs The left hand segment to compare.
/// @param   rhs The right hand segment to compare.
/// @returns True if both segments *do not* have the same variant; otherwise
///          false.
inline bool
operator!=(const segment& lhs, const segment& rhs) noexcept {
    return !operator==(lhs, rhs);
}


/// @brief   Compares one path expression to another.
/// @param   lhs The left hand path expression to compare.
/// @param   rhs The right hand path expression to compare.
/// @returns True if both path expressions have the same segments; otherwise
///          false.
inline bool
operator==(const path_expr& lhs, const path_expr& rhs) noexcept {
    return lhs.segments == rhs.segments;
}

/// @brief   Compares one path expression to another.
/// @param   lhs The left hand path expression to compare.
/// @param   rhs The right hand path expression to compare.
/// @returns True if both path expressions *do not* have the same segments;
///          otherwise false.
inline bool
operator!=(const path_expr& lhs, const path_expr& rhs) noexcept {
    return !operator==(lhs, rhs);
}

} // namespace cherry::ast