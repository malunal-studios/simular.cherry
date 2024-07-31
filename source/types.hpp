#pragma once
#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>


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


template<typename Underlying>
using uptr_t = std::unique_ptr<Underlying>;

template<typename Underlying>
using sptr_t = std::shared_ptr<Underlying>;

template<typename Underlying>
using wptr_t = std::weak_ptr<Underlying>;

template<typename Data>
using opt_t = std::optional<Data>;

template<typename T, typename U>
using pair_t = std::pair<T, U>;

template<typename Data>
using stack_t = std::stack<Data>;

template<typename Data>
using vec_t = std::vector<Data>;

template<typename Key, typename Value>
using udict_t = std::unordered_map<Key, Value>;

template<typename Key, typename Value>
using odict_t = std::map<Key, Value>;

template<typename Key, typename Value>
using umdict_t = std::unordered_multimap<Key, Value>;

template<typename Key, typename Value>
using omdict_t = std::multimap<Key, Value>;

template<typename Key>
using uset_t = std::unordered_set<Key>;

template<typename Key>
using oset_t = std::set<Key>;

using string_t = std::string;
using strview_t = std::string_view;

} // namespace cherry