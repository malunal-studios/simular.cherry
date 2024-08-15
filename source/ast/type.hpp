#pragma once
#include "expression.hpp"
#include "paths.hpp"


namespace cherry::ast {

/// @brief   Type abstract syntax tree node.
/// @details A type is any path that leads to a function, or a pointer to-,
///          reference of-, or array of- an object type or primitive type.
struct type {
    struct fn;
    struct arr;
    struct ref;

    /// @brief   Defines the variant subtypes for this type node.
    /// @details They are designed to be used to downcast any given type node
    ///          to its subtype variant: `raw`, `fn`, `arr`, `ref`.
    enum class variant { raw, fn, arr, ref };

    /// @brief A list of the segments that were parsed from the source code
    ///        input that make up the path to this type.
    vec_t<uptr_t<segment>> segments;

    /// @brief   A default constructor for when the type is raw and not a
    ///          variant.
    /// @details Sometimes a type may be raw where there is no function
    ///          parameters, array dimensions, or reference depths. In this
    ///          case, we should allow and provide a raw type.
    type() : type(variant::raw) {
    }

    /// @brief   Gets the subtype for this type node.
    /// @returns The variant subtype for this type node.
    variant
    subtype() const noexcept {
        return subtype_;
    }

protected:
    /// @brief   The variant subtype of this type node.
    /// @details Available to the children incase they need to modify it or
    ///          provide it directly to something without calling the function
    ///          which is designed for non-inheriters.
    variant subtype_;

    /// @brief Constructs this type from the subtype provided.
    /// @param subtype The subtype, indicating that this type holds more
    ///        information than its base implies.
    explicit
    type(variant subtype) noexcept
        : subtype_{ subtype }
    { }
};


/// @brief   Function type abstract syntax tree node variant.
/// @details A type which represents a function and contains the necessary
///          information to reconstruct that function's prototype.
struct type::fn final : type {
    /// @brief   Constructs this variant type node.
    /// @details Simply put, this is a function type variant, so it provides the
    ///          base with that variant to store.
    fn() : type(variant::fn) {
    }

    /// @brief   A list of types that the function expects.
    /// @details These come from the parameters of the function's prototype and
    ///          uniquely represent the identity of the function, allowing for
    ///          overloads based on number and types of parameters.
    vec_t<uptr_t<type>> inputs;

    /// @brief   The return type the function produces.
    /// @details This comes from the function's prototype, just like the inputs.
    ///          In most typical cases, it cannot be used to uniquely identify
    ///          one function from another.
    opt_t<uptr_t<type>> output;
};


/// @brief   Array type abstract syntax tree node variant.
/// @details A type which represents an array and contains the necessary
///          information to resonstruct that array's dimensions.
struct type::arr final : type {
    /// @brief   Constructs this variant type node.
    /// @details Simply put, this is an array type variant, so it provides the
    ///          base with that variant to store.
    arr() : type(variant::arr) {
    }

    /// @brief   A list of the dimensions that make up the array type.
    /// @details The dimensions represent how much data the array stores in
    ///          total across each 'axis' that it has.
    // vec_t<expression> dimensions;
};


/// @brief   Reference type abstract syntax tree node variant.
/// @details A type which represents a reference of a type. It's a flat object
///          that relies on a depth list, where each entry represents if the
///          current depth is a pointer or a normal reference. The deeper the
///          depth of references, the more pointers/references where found on
///          the type.
struct type::ref final : type {
    /// @brief   Constructs this variant type node.
    /// @details Simply put, this is a reference type variant, so it provides
    ///          the base with that variant to store.
    ref() : type(variant::ref) {
    }

    /// @brief   The reference depth for this type.
    /// @details Contains each pointer/reference that was associated with the
    ///          type when it was parsed. Each depth entry represents whether
    ///          it was a pointer or normal reference at that depth.
    /// @remarks 0/False represents normal reference, while 1/True represents
    ///          a pointer.
    vec_t<bool> depth;
};


// Forwarded.
bool operator==(const type&, const type&) noexcept;
bool operator!=(const type&, const type&) noexcept;
std::ostream& operator<<(std::ostream&, const type&) noexcept;


/// @brief   Compares one function type variant to another.
/// @param   lhs The left hand function type variant to compare.
/// @param   rhs The right hand function type variant to compare.
/// @returns True if both function type variants have the same segments, inputs,
///          and output; otherwise false.
inline bool
operator==(
    const type::fn& lhs,
    const type::fn& rhs
) noexcept {
    // Both must have the same number of inputs
    if (lhs.inputs.size() != rhs.inputs.size())
        return false;

    // Both must have an output or neither have an output.
    if (lhs.output.has_value() != rhs.output.has_value())
        return false;
    
    // Check each input.
    for (auto index = 0; index < lhs.inputs.size(); index++)
        if (*lhs.inputs[index] != *rhs.inputs[index])
            return false;

    // Compare both outputs.
    return *lhs.output.value() == *rhs.output.value();
}

/// @brief   Compares one function type variant to another.
/// @param   lhs The left hand function type variant to compare.
/// @param   rhs The right hand function type variant to compare.
/// @returns True if both function type variants *do not* have the same inputs
///          and output; otherwise false.
inline bool
operator!=(
    const type::fn& lhs,
    const type::fn& rhs
) noexcept {
    return !operator==(lhs, rhs);
}


/// @brief   Compares one array type variant to another.
/// @param   lhs The left hand array type variant to compare.
/// @param   rhs The right hand array type variant to compare.
/// @returns True if both array type variants have the same inputs; otherwise
///          false.
inline bool
operator==(
    const type::arr& lhs,
    const type::arr& rhs
) noexcept {
    // TODO: compare dimensions when implemented.
    return true;
}

/// @brief   Compares one array type variant to another.
/// @param   lhs The left hand array type variant to compare.
/// @param   rhs The right hand array type variant to compare.
/// @returns True if both array type variants *do not* have the same inputs;
///          otherwise false.
inline bool
operator!=(
    const type::arr& lhs,
    const type::arr& rhs
) noexcept {
    return !operator==(lhs, rhs);
}


/// @brief   Compares one reference type variant to another.
/// @param   lhs The left hand reference type variant to compare.
/// @param   rhs The right hand reference type variant to compare.
/// @returns True if both reference types have the same underlying type;
///          otherwise false.
inline bool
operator==(
    const type::ref& lhs,
    const type::ref& rhs
) noexcept {
    // Must have same depth.
    if (lhs.depth.size() != rhs.depth.size())
        return false;
    
    // Check each depth value.
    for (auto index = 0; index < lhs.depth.size(); index++)
        if (lhs.depth[index] != rhs.depth[index])
            return false;

    return true;
}

/// @brief   Compares one reference type variant to another.
/// @param   lhs The left hand reference type variant to compare.
/// @param   rhs The right hand reference type variant to compare.
/// @returns True if both reference types *do not* have the same underlying
///          type; otherwise false.
inline bool
operator!=(
    const type::ref& lhs,
    const type::ref& rhs
) noexcept {
    return !operator==(lhs, rhs);
}


/// @brief   Compares one type to another type.
/// @param   lhs The left hand type to compare.
/// @param   rhs The right hand type to compare.
/// @returns True if both types have the same variant and segments; otherwise
///          false.
inline bool
operator==(const type& lhs, const type& rhs) noexcept {
    if (lhs.subtype() != rhs.subtype())
        return false;

    // Both must have the same number of segments.
    if (lhs.segments.size() != rhs.segments.size())
        return false;

    // Check each segment.
    for (auto index = 0; index < lhs.segments.size(); index++)
        if (*lhs.segments[index] != *rhs.segments[index])
            return false;
    
    // Const references do not slice the object!
    switch (lhs.subtype()) {
    case type::variant::fn: {
        auto lhs_fn = static_cast<const type::fn*>(&lhs);
        auto rhs_fn = static_cast<const type::fn*>(&rhs);
        return operator==(*lhs_fn, *rhs_fn);
    }
    case type::variant::arr: {
        auto lhs_arr = static_cast<const type::arr*>(&lhs);
        auto rhs_arr = static_cast<const type::arr*>(&rhs);
        return operator==(*lhs_arr, *rhs_arr);
    }
    case type::variant::ref: {
        auto lhs_ref = static_cast<const type::ref*>(&lhs);
        auto rhs_ref = static_cast<const type::ref*>(&rhs);
        return operator==(*lhs_ref, *rhs_ref);
    }
    }

    return true;
}

/// @brief   Compares one type to another type.
/// @param   lhs The left hand type to compare.
/// @param   rhs The right hand type to compare.
/// @returns True if both types *do not* have the same variant and segments;
///          otherwise false.
inline bool
operator!=(const type& lhs, const type& rhs) noexcept {
    return !operator==(lhs, rhs);
}


/// @brief   Writes the function type abstract syntax tree node variant to the
///          provided output stream.
/// @param   oss The output stream to write the type node to.
/// @param   t The type node to write to the output stream.
/// @returns The output stream with the type node written to it.
inline std::ostream&
operator<<(std::ostream& oss, const type::fn& t) noexcept {
    oss << "(";
    if (!t.inputs.empty()) {
        for (auto index = 0; index < t.inputs.size() - 1; index++)
            oss << *t.inputs[index] << ",";
        oss << *t.inputs.back();
    }
    oss << ")";
    if (t.output.has_value())
        oss << " : " << *t.output.value();
    return oss;
}

/// @brief   Writes the array type abstract syntax tree node variant to the
///          provided output stream.
/// @param   oss The output stream to write the type node to.
/// @param   t The type node to write to the output stream.
/// @returns The output stream with the type node written to it.
inline std::ostream&
operator<<(std::ostream& oss, const type::arr& t) noexcept {
    // TODO: implement expressions.
    oss << "[]";
    return oss;
}

/// @brief   Writes the function type abstract syntax tree node variant to the
///          provided output stream.
/// @param   oss The output stream to write the type node to.
/// @param   t The type node to write to the output stream.
/// @returns The output stream with the type node written to it.
inline std::ostream&
operator<<(std::ostream& oss, const type::ref& t) noexcept {
    for (const auto& ptrref : t.depth)
        oss << (ptrref ? "*" : "&");
    return oss;    
}

/// @brief   Writes the type abstract syntax tree node to the provided output
///          stream.
/// @param   oss The output stream to write the type node to.
/// @param   t The type node to write to the output stream.
/// @returns The output stream with the type node written to it.
inline std::ostream&
operator<<(std::ostream& oss, const type& t) noexcept {
    if (!t.segments.empty()) {
        for (auto index = 0; index < t.segments.size() - 1; index++)
            oss << t.segments[index] << ".";
        oss << t.segments.back();
    }

    switch (t.subtype()) {
    case type::variant::fn:  oss << *static_cast<const type::fn*>(&t);  break;
    case type::variant::arr: oss << *static_cast<const type::arr*>(&t); break;
    case type::variant::ref: oss << *static_cast<const type::ref*>(&t); break;
    }

    return oss;
}

} // namespace cherry::ast