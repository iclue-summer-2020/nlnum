// Copyright 2020 ICLUE @ UIUC. All rights reserved.

#include <algorithm>
#include <climits>
#include <vector>

#include <nlnum/partitions_in.h>

namespace nlnum {

PartitionsIn::PartitionsIn(const Partition& limit, const size_t size)
    : limit_{limit}, size_{size} {}

PartitionsIn::const_iterator PartitionsIn::begin() const {
  return const_iterator{limit_, size_};
}

PartitionsIn::const_iterator PartitionsIn::end() const {
  const Partition p;
  return const_iterator{p, 0};
}

PartitionsIn::const_iterator::const_iterator(const Partition& limit,
                                             const size_t size)
    : limit_{limit}, size_{size}, done_{false} {
  // Keep track of suffix sums.
  for (auto it = limit_.rbegin(); it != limit_.rend(); ++it) {
    const int last = rsums_.size() > 0 ? rsums_.back() : 0;
    rsums_.push_back(last + *it);
  }
  std::reverse(rsums_.begin(), rsums_.end());

  call_stack_.push_back(new var{0, size_, 1, -1, nullptr});

  ++(*this);
}

bool PartitionsIn::const_iterator::GoBack(var* v) {
  var* came_from = v->came_from;
  delete v;

  if (came_from == nullptr) return false;
  parts_.pop_back();
  ++came_from->mn;
  call_stack_.push_back(came_from);
  return true;
}

bool PartitionsIn::const_iterator::Next() {
  while (!call_stack_.empty()) {
    var* v = call_stack_.back();
    call_stack_.pop_back();

    if (v->rem == 0 && parts_.size() <= limit_.size()) {
      ret_parts_ = parts_;
      if (!GoBack(v)) break;
      return true;
    } else if (v->rem < 0 || v->level >= limit_.size() ||
               rsums_[v->level] < v->rem ||
               (parts_.size() > 0 &&
                parts_.back() * (limit_.size() - parts_.size()) < v->rem)) {
      if (!GoBack(v)) break;
    } else if (v->mx == -1) {
      const int max_part = std::min(
          v->rem, std::min(limit_[v->level], parts_.size() > 0
                                                 ? parts_.back()
                                                 : static_cast<int>(size_)));
      v->mx = max_part;
      if (v->mn <= v->mx) {
        parts_.push_back(1);
        call_stack_.push_back(new var{v->level + 1, v->rem - 1, 1, -1, v});
      } else if (!GoBack(v)) {
        break;
      }
    } else if (v->mn <= v->mx) {
      parts_.push_back(v->mn);
      call_stack_.push_back(new var{v->level + 1, v->rem - v->mn, 1, -1, v});
    } else if (!GoBack(v)) {
      break;
    }
  }

  return false;
}

PartitionsIn::const_iterator& PartitionsIn::const_iterator::operator++() {
  if (!Next()) {
    done_ = true;
  }
  return *this;
}

const Partition& PartitionsIn::const_iterator::operator*() const {
  return ret_parts_;
}

bool PartitionsIn::const_iterator::operator!=(
    const nlnum::PartitionsIn::const_iterator& rhs) const {
  return !done_ || !rhs.done_;
}

// Computes the intersection of two partitions.
const Partition Intersection(const Partition& a, const Partition& b) {
  const size_t n = std::min(a.size(), b.size());
  Partition c(n);
  for (size_t i = 0; i < n; ++i) {
    c[i] = std::min(a[i], b[i]);
  }

  return c;
}

}  // namespace nlnum