#pragma once
#include "../types.hpp"


namespace cherry::ast {

// Forward declaration.
struct type;
bool operator==(const type&, const type&) noexcept;
bool operator!=(const type&, const type&) noexcept;
std::ostream& operator<<(std::ostream&, const type&) noexcept;


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
struct segment {
    struct primitive;
    struct generic;

    /// @brief   Defines the different subtypes of segments.
    /// @details 
    enum struct variant { primitive, generic };

    /// @brief   Gets the subtype variant of this segment.
    /// @returns The subtype variant of this segment.
    variant
    subtype() const noexcept {
        return subtype_;
    }

protected:
    variant subtype_;

    explicit
    segment(variant subtype) noexcept
        : subtype_{ subtype }
    { }
};


/// @brief   Primitive segment abstract syntax tree node variant.
/// @details ...
struct segment::primitive final : segment {
    /// @brief   Defines the different supported primitive types.
    /// @details These match up with the leaf types that are defined within the
    ///          lexer, that way we can assure those keywords are treated properly.
    enum value {
        p_bool,   p_char,
        p_int8,   p_int16,  p_int32,  p_int64,
        p_uint8,  p_uint16, p_uint32, p_uint64,
        p_single, p_double, p_string, p_void,

        k_last_primitive
    };

    /// @brief   The primitive's value.
    /// @details This represents the primitive in its entirety. We don't need to
    ///          keep the lexeme for the primitive around any longer.
    value val;

    /// @brief Constructs the primitive segment from the given value.
    /// @param val The value of the primitive.
    primitive(value val = p_bool) noexcept
        : segment(variant::primitive)
        , val{ val }
    { }
};


/// @brief   Generic segment abstract syntax tree node variant.
/// @details ...
struct segment::generic final : segment {
    /// @brief Default constructor, yup.
    generic() noexcept : segment(variant::generic) {
    }

    /// @brief   The generic arguments provided along with the identifier if
    ///          there were any.
    /// @details This is shared between path expressions and type paths where
    ///          both need to drill down a path of module/object accesses. Some
    ///          objects may require generic arguments to them, this tracks what
    ///          the values where for those.
    vec_t<uptr_t<type>> inputs;

    /// @brief   The name of the module/object that was accessed with this
    ///          segment.
    /// @details This comes directly from a token that was lexically and
    ///          syntactically analyzed from the source code.
    strview_t name;
};

/// @brief   Defines a path expression consisting of segments separated by the
///          access operator.
/// @details Each segment may be an identifier or an identifier with generic
///          arguments, as it may need to drill down through known types which
///          have generic parameters.
struct path_expr final {
    /// @brief   A list of the segments that were parsed from the source code
    ///          input that make up this path expression.
    vec_t<uptr_t<segment>> segments;
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


/// @brief   Compares one primitive segment to another.
/// @param   lhs The left hand segment to compare.
/// @param   rhs The right hand segment to compare.
/// @returns True if both segments have the same value; otherwise false.
inline bool
operator==(
    const segment::primitive& lhs,
    const segment::primitive& rhs
) noexcept {
    return lhs.val == rhs.val;
}

/// @brief   Compares one primitive segment to another.
/// @param   lhs The left hand segment to compare.
/// @param   rhs The right hand segment to compare.
/// @returns True if both segments *do not* have the same value; otherwise
///          false.
inline bool
operator!=(
    const segment::primitive& lhs,
    const segment::primitive& rhs
) noexcept {
    return !operator==(lhs, rhs);
}


/// @brief   Compares one generic segment to another.
/// @param   lhs The left hand generic segment to compare.
/// @param   rhs The right hand generic segment to compare.
/// @returns True if both segments have the same name and inputs; otherwise
///          false.
inline bool
operator==(
    const segment::generic& lhs,
    const segment::generic& rhs
) noexcept {
    if (lhs.name != rhs.name)
        return false;

    // Both must have the same number of inputs.
    if (lhs.inputs.size() != rhs.inputs.size())
        return false;
    
    // Compare all the inputs.
    for (auto index = 0; index < lhs.inputs.size(); index++)
        if (*lhs.inputs[index] != *rhs.inputs[index])
            return false;
    
    return true;
}

/// @brief   Compares one generic segment to another.
/// @param   lhs The left hand generic segment to compare.
/// @param   rhs The right hand generic segment to compare.
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
    // Both segments need to have the same subtype.
    if (lhs.subtype() != rhs.subtype())
        return false;

    switch (lhs.subtype()) {
    case segment::variant::primitive: {
        auto lhs_prm = static_cast<const segment::primitive*>(&lhs);
        auto rhs_prm = static_cast<const segment::primitive*>(&rhs);
        return operator==(*lhs_prm, *rhs_prm);
    }
    case segment::variant::generic: {
        auto lhs_gen = static_cast<const segment::generic*>(&lhs);
        auto rhs_gen = static_cast<const segment::generic*>(&rhs);
        return operator==(*lhs_gen, *rhs_gen);
    }
    }

    // Technically unreachable.
    return false;
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
    // Both paths must have same number of segments.
    if (lhs.segments.size() != rhs.segments.size())
        return false;
    
    // Compare all segments.
    for (auto index = 0; index < lhs.segments.size(); index++)
        if (*lhs.segments[index] != *rhs.segments[index])
            return false;

    return true;
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


/// @brief   Writes the given primitive segment to the given output stream.
/// @param   oss The output stream to write the segment to.
/// @param   seg The segment to write to the output stream.
/// @returns The output stream with the segment written to it.
inline std::ostream&
operator<<(std::ostream& oss, const segment::primitive& seg) noexcept {
    switch (seg.val) {
    case segment::primitive::p_bool:   oss << "bool";   break;
    case segment::primitive::p_char:   oss << "char";   break;
    case segment::primitive::p_int8:   oss << "int8";   break;
    case segment::primitive::p_int16:  oss << "int16";  break;
    case segment::primitive::p_int32:  oss << "int32";  break;
    case segment::primitive::p_int64:  oss << "int64";  break;
    case segment::primitive::p_uint8:  oss << "uint8";  break;
    case segment::primitive::p_uint16: oss << "uint16"; break;
    case segment::primitive::p_uint32: oss << "uint32"; break;
    case segment::primitive::p_uint64: oss << "uint64"; break;
    case segment::primitive::p_single: oss << "single"; break;
    case segment::primitive::p_double: oss << "double"; break;
    case segment::primitive::p_string: oss << "string"; break;
    case segment::primitive::p_void:   oss << "void";   break;
    }

    return oss;
}

/// @brief   Writes the given generic segment to the given output stream.
/// @param   oss The output stream to write the segment to.
/// @param   seg The segment to write to the output stream.
/// @returns The output stream with the segment written to it.
inline std::ostream&
operator<<(std::ostream& oss, const segment::generic& seg) noexcept {
    oss << seg.name;
    if (!seg.inputs.empty()) {
        oss << "<";
        for (auto index = 0; index < seg.inputs.size() - 1; index++)
            oss << *seg.inputs[index] << ",";
        oss << *seg.inputs.back() << ">";
    }
    return oss;
}

/// @brief   Writes the given segment to the given output stream.
/// @param   oss The output stream to write the segment to.
/// @param   seg The segment to write to the output stream.
/// @returns The output stream with the segment written to it.
inline std::ostream&
operator<<(std::ostream& oss, const segment& seg) noexcept {
    switch (seg.subtype()) {
    case segment::variant::primitive: oss << *static_cast<const segment::primitive*>(&seg); break;
    case segment::variant::generic:   oss << *static_cast<const segment::generic*>(&seg);   break;
    }

    return oss;
}


/// @brief   Writes the given path expression to the given output stream.
/// @param   oss The output stream to write the path expression to.
/// @param   path The path expression to write to the output stream.
/// @returns The output stream with the path expression written to it.
inline std::ostream&
operator<<(std::ostream& oss, const path_expr& path) noexcept {
    if (path.segments.empty())
        return oss;
    
    for (auto index = 0; index < path.segments.size() - 1; index++)
        oss << *path.segments[index] << ".";
    oss << *path.segments.back();
    return oss;
}

} // namespace cherry::ast