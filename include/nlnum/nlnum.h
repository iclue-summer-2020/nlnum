// Copyright (c) 2020 ICLUE @ UIUC. All rights reserved.

#ifndef NLNUM_NLNUM_H_
#define NLNUM_NLNUM_H_

#include <cstdint>
#include <map>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


extern "C" {
#include <lrcalc/hashtab.h>
#include <lrcalc/vector.h>
}

namespace py = pybind11;

namespace nlnum {
// Converts a C++ vector into a C vector defined by lrcalc.
vector* to_vector(const std::vector<int>&);

// Converts a C vector defined by lrcalc into a C++ vector.
bool to_cppvec(const vector*, std::vector<int>*);

// Converts a C hashtable defined by lrcalc to a C++ map.
bool to_map(hashtab*, std::map<std::vector<int>, int>*);

// Computes the Littlewood-Richardson coefficient.
int64_t lrcoef(
    const std::vector<int>& outer,
    const std::vector<int>& inner1,
    const std::vector<int>& inner2);

// Computes the Newell-Littlewood coefficient using Proposition 2.3.
int64_t nlcoef_slow(
    const std::vector<int>& mu,
    const std::vector<int>& nu,
    const std::vector<int>& lambda);

// Computes the Newell-Littlewood coefficient using the definition 1.1.
int64_t nlcoef(
    const std::vector<int>& mu,
    const std::vector<int>& nu,
    const std::vector<int>& lambda);

}  // namespace nlnum

#endif  // NLNUM_NLNUM_H_
