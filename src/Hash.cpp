/**
Hash.cpp

Copyright (c) 2021 Yasuo Tabei

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

#include "Hash.hpp"

using namespace std;

const uint32_t HASH::kUndefPos;

uint64_t HASH::scaleUpHashSize() {
  return primes[++primePointer_];
  //  return (size << 1U);
}

void HASH::init(const double loadfactor, uint64_t countCode) {
  loadfactor_ = loadfactor;
  next_.clear();
  hash_.clear();
  primePointer_ = 25;
  hash_.resize(primes[primePointer_], kUndefPos);
  countCode_ = countCode;
}

uint64_t HASH::computeHashVal(const uint64_t left, 
			      const uint64_t right) const {
  return (left^(right+3)*4099) % hash_.size();
}

void HASH::reHash() {
  for (size_t i = 0; i != hash_.size(); ++i)
    hash_[i] = kUndefPos;
  for (size_t i = 0; i != next_.size(); ++i)
    next_[i] = kUndefPos;
  for (size_t i = 0; i != rules_.size(); ++i) {
    uint64_t h = computeHashVal(rules_[i].left_, 
				rules_[i].right_);
    if (hash_[h] == kUndefPos)
      hash_[h] = i;
    else {
      next_[i] = hash_[h];
      hash_[h] = i;
    }
  }
}

void HASH::reHash(uint64_t hashSize) {
  hash_.resize(hashSize);
  reHash();
}

void HASH::addRule(const uint64_t left, const uint64_t right) {
  rules_.push_back(RULE(countCode_++, left, right));
  next_.push_back(kUndefPos);

  uint64_t h = computeHashVal(left, right);
  if (hash_[h] == kUndefPos) {
    hash_[h] = rules_.size() - 1;
  }
  else {
    next_.back() = hash_[h];
    hash_[h] = rules_.size() - 1;
  }

  if (rules_.size() * loadfactor_ > hash_.size()) {
    cerr << "rehash: " << endl;
    reHash(scaleUpHashSize());
  }
}

bool HASH::reverseAccess(const uint64_t left, 
			 const uint64_t right) {
  uint64_t h = computeHashVal(left, right);
  uint64_t target = hash_[h];
  while (target != kUndefPos) {
    if ((rules_[target].left_ == left) && 
	(rules_[target].right_ == right)) {
      return true;
    }
    target = next_[target];
  }
  return false;
}

bool HASH::getCode(const uint64_t left, const uint64_t right) {
  return reverseAccess(left, right);
}

uint64_t HASH::replace(const uint64_t left, const uint64_t right) {
  uint64_t h = computeHashVal(left, right);
  uint64_t target = hash_[h];
  while (target != kUndefPos) {
    if ((rules_[target].left_ == left) && 
	(rules_[target].right_ == right)) {
      return rules_[target].code_;
    }
    target = next_[target];
  }
  return kDummy;
}

void HASH::setDictionary(vector<pair<uint64_t, uint64_t> > &dictionary, uint64_t alphabet) {
  for (size_t i = 0; i < rules_.size(); ++i) {
    if (dictionary.size() <= rules_[i].code_ - alphabet)
      dictionary.resize(rules_[i].code_ + 1 - alphabet);
    dictionary[rules_[i].code_ - alphabet] = make_pair(rules_[i].left_, rules_[i].right_);
  }
}

void HASH::setDictionary(ostream &os) {
  for (size_t i = 0; i < rules_.size(); ++i) {
    uint64_t left  = rules_[i].left_;
    uint64_t right = rules_[i].right_;
    os.write((const char*)(&left), sizeof(left));
    os.write((const char*)(&right), sizeof(right));
  }
}

