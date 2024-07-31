#pragma once


namespace cherry::ast {

/// @brief 
struct object final {
};


/// @brief   Compares one object to another object.
/// @param   lhs The left hand object to compare.
/// @param   rhs The right hand object to compare.
/// @returns True.
inline bool
operator==(const object& lhs, const object& rhs) noexcept {
    return true;
}

} // namespace cherry::ast