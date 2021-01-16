/**
   Hash.hpp

   Copyright (c) 2021 Yasuo Tabei

   This software is released under the MIT License.
   http://opensource.org/licenses/mit-license.php
*/

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <stdint.h>
#include <algorithm>
#include <unordered_map>

#include "Util.hpp"

class RULE {
public:
  RULE() {
    code_  = kDummy;
    left_  = kDummy;
    right_ = kDummy;
  }

  RULE(uint64_t code,
       uint64_t left, 
       uint64_t right) {
    code_  = code;
    left_  = left;
    right_ = right;
  }
  
public:
  uint64_t code_;
  uint64_t left_;
  uint64_t right_;
};

class HASH {
private:
  bool reverseAccess(const uint64_t left, 
		     const uint64_t right);
  void reHash();
  void reHash(uint64_t hashSize);
  uint64_t computeHashVal(const uint64_t left, 
			  const uint64_t right) const;
  uint64_t scaleUpHashSize();
public:
  HASH() {
    delta_      = 0;
    loadfactor_ = 0.0;
    rules_.clear();
    next_.clear();
    hash_.clear();
  }
  void init(const double loadfactor, uint64_t countCode);
  void reDict();
  bool getCode(const uint64_t left, const uint64_t right);
  uint64_t replace(const uint64_t left, const uint64_t right);
  void setDictionary(std::vector<std::pair<uint64_t, uint64_t> > &dictionary, uint64_t alphabet);
  void setDictionary(std::ostream &os);
  void addRule(const uint64_t left, const uint64_t right);
  uint64_t dictNumRules() const {
    return rules_.size();
  }
  uint64_t getTopKRules(uint64_t topk, uint64_t &numRules);

  uint64_t spaceInBytes() {
    uint64_t bytes = 0;
    bytes += rules_.size() * sizeof(RULE);
    bytes += next_.size() * sizeof(uint32_t);
    bytes += hash_.size() * sizeof(uint32_t);
    bytes += sizeof(HASH);
    return bytes;
  }
private:
  static const uint32_t kUndefPos = UINT_MAX;
  uint64_t delta_;
  double   loadfactor_;
  uint64_t primePointer_;
  std::vector<RULE> rules_;
  std::vector<uint32_t> next_;
  std::vector<uint32_t> hash_;
  uint64_t countCode_;
};
