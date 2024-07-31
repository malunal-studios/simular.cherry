#pragma once


namespace cherry::ast {

/// @brief 
struct function final {
};


/// @brief   Compares one function to another function.
/// @param   lhs The left hand function to compare.
/// @param   rhs The right hand function to compare.
/// @returns True.
inline bool
operator==(const function& lhs, const function& rhs) noexcept {
    return true;
}

} // namespace cherry::ast