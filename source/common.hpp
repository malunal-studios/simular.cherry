#pragma once
#include <algorithm>
#include <cstdint>
#include <concepts>
#include <expected>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <memory_resource>
#include <optional>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <simular/allocators.hpp>
#include <simular/tooling.hpp>


namespace simular::cherry {

// Types we use alot.
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::intptr_t;
using std::uintptr_t;
using std::ptrdiff_t;
using std::size_t;
using std::expected;
using std::unexpected;

template<typename T>
using sptr_t = std::shared_ptr<T>;

template<typename T, typename U>
using pair_t = std::pair<T, U>;

template<typename Key, typename Value>
using odict_t = std::map<Key, Value>;

template<typename Key, typename Value>
using udict_t = std::unordered_map<Key, Value>;

template<typename Key, typename Value>
using omdict_t = std::multimap<Key, Value>;

template<typename Key, typename Value>
using umdict_t = std::unordered_multimap<Key, Value>;

template<typename Key>
using oset_t = std::set<Key>;

template<typename Key>
using uset_t = std::unordered_set<Key>;

template<typename Data>
using stack_t = std::stack<Data>;

template<typename T>
using vec_t = std::pmr::vector<T>;

using string_t = std::string;
using strview_t = std::string_view;


/// @brief   Returns a vector that uses the default arena as the memory resource
///          that it will allocate into.
/// @tparam  T The type of data that the vector will store.
/// @returns A vector constructed with the default arena memory resource such
///          that it will allocate data into the default arena when requested.
template<typename T>
inline vec_t<T>
arena_make_vector() {
    using allocators::arena_allocator_instance;
    return vec_t<T>(arena_allocator_instance());
}


} // namespace simular::cherry
