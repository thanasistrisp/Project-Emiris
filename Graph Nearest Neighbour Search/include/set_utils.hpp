#pragma once

#include <tuple>
#include <functional>

inline bool set_cmp(const std::pair<int, double> *left, const std::pair<int, double> *right) { return left->second < right->second; }
inline bool set_hash(const std::pair<int, double> *p){ return std::hash<int>()(p->first); };
inline bool set_equal(const std::pair<int, double> *p1, const std::pair<int, double> *p2){ return p1->first == p2->first; };