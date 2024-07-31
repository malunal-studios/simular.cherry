#pragma once


namespace cherry::ast {

/// @brief 
struct variable final {
};


/// @brief   Compares one variable to another variable.
/// @param   lhs The left hand variable to compare.
/// @param   rhs The right hand variable to compare.
/// @returns True.
inline bool
operator==(const variable& lhs, const variable& rhs) noexcept {
    return true;
}

} // namespace cherry::ast