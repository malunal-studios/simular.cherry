#pragma once


namespace cherry::ast {

/// @brief 
struct enumeration final {
};


/// @brief   Compares one enumeration to another enumeration.
/// @param   lhs The left hand enumeration to compare.
/// @param   rhs The right hand enumeration to compare.
/// @returns True.
inline bool
operator==(const enumeration& lhs, const enumeration& rhs) noexcept {
    return true;
}

} // namespace cherry::ast