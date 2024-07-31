#pragma once


namespace cherry::ast {

/// @brief 
struct extension final {
};

/// @brief   Compares one extension to another extension.
/// @param   lhs The left hand extension to compare.
/// @param   rhs The right hand extension to compare.
/// @returns True.
inline bool
operator==(const extension& lhs, const extension& rhs) noexcept {
    return true;
}

} // namespace cherry::ast