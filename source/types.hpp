#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>


namespace cherry {

using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::size_t;


template<typename Data>
using vec_t = std::vector<Data>;

template<typename Key, typename Value>
using dict_t = std::unordered_map<Key, Value>;

template<typename Key>
using set_t = std::unordered_set<Key>;

using string_t = std::string;
using strview_t = std::string_view;

} // namespace cherry